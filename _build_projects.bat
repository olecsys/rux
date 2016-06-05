set bat_file=%1
set _build_project_platform=%2
set OLDDIR=%CD%

@if not exist "%OLDDIR%\extern_lib\windows\win32\zlibstat.lib" goto zlib_compile
@if not exist "%OLDDIR%\extern_lib\windows\win64\zlibstat.lib" goto zlib_compile
goto without_zlib_compile
:zlib_compile
pushd %OLDDIR%\extern_sources\zlib\contrib\vstudio\vc10
@call %CD%\%bat_file%
if %ERRORLEVEL% NEQ 0 goto errors
@if exist "%CD%\x86\ZlibStatRelease\zlibstat.lib" @copy   /V /Y  "%CD%\x86\ZlibStatRelease\zlibstat.lib" "%OLDDIR%\extern_lib\windows\win32\"
@if exist "%CD%\x64\ZlibStatRelease\zlibstat.lib" @copy   /V /Y  "%CD%\x64\ZlibStatRelease\zlibstat.lib" "%OLDDIR%\extern_lib\windows\win64\"
@copy   /V /Y  "%CD%\..\..\..\zlib.h" "%OLDDIR%\extern_includes\"
@copy   /V /Y  "%CD%\..\..\..\zconf.h" "%OLDDIR%\extern_includes\"
@copy   /V /Y  "%CD%\..\..\minizip\zip.h" "%OLDDIR%\extern_includes\"
@copy   /V /Y  "%CD%\..\..\minizip\unzip.h" "%OLDDIR%\extern_includes\"
@copy   /V /Y  "%CD%\..\..\minizip\ioapi.h" "%OLDDIR%\extern_includes\"
popd

:without_zlib_compile
pushd %OLDDIR%\rux.framework
@call %CD%\%bat_file%
if %ERRORLEVEL% NEQ 0 goto errors
popd
pushd %OLDDIR%\rux.engine
@call %CD%\%bat_file%
if %ERRORLEVEL% NEQ 0 goto errors
popd
pushd %OLDDIR%\rux.framework\test
@call %CD%\%bat_file%
if %ERRORLEVEL% NEQ 0 goto errors
popd
%OLDDIR%\build\%_build_project_platform%\rux.framework.test.exe
if %ERRORLEVEL% NEQ 0 goto errors
goto done
:errors
echo WARNING! Error(s) were detected!
@pause
exit /b %ERRORLEVEL%
:done
exit /b 0