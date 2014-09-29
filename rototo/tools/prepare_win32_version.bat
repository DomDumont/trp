call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
REM ----------------------------------------------------------------------------
REM  Build Documentation
REM ----------------------------------------------------------------------------
REM activate this line to skip compilation goto tralala

PUSHD .\doc
call extract_api.bat
call build_doc.bat
POPD

REM ----------------------------------------------------------------------------
REM  Build Ratata
REM ----------------------------------------------------------------------------

PUSHD .\ratata
"C:\Qt\4.8.6\bin\qmake.exe" .\ratata.pro -r -spec win32-msvc2010 CONFIG+=release
nmake clean -f .\Makefile.Release
nmake  -f .\Makefile.Release
POPD

pause

REM ----------------------------------------------------------------------------
REM  Build Rototo
REM ----------------------------------------------------------------------------

call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
msbuild ..\projects\Visual2010\Rototo.sln /property:Configuration=Release /t:Clean;Rebuild


:tralala

echo delete old version directory
rmdir win32-version /S /Q
mkdir win32-version

REM --------------------
REM  Copy Documentation
REM --------------------

robocopy  ../doc/en/out ./win32-version/doc/en  /e
REM Too soon, french docs are not ready robocopy  ../doc/fr/out ./win32-version/doc/fr  /e

REM --------------------
REM  Copy Ratata
REM --------------------
robocopy  ./ratata_distrib/bin ./win32-version/ ratata.exe
robocopy   C:/Qt/4.8.6/bin   ./win32-version/ QtNetwork4.dll
robocopy   C:/Qt/4.8.6/bin   ./win32-version/ QtCore4.dll
robocopy   C:/Qt/4.8.6/bin   ./win32-version/ QtGui4.dll
robocopy   C:/Qt/4.8.6/bin   ./win32-version/ QtXml4.dll

REM --------------------
REM  Copy Rototo
REM --------------------

robocopy ../bin/win32/Release ./win32-version /e

#--------------------
# Copy GameData
#--------------------
robocopy   ../data/gamedata-sample ./win32-version/gamedata-sample /e
robocopy   ../data/helpers ./win32-version/helpers /e
robocopy   ../data/themes ./win32-version/themes /e
robocopy   ../data ./win32-version settings.xml
robocopy   ../doc/en ./win32-version README-WIN32.md
robocopy   ../doc/fr ./win32-version LISEZMOI-WIN32.md 
robocopy   ../doc ./win32-version VERSION.md 

rem robocopy ../data/gamedata-sample ./win32-version/data/gamedata-sample /e
rem robocopy ../doc/out ./win32-version/docs /e
rem robocopy ../tools/ratata-distrib ./win32-version/tools /e

rem robocopy ../Rototo/source/doc ./trp settings.xml
rem robocopy ../Rototo/source/doc ./trp version.txt
rem robocopy ../Rototo/rototo/helpers ./trp/helpers trp_api.rsc
rem robocopy ../Rototo/rototo/helpers ./trp/helpers scene_manager.rsc
rem robocopy ../Rototo/rototo/themes ./trp/themes /e


del  trp-win32.zip
cd win32-version
"../7za" a -tzip  ../trp-win32.zip .
cd ..

"C:\Program Files (x86)\NSIS\makensis.exe" trp.nsi



