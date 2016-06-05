@echo off
setlocal enabledelayedexpansion
@set replace=
@set line=%1
@set find="
@echo off
setlocal enabledelayedexpansion
if not '%1' == '' @(
@set targetdir=!line:%find%=%replace%!
)
@set bat_dir=%~dp0
@echo on
@if "%targetdir%" == "" @set targetdir=%bat_dir%

@set old_bat_dir=%bat_dir%
@set old_targetdir=%targetdir%

del /q /a "%targetdir%\"*.sdf
del /q /a "%targetdir%\"*.opensdf
del /q /a "%targetdir%\"*.pdb
del /q /a "%targetdir%\"*.obj
del /q /a "%targetdir%\"*.ipch
del /q /a "%targetdir%\"*.tlog
del /q /a "%targetdir%\"*.suo
del /q /a "%targetdir%\"*.vcxproj.filters
del /q /a "%targetdir%\"*.vcxproj.user
del /q /a "%targetdir%\"*.Build.CppClean.log
del /q /a "%targetdir%\"*.dll.intermediate.manifest
del /q /a "%targetdir%\"*.exe.intermediate.manifest
del /q /a "%targetdir%\"*.lastbuildstate
del /q /a "%targetdir%\"*.vcxprojResolveAssemblyReference.cache

for /d %%x in ("%targetdir%\"*) do @call %0 ^"%%x^"

@set bat_dir=%old_bat_dir%
@set targetdir=%old_targetdir%