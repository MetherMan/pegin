@echo off
cd /d "%~dp0"
if not exist "runtime\python\python.exe" (echo Run Initialize.cmd first.& pause& exit /b 1)
"runtime\python\python.exe" -X utf8 "tools\manage.py" play
if errorlevel 1 pause
