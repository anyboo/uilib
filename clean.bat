@echo off
setlocal
call "%VS120COMNTOOLS%vsvars32.bat
MSBUILD TopUI.sln /t:Clean /p:Configuration=Debug
MSBUILD TopUI.sln /t:Clean /p:Configuration=Release
MSBUILD TopUI.sln /t:Clean /p:Configuration=UnicodeDebug
MSBUILD TopUI.sln /t:Clean /p:Configuration=UnicodeRelease

RD /S /Q  demo\Launcher\Debug
RD /S /Q  demo\Launcher\Release
RD /S /Q  demo\Launcher\Debug_u
RD /S /Q  demo\Launcher\Release_u
    
RD /S /Q  demo\TestApp1\Debug
RD /S /Q  demo\TestApp1\Release
RD /S /Q  demo\TestApp1\Debug_u
RD /S /Q  demo\TestApp1\Release_u

exit /b %ERRORLEVEL%
endlocal
pause