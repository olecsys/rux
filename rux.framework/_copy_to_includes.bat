@set bat_dir=%~dp0
@if not exist "%bat_dir%..\includes" @md "%bat_dir%..\includes"
copy /V /Y "%bat_dir%lib\"*.h "%bat_dir%..\includes"
copy /V /Y "%bat_dir%..\extern_includes\xan_platform.h" "%bat_dir%..\includes"
copy /V /Y "%bat_dir%..\extern_includes\xan_types.h" "%bat_dir%..\includes"
copy /V /Y "%bat_dir%..\extern_includes\xan_utils.h" "%bat_dir%..\includes"
copy /V /Y "%bat_dir%..\extern_includes\xan_utils_consts.h" "%bat_dir%..\includes"