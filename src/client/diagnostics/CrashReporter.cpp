// Local-only exception evidence for the legacy 32-bit client.
// Initialization runs after the game's CRT startup, never under DllMain's loader lock.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>
#include <strsafe.h>

static HANDLE requestEvent, doneEvent;
static volatile LONG initialized, capturing;
static EXCEPTION_RECORD exceptionRecord;
static CONTEXT exceptionContext;
static EXCEPTION_POINTERS exceptionPointers = { &exceptionRecord, &exceptionContext };
static DWORD exceptionThread;
static WCHAR directory[MAX_PATH], executable[MAX_PATH];
static LPTOP_LEVEL_EXCEPTION_FILTER previousFilter;
typedef BOOL (WINAPI *WriteDump)(HANDLE,DWORD,HANDLE,MINIDUMP_TYPE,
    PMINIDUMP_EXCEPTION_INFORMATION,PMINIDUMP_USER_STREAM_INFORMATION,PMINIDUMP_CALLBACK_INFORMATION);
static WriteDump writeDump;

static void WriteText(HANDLE file, const char* text) {
    DWORD written;
    if (file != INVALID_HANDLE_VALUE) WriteFile(file, text, lstrlenA(text), &written, NULL);
}

static DWORD WINAPI CaptureThread(void*) {
    if (WaitForSingleObject(requestEvent, INFINITE) != WAIT_OBJECT_0) return 0;
    SYSTEMTIME time; GetSystemTime(&time);
    WCHAR stem[MAX_PATH], path[MAX_PATH];
    StringCchPrintfW(stem, MAX_PATH, L"%s\\crash-%04u%02u%02u-%02u%02u%02u-%lu",
        directory,time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,GetCurrentProcessId());
    StringCchPrintfW(path,MAX_PATH,L"%s.txt",stem);
    HANDLE text=CreateFileW(path,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
    char line[1024];
    StringCchPrintfA(line,1024,"LAQIA crash report v1 (UTC)\r\nexception=0x%08lX\r\naddress=0x%08lX\r\nthread=%lu\r\n",
        exceptionRecord.ExceptionCode,(DWORD)(ULONG_PTR)exceptionRecord.ExceptionAddress,exceptionThread);
    WriteText(text,line);
    HMODULE module=NULL;
    WCHAR modulePath[MAX_PATH]=L"unknown";
    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCWSTR)exceptionRecord.ExceptionAddress,&module)) GetModuleFileNameW(module,modulePath,MAX_PATH);
    char moduleUtf8[MAX_PATH*3]={0};
    WideCharToMultiByte(CP_UTF8,0,modulePath,-1,moduleUtf8,sizeof(moduleUtf8),NULL,NULL);
    StringCchPrintfA(line,1024,"module=%s\r\nmodule_base=0x%08lX\r\nmodule_rva=0x%08lX\r\n",
        moduleUtf8,(DWORD)(ULONG_PTR)module,(DWORD)((ULONG_PTR)exceptionRecord.ExceptionAddress-(ULONG_PTR)module));
    WriteText(text,line);
    StringCchPrintfA(line,1024,"eax=%08lX ebx=%08lX ecx=%08lX edx=%08lX\r\nesi=%08lX edi=%08lX ebp=%08lX esp=%08lX eip=%08lX\r\n",
        exceptionContext.Eax,exceptionContext.Ebx,exceptionContext.Ecx,exceptionContext.Edx,
        exceptionContext.Esi,exceptionContext.Edi,exceptionContext.Ebp,exceptionContext.Esp,exceptionContext.Eip);
    WriteText(text,line);
    if(exceptionRecord.ExceptionCode==EXCEPTION_ACCESS_VIOLATION && exceptionRecord.NumberParameters>=2) {
        StringCchPrintfA(line,1024,"access_operation=%lu\r\naccess_address=0x%08lX\r\n",
            (DWORD)exceptionRecord.ExceptionInformation[0],(DWORD)exceptionRecord.ExceptionInformation[1]);
        WriteText(text,line);
    }
    StringCchPrintfW(path,MAX_PATH,L"%s.dmp",stem);
    HANDLE dump=CreateFileW(path,GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
    BOOL ok=FALSE; DWORD error=ERROR_PROC_NOT_FOUND;
    if(dump!=INVALID_HANDLE_VALUE && writeDump) {
        MINIDUMP_EXCEPTION_INFORMATION info={exceptionThread,&exceptionPointers,FALSE};
        // Stacks, registers and module metadata; no full process-memory collection.
        ok=writeDump(GetCurrentProcess(),GetCurrentProcessId(),dump,
            (MINIDUMP_TYPE)(MiniDumpNormal|MiniDumpWithUnloadedModules|MiniDumpWithThreadInfo),&info,NULL,NULL);
        error=ok?0:GetLastError();
    } else if(dump==INVALID_HANDLE_VALUE) error=GetLastError();
    if(dump!=INVALID_HANDLE_VALUE) CloseHandle(dump);
    StringCchPrintfA(line,1024,"minidump_written=%u\r\nminidump_error=%lu\r\n",ok,error);
    WriteText(text,line);
    if(text!=INVALID_HANDLE_VALUE) { FlushFileBuffers(text); CloseHandle(text); }
    SetEvent(doneEvent);
    return 0;
}

