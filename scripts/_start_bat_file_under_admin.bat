@echo off
setlocal enabledelayedexpansion
@set replace=
@set line=%1
@set find="
@echo off
setlocal enabledelayedexpansion 
if not '%1' == '' @(
@set bat_file=!line:%find%=%replace%!
)
@echo on

@echo off
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
if '%errorlevel%' NEQ '0' (
    goto UACPrompt
) else ( goto gotAdmin )
:UACPrompt
	@if exist "%temp%\getadmin.vbs" del "%temp%\getadmin.vbs"
    @echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    @echo UAC.ShellExecute "%bat_file%", "", "", "runas", 1 >> "%temp%\getadmin.vbs"
	@echo WScript.Sleep 3000 >> "%temp%\getadmin.vbs"
    @call "%temp%\getadmin.vbs"
	@endlocal
    @exit /B
:gotAdmin
    @if exist "%temp%\getadmin.vbs" del "%temp%\getadmin.vbs"
    @pushd "%CD%"
    @CD /D "%~dp0"
	@call "%bat_file%"	
	@endlocal