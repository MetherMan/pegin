@echo off
cd /d "%~dp0"
git pull --ff-only
if errorlevel 1 (pause& exit /b 1)
"runtime\python\python.exe" -X utf8 "tools\manage.py" sync
if errorlevel 1 pause