static LONG WINAPI OnException(EXCEPTION_POINTERS* pointers) {
    if(InterlockedCompareExchange(&capturing,1,0)==0 && pointers && pointers->ExceptionRecord && pointers->ContextRecord) {
        exceptionRecord=*pointers->ExceptionRecord;
        exceptionRecord.ExceptionRecord=NULL;
        exceptionContext=*pointers->ContextRecord;
        exceptionThread=GetCurrentThreadId();
        SetEvent(requestEvent);
        WaitForSingleObject(doneEvent,8000); // A failed diagnostic must not hang the game indefinitely.
    }
    return previousFilter?previousFilter(pointers):EXCEPTION_CONTINUE_SEARCH;
}

extern "C" __declspec(dllexport) BOOL WINAPI InitializeCrashReporter() {
    if(InterlockedCompareExchange(&initialized,1,0)!=0) return TRUE;
    DWORD length=GetModuleFileNameW(NULL,executable,MAX_PATH);
    if(!length || length>=MAX_PATH-32) return FALSE;
    StringCchCopyW(directory,MAX_PATH,executable);
    WCHAR* slash=NULL;
    for(WCHAR* p=directory;*p;++p) if(*p==L'\\' || *p==L'/') slash=p;
    if(!slash) return FALSE;
    *slash=0; StringCchCatW(directory,MAX_PATH,L"\\CrashDumps");
    if(!CreateDirectoryW(directory,NULL) && GetLastError()!=ERROR_ALREADY_EXISTS) return FALSE;
    // Load the system DbgHelp before any fault or loader lock is involved.
    WCHAR dbghelp[MAX_PATH];
    if(!GetSystemDirectoryW(dbghelp,MAX_PATH-16)) return FALSE;
    StringCchCatW(dbghelp,MAX_PATH,L"\\dbghelp.dll");
    HMODULE library=LoadLibraryW(dbghelp);
    if(library) writeDump=(WriteDump)GetProcAddress(library,"MiniDumpWriteDump");
    requestEvent=CreateEventW(NULL,TRUE,FALSE,NULL);
    doneEvent=CreateEventW(NULL,TRUE,FALSE,NULL);
    if(!requestEvent || !doneEvent) return FALSE;
    HANDLE thread=CreateThread(NULL,256*1024,CaptureThread,NULL,0,NULL);
    if(!thread) return FALSE;
    CloseHandle(thread);
    ULONG reserve=64*1024; SetThreadStackGuarantee(&reserve);
    previousFilter=SetUnhandledExceptionFilter(OnException);
    return TRUE;
}
