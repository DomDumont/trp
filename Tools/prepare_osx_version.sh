#
#
# Script to print user information who currently login , current date & time
#
clear
echo "Hello $USER"

#--------------------
# Build Documentation
#--------------------

pushd .
cd doc
./extract_api.sh
./build_doc.sh
popd

#--------------------
# Build Ratata
#--------------------
pushd .
cd ratata
make clean
make
popd

#--------------------
# Embed QT Librairies
#--------------------
/Developer/Tools/QT/macdeployqt ./ratata_distrib/bin/Ratata.app

#--------------------
# Build Rototo
#--------------------
pushd .
cd ../projects/XCode/Rototo
xcodebuild clean
xcodebuild -target Rototo -configuration Release | egrep -A 5 "(error|warning):"
popd

pushd .
rm -rf ./osx-version
mkdir -p ./osx-version
cd osx-version
mkdir -p ./doc
cd doc
mkdir -p ./en
mkdir -p ./fr
popd

rm -rf osx-version
mkdir osx-version

#--------------------
# Copy Documentation
#--------------------
cd osx-version
mkdir doc
cd doc
mkdir en
mkdir fr
cd ..
cd ..
rsync -r  ../doc/en/out/* ./osx-version/doc/en
# Too Soon french docs are not ready rsync -r  ../doc/fr/out/* ./osx-version/doc/fr
#--------------------
# Copy Ratata
#--------------------
rsync -r ./ratata_distrib/bin/Ratata.app ./osx-version/

#--------------------
# Copy Rototo
#--------------------
rsync -r ../projects/XCode/Rototo/Build/Release/Rototo.app ./osx-version/
#--------------------
# Copy GameData
#--------------------
rsync -r  ../data/gamedata-sample ./osx-version/
rsync -r  ../data/helpers ./osx-version/
rsync -r  ../data/themes ./osx-version/
rsync   ../data/settings.xml ./osx-version/
rsync   ../doc/en/README-OSX.md ./osx-version/
rsync   ../doc/fr/LISEZMOI-OSX.md ./osx-version/
rsync   ../doc/VERSION.md ./osx-version/

#--------------------
# DMG Creation
#--------------------
rm trp.dmg
#hdiutil create -format UDRW -srcfolder osx-version trp.dmg
hdiutil create -format UDBZ -srcfolder osx-version trp.dmg
