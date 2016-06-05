@set bat_dir=%~dp0
@set temp_bat_file=%temp%\rux_temp.bat
@if exist "%temp_bat_file%" del "%temp_bat_file%"
@echo setx PATH "%PATH%;%bat_dir%..\..\tools\depot_tools;%bat_dir%..\..\tools\depot_tools\python276_bin" /M > "%temp_bat_file%"
@echo reg add HKCU\Software\__RuxUpdateRegTEMP /f >> "%temp_bat_file%"
@cmd.exe /c "%temp_bat_file%"
@if exist "%temp_bat_file%" del "%temp_bat_file%"