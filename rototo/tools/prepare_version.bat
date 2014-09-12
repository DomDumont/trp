call "c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
msbuild ..\projects\Visual2010\Rototo.sln /property:Configuration=Release

cd .\doc
python build_doc.py
cd ..

echo delete old version directory
rmdir versions /S /Q
mkdir versions

pause 

robocopy ../bin/win32/Release ./versions /e
robocopy ../data/gamedata-sample ./versions/data/gamedata-sample /e
robocopy ../doc/out ./versions/docs /e
robocopy ../tools/ratata-distrib ./versions/tools /e

rem robocopy ../Rototo/source/doc ./trp settings.xml
rem robocopy ../Rototo/source/doc ./trp version.txt
rem robocopy ../Rototo/rototo/helpers ./trp/helpers trp_api.rsc
rem robocopy ../Rototo/rototo/helpers ./trp/helpers scene_manager.rsc
rem robocopy ../Rototo/rototo/themes ./trp/themes /e


del  trp.zip
7za a -tzip  trp.zip ./versions
rem "C:\Program Files (x86)\NSIS\makensis.exe" trp.nsi


pause

