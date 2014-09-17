
You must create relative symbolic Links like this one : 

mklink /D SDL2-2.0.3 ..\..\..\extern\SDL2-2.0.3
mklink /D SDL2_mixer-2.0.0 ..\..\..\extern\SDL2_mixer-2.0.0
mklink /D SDL2_image-2.0.0 ..\..\..\extern\SDL2_image-2.0.0
mklink /D SDL2_ttf-2.0.12 ..\..\..\extern\SDL2_ttf-2.0.12
mklink /D SDL2_net-2.0.0 ..\..\..\extern\SDL2_net-2.0.0
mklink /D SDL2_gfx-1.0.0 ..\..\..\extern\SDL2_gfx-1.0.0
mklink /D AngelScript-2.29.0 ..\..\..\extern\AngelScript-2.29.0
mklink /D Box2D-2.3.1 ..\..\..\extern\Box2D-2.3.1
mklink /D tinyxml2 ..\..\..\extern\tinyxml2

or

ln -s ../../../extern/SDL2-2.0.3 SDL2-2.0.3
ln -s ../../../extern/SDL2_mixer-2.0.0 SDL2_mixer-2.0.0
ln -s ../../../extern/SDL2_image-2.0.0 SDL2_image-2.0.0
ln -s ../../../extern/SDL2_ttf-2.0.12 SDL2_ttf-2.0.12
ln -s ../../../extern/AngelScript-2.29.0 AngelScript-2.29.0
ln -s ../../../extern/SDL2_net-2.0.0 SDL2_net-2.0.0
ln -s ../../../extern/Box2D-2.3.1 Box2D-2.3.1
ln -s ../../../extern/tinyxml2 tinyxml2
ln -s ../../../extern/SDL2_gfx-1.0.0 SDL2_gfx-1.0.0



