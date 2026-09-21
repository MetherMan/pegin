@echo off
setlocal
cd /d "%~dp0.."
call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" >nul
if errorlevel 1 exit /b 1
if not exist .cache\crash-reporter mkdir .cache\crash-reporter
cl /nologo /W4 /O2 /MT /LD /Zi src\client\diagnostics\CrashReporter.cpp /Fo.cache\crash-reporter\CrashReporter.obj /Fd.cache\crash-reporter\compiler.pdb /link /OUT:.cache\crash-reporter\LaqiaCrash.dll /IMPLIB:.cache\crash-reporter\LaqiaCrash.lib /PDB:.cache\crash-reporter\LaqiaCrash.pdb /EXPORT:InitializeCrashReporter=_InitializeCrashReporter@0 /INCREMENTAL:NO
if errorlevel 1 exit /b 1
cl /nologo /W4 /Od /MT /Zi src\client\diagnostics\crash_fixture.cpp /Fo.cache\crash-reporter\crash_fixture.obj /Fd.cache\crash-reporter\fixture-compiler.pdb /link /OUT:.cache\crash-reporter\crash_fixture.exe /PDB:.cache\crash-reporter\crash_fixture.pdb /INCREMENTAL:NO
exit /b %errorlevel%
