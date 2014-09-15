REM ----------------------------------------------------------------------------
REM  Build Documentation
REM ----------------------------------------------------------------------------

PUSHD .\doc
call extract_api.bat
call build_doc.bat
POPD
pause

REM ----------------------------------------------------------------------------
REM  Build Rototo
REM ----------------------------------------------------------------------------

call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
msbuild ..\projects\Visual2010\Rototo.sln /property:Configuration=Release /t:Clean;Rebuild

pause

echo delete old version directory
rmdir win32-version /S /Q
mkdir win32-version

pause 

robocopy ../bin/win32/Release ./win32-version /e
robocopy ../data/gamedata-sample ./win32-version/data/gamedata-sample /e
robocopy ../doc/out ./win32-version/docs /e
robocopy ../tools/ratata-distrib ./win32-version/tools /e

rem robocopy ../Rototo/source/doc ./trp settings.xml
rem robocopy ../Rototo/source/doc ./trp version.txt
rem robocopy ../Rototo/rototo/helpers ./trp/helpers trp_api.rsc
rem robocopy ../Rototo/rototo/helpers ./trp/helpers scene_manager.rsc
rem robocopy ../Rototo/rototo/themes ./trp/themes /e


del  trp.zip
7za a -tzip  trp.zip ./win32-version
rem "C:\Program Files (x86)\NSIS\makensis.exe" trp.nsi


pause

