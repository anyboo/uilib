@echo off
setlocal
IF /I "%1"=="" goto usage
@call "%VS120COMNTOOLS%vsvars32.bat

SET PROJECT=%1
SET CONFIGURATION=%2
echo %PROJECT%

IF /I "%PROJECT%"=="all" (
	SET PROJECT=Rebuild
) ELSE (
    SET PROJECT=%PROJECT%:rebuild
)

goto msbuild
pause

:msbuild
MSBUILD TopUI.sln /t:%PROJECT% /p:Configuration=Debug
MSBUILD TopUI.sln /t:%PROJECT% /p:Configuration=Release
MSBUILD TopUI.sln /t:%PROJECT% /p:Configuration=UnicodeDebug
MSBUILD TopUI.sln /t:%PROJECT% /p:Configuration=UnicodeRelease
endlocal
exit /b %ERRORLEVEL%


:usage
echo Usage:
echo ------
echo build [PROJECT]
echo PROJECT: ".vcxproj|all"
echo.
echo Default is build all.
endlocal