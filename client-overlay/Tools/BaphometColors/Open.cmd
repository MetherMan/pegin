@echo off
chcp 65001 >nul
set "BAPHOMET_PYTHON=%~dp0..\..\..\..\python\python.exe"
if not exist "%BAPHOMET_PYTHON%" set "BAPHOMET_PYTHON=%~dp0..\..\..\runtime\python\python.exe"
"%BAPHOMET_PYTHON%" -B -X utf8 "%~dp0edit_skin.py"
if errorlevel 1 pause
