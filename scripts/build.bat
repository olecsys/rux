@setlocal enableextensions enabledelayedexpansion
@set bat_file=%~n0
@set bat_dir=%~dp0

@for /f "tokens=*" %%a in ('where git.exe') do @set filename=%%a

@for %%F in ("%filename%") do @set gitdir=%%~dpF
@set gitdir=!gitdir:~0,-1!
@for %%b in ("%gitdir%") do @set shdir=%%~dpb
@set shfilename=%shdir%bin\sh.exe

@if not "%~9"=="" @goto :9arguments
@if not "%~8"=="" @goto :8arguments
@if not "%~7"=="" @goto :7arguments
@if not "%~6"=="" @goto :6arguments
@if not "%~5"=="" @goto :5arguments
@if not "%~4"=="" @goto :4arguments
@if not "%~3"=="" @goto :3arguments
@if not "%~2"=="" @goto :2arguments
@if not "%~1"=="" @goto :1arguments
@goto :0arguments

:9arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1" "%~2" "%~3" "%~4" "%~5" "%~6" "%~7" "%~8" "%~9"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:8arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1" "%~2" "%~3" "%~4" "%~5" "%~6" "%~7" "%~8"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:7arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1" "%~2" "%~3" "%~4" "%~5" "%~6" "%~7"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:6arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1" "%~2" "%~3" "%~4" "%~5" "%~6"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:5arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1" "%~2" "%~3" "%~4" "%~5"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:4arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1" "%~2" "%~3" "%~4"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:3arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1" "%~2" "%~3"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:2arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1" "%~2"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:1arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh" "%~1"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:0arguments
"%shfilename%" --login -i "%bat_dir%%bat_file%.sh"
@if !ERRORLEVEL! NEQ 0 @goto :error
@goto :done

:error
@echo "********** ERROR: %bat_file% **********"
@endlocal
@EXIT /b 1

:done
@echo "********** SUCCESS: %bat_file% **********"
@endlocal
@EXIT /b 0