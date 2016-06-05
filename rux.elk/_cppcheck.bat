@set bat_dir=%~dp0
@rem unix32 unix64 wind32A win32W win64
@rem --enable=warning,style,performance,portability
"C:\Program Files (x86)\Cppcheck\cppcheck.exe" -q -j4 --enable=warning,style --platform=win64 --std=posix --suppress=variableScope "%bat_dir%lib" 2> "%bat_dir%lib\cppcheck_errors.txt"
@pause
