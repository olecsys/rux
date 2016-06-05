@set bat_dir=%~dp0
@if not exist "%bat_dir%..\includes" @md "%bat_dir%..\includes"
copy /V /Y "%bat_dir%lib\"*.h "%bat_dir%..\includes"