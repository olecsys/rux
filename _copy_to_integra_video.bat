set copy_bat_file=%1
set copy_configuration=%2
set copy_platform=%3
set ask=%4
@set bat_dir=%~dp0
set OLDDIR=%CD%
set core_path="F:\SVN\Integra.Video"
@if not exist "%core_path%" set core_path="D:\core"
@if not exist "%core_path%" set core_path="%bat_dir%\..\core"
if "%ask%"=="compile" goto compile
if "%ask%"=="only_copy" goto only_copy
echo "Do you want to rebuid all rux projects?"
set /p is_rebuild=Do you want to rebuild all rux projects? (y/n):
@VERIFY > nul
if %is_rebuild%==n goto only_copy
:compile
rmdir /S /Q %OLDDIR%\build\%copy_platform%
rmdir /S /Q %OLDDIR%\includes
@call %copy_bat_file%
:only_copy
if %ERRORLEVEL% NEQ 0 goto errors
pushd %OLDDIR%
@if not exist "%core_path%\crox_headers" @md "%core_path%\crox_headers"
@if not exist "%core_path%\crox_headers\openssl" @md "%core_path%\crox_headers\openssl"
@copy /Y /V  "%OLDDIR%\includes\"*.h "%core_path%\crox_headers"
@copy /Y /V  "%OLDDIR%\extern_includes\openssl\"*.h "%core_path%\crox_headers\openssl"

@if not exist "%core_path%\crox_lib" @md "%core_path%\crox_lib"
@if not exist "%core_path%\crox_lib\%copy_platform%" @md "%core_path%\crox_lib\%copy_platform%"
@if not exist "%core_path%\crox_lib\%copy_platform%\%copy_configuration%" @md "%core_path%\crox_lib\%copy_platform%\%copy_configuration%"
@copy /Y /V "%OLDDIR%\build\%copy_platform%\"*.lib "%core_path%\crox_lib\%copy_platform%\%copy_configuration%"
copy /V /Y  "%OLDDIR%\build\%copy_platform%\rux.engine.dll" "%core_path%\bin\windows\%copy_platform%\rux.engine.dll"
copy /V /Y  "%OLDDIR%\build\%copy_platform%\rux.engine.pdb" "%core_path%\bin\windows\%copy_platform%\rux.engine.pdb"
popd
goto done
:errors
exit /b 1
:done
exit /b 0