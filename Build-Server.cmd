@echo off
cd /d "%~dp0"
"runtime\python\python.exe" -X utf8 "tools\manage.py" build
if errorlevel 1 pause
