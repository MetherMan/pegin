// Standalone verification process. Never deliberately faults the running game.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
__declspec(noinline) static void Overflow(unsigned n) {
    volatile char stack[4096]; stack[n%4096]=(char)n;
    Overflow(n+1);
    if(stack[0]==42) OutputDebugStringA("unreachable");
}
static DWORD WINAPI FaultThread(void*) { *(volatile int*)0=42; return 0; }
int main(int argc,char** argv) {
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
    HMODULE dll=LoadLibraryW(L"LaqiaCrash.dll");
    if(!dll) return 2;
    typedef BOOL (WINAPI *Initialize)();
    Initialize init=(Initialize)GetProcAddress(dll,"InitializeCrashReporter");
    if(!init || !init()) return 3;
    if(argc==1) return 0;
    if(argv[1][0]=='s') Overflow(0);
    else if(argv[1][0]=='t') { HANDLE thread=CreateThread(NULL,0,FaultThread,NULL,0,NULL); WaitForSingleObject(thread,INFINITE); }
    else if(argv[1][0]=='h') { __try { *(volatile int*)0=42; } __except(EXCEPTION_EXECUTE_HANDLER) { return 0; } }
    else *(volatile int*)0=42;
    return 4;
}
