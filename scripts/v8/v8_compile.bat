@echo off
setlocal enabledelayedexpansion
@set bat_dir=%~dp0
@set base_path=%bat_dir%..\..

@if exist "%base_path%\tools\depot_tools\gclient" @goto :skip_depot_tools

cd %temp%
@if %ERRORLEVEL% NEQ 0 @goto :errors

@if exist "%temp%\depot_tools.zip" @goto :skip_wget

wget --no-check-certificate https://src.chromium.org/svn/trunk/tools/depot_tools.zip
@if %ERRORLEVEL% NEQ 0 @goto :delete_zip

:skip_wget

@mkdir "%base_path%\tools\depot_tools"

unzip "%temp%/depot_tools.zip" -d "%base_path%/tools/depot_tools"
@if %ERRORLEVEL% NEQ 0 @goto :delete_zip

for /f "delims=" %%a in ('dir "%base_path%\tools\depot_tools" /b /ad ') do (
	@xcopy /e /v /q /r /y "%base_path%\tools\depot_tools\%%a" "%base_path%\tools\depot_tools"
    @rmdir /S /Q "%base_path%\tools\depot_tools\%%a"
)

:skip_depot_tools

for %%X in (gclient) do (set gclient_exists=%%~$PATH:X)
if defined gclient_exists @goto :skip_depot_tools_registry

@call "%bat_dir%/../_start_bat_file_under_admin.bat" "%bat_dir%/setvars.bat"
@if %ERRORLEVEL% NEQ 0 @goto :errors

:skip_depot_tools_registry

for %%X in (gclient) do (set gclient_exists=%%~$PATH:X)
@if not defined gclient_exists @echo Please call this script again	
@if not defined gclient_exists @goto :done

@call gclient
@if %ERRORLEVEL% NEQ 0 @goto :errors

@if exist "%base_path%\downloaded_sources\v8\build\gyp_v8" @goto :skip_fetch_v8

@mkdir "%base_path%\downloaded_sources"

cd "%base_path%\downloaded_sources"
@if %ERRORLEVEL% NEQ 0 @goto :errors

fetch v8
@if %ERRORLEVEL% NEQ 0 @goto :errors

:skip_fetch_v8

cd "%base_path%\downloaded_sources\v8"
@if %ERRORLEVEL% NEQ 0 @goto :errors

set platform=Win32
set msbuild_path=C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild.exe

@if not exist "%base_path%\downloaded_sources\v8\build\all.sln" @goto :skip_sln_clean
"%msbuild_path%" /m "%base_path%\downloaded_sources\v8\build\all.sln" /t:Clean /p:Configuration="Release"

:skip_sln_clean

del /q /a "%base_path%\downloaded_sources\v8\build\"*.vcxproj
@if %ERRORLEVEL% NEQ 0 @goto :errors

del /q /a "%base_path%\downloaded_sources\v8\build\"*.sln
@if %ERRORLEVEL% NEQ 0 @goto :errors

call python build\gyp_v8 -Dcomponent=static_library -Dtarget_arch=%platform% -Dv8_use_snapshot=0 -Dv8_enable_i18n_support=0
@if %ERRORLEVEL% NEQ 0 @goto :errors

"%msbuild_path%" /m "%base_path%\downloaded_sources\v8\build\all.sln" /t:v8;v8_libplatform;v8_libbase /p:Configuration="Release" /p:Platform="%platform%"
@if %ERRORLEVEL% NEQ 0 @goto :errors

@mkdir "%base_path%\extern_lib\windows\%platform%\v8"

@xcopy /e /v /q /r /y "%base_path%\downloaded_sources\v8\build\Release\"*.lib "%base_path%\extern_lib\windows\%platform%\v8"
@if %ERRORLEVEL% NEQ 0 @goto :errors

"%msbuild_path%" /m "%base_path%\downloaded_sources\v8\build\all.sln" /t:Clean /p:Configuration="Release" /p:Platform="%platform%"
@if %ERRORLEVEL% NEQ 0 @goto :errors

set platform=x64

del /q /a "%base_path%\downloaded_sources\v8\build\"*.vcxproj
@if %ERRORLEVEL% NEQ 0 @goto :errors

del /q /a "%base_path%\downloaded_sources\v8\build\"*.sln
@if %ERRORLEVEL% NEQ 0 @goto :errors

call python build\gyp_v8 -Dcomponent=static_library -Dtarget_arch=%platform% -Dv8_use_snapshot=0 -Dv8_enable_i18n_support=0
@if %ERRORLEVEL% NEQ 0 @goto :errors

"%msbuild_path%" /m "%base_path%\downloaded_sources\v8\build\all.sln" /t:v8;v8_libplatform;v8_libbase /p:Configuration="Release" /p:Platform="%platform%"
@if %ERRORLEVEL% NEQ 0 @goto :errors

@mkdir "%base_path%\extern_lib\windows\%platform%\v8"

@xcopy /e /v /q /r /y "%base_path%\downloaded_sources\v8\build\Release\"*.lib "%base_path%\extern_lib\windows\%platform%\v8"
@if %ERRORLEVEL% NEQ 0 @goto :errors

"%msbuild_path%" /m "%base_path%\downloaded_sources\v8\build\all.sln" /t:Clean /p:Configuration="Release" /p:Platform="%platform%"
@if %ERRORLEVEL% NEQ 0 @goto :errors

@mkdir "%base_path%\extern_includes\v8\include"

@xcopy /e /v /q /r /y "%base_path%\downloaded_sources\v8\include\"*.h "%base_path%\extern_includes\v8\include"
@if %ERRORLEVEL% NEQ 0 @goto :errors

@goto :done
:delete_zip
@if exist "%temp%\depot_tools.zip" del "%temp%\depot_tools.zip"
:errors
@echo WARNING! Error(s) were detected!
@pause
@endlocal
exit /b %ERRORLEVEL%
:done
@pause
@endlocal
exit /b 0