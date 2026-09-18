@echo off
cd /d "%~dp0"
"runtime\python\python.exe" -X utf8 "tools\build_twilight.py"
if errorlevel 1 pause
