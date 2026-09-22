@echo off
chcp 65001 >nul
"%~dp0..\..\..\..\python\python.exe" -X utf8 "%~dp0edit_colors.py"
if errorlevel 1 pause
