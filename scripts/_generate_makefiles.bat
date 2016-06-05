@set bat_dir=%~dp0
"%bat_dir%\..\tools\rux.configure\windows\x86\rux.configure" --generate-makefile "%bat_dir%\..\rux.framework\build.ruxprj"
"%bat_dir%\..\tools\rux.configure\windows\x86\rux.configure" --generate-makefile "%bat_dir%\..\rux.engine\build.ruxprj"
"%bat_dir%\..\tools\rux.configure\windows\x86\rux.configure" --generate-makefile "%bat_dir%\..\rux.crypto\build.ruxprj"
"%bat_dir%\..\tools\rux.configure\windows\x86\rux.configure" --generate-makefile "%bat_dir%\..\rux.neficl\build.ruxprj"
"%bat_dir%\..\tools\rux.configure\windows\x86\rux.configure" --generate-makefile "%bat_dir%\..\ruxiver\build.ruxprj"
"%bat_dir%\..\tools\rux.configure\windows\x86\rux.configure" --generate-makefile "%bat_dir%\..\rux.configure\build.ruxprj"
@pause