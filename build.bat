@echo off
setlocal
MSBUILD TopUI.sln /t:Rebuild /p:Configuration=Debug
MSBUILD TopUI.sln /t:Rebuild /p:Configuration=Release
MSBUILD TopUI.sln /t:Rebuild /p:Configuration=UnicodeDebug
MSBUILD TopUI.sln /t:Rebuild /p:Configuration=UnicodeRelease
exit /b %ERRORLEVEL%
endlocal
pause