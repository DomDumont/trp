#!/usr/bin/python

import shutil

out_release = '..\\bin\\win32\\release'
out_debug = '..\\bin\\win32\\debug\\'
def main():
    print ('Copy DLL ...')
    try:
        shutil.copy('../extern/SDL2-2.0.3/VisualC/SDL/Win32/Release/SDL2.dll', out_release)
        shutil.copy('../extern/SDL2-2.0.3/VisualC/SDL/Win32/Debug/SDL2.dll', out_debug)

        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/Release/SDL2_mixer.dll', out_release)
        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/Debug/SDL2_mixer.dll', out_debug)

        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libFLAC-8.dll', out_release)
        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libFLAC-8.dll', out_debug)

        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libmodplug-1.dll', out_release)
        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libmodplug-1.dll', out_debug)

        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libogg-0.dll', out_release)
        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libogg-0.dll', out_debug)

        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libvorbis-0.dll', out_release)
        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libvorbis-0.dll', out_debug)

        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libvorbisfile-3.dll', out_release)
        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/libvorbisfile-3.dll', out_debug)

        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/smpeg2.dll', out_release)
        shutil.copy('../extern/SDL2_mixer-2.0.0/VisualC/external/lib/x86/smpeg2.dll', out_debug)


        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/Release/SDL2_image.dll', out_release)
        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/Debug/SDL2_image.dll', out_debug)

        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/external/lib/x86/libjpeg-9.dll', out_release)
        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/external/lib/x86/libjpeg-9.dll', out_debug)

        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/external/lib/x86/libpng16-16.dll', out_release)
        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/external/lib/x86/libpng16-16.dll', out_debug)

        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/external/lib/x86/libtiff-5.dll', out_release)
        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/external/lib/x86/libtiff-5.dll', out_debug)

        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/external/lib/x86/libwebp-4.dll', out_release)
        shutil.copy('../extern/SDL2_image-2.0.0/VisualC/external/lib/x86/libwebp-4.dll', out_debug)


        shutil.copy('../extern/SDL2_ttf-2.0.12/VisualC/Win32/Release/SDL2_ttf.dll', out_release)
        shutil.copy('../extern/SDL2_ttf-2.0.12/VisualC/Win32/Debug/SDL2_ttf.dll', out_debug)

        shutil.copy('../extern/SDL2_ttf-2.0.12/VisualC/external/lib/x86/libfreetype-6.dll', out_release)
        shutil.copy('../extern/SDL2_ttf-2.0.12/VisualC/external/lib/x86/libfreetype-6.dll', out_debug)

        shutil.copy('../extern/SDL2_ttf-2.0.12/VisualC/external/lib/x86/zlib1.dll', out_release)
        shutil.copy('../extern/SDL2_ttf-2.0.12/VisualC/external/lib/x86/zlib1.dll', out_debug)

        shutil.copy('../extern/SDL2_net-2.0.0/VisualC/Release/SDL2_net.dll', out_release)
        shutil.copy('../extern/SDL2_net-2.0.0/VisualC/Debug/SDL2_net.dll', out_debug)

        shutil.copy('../extern/SDL2_gfx-1.0.0/Win32/Release/SDL2_gfx.dll', out_release)
        shutil.copy('../extern/SDL2_gfx-1.0.0/Win32/Debug/SDL2_gfx.dll', out_debug)
        
        print ('Done !')

    except Exception,err:
        print err

if __name__ == '__main__':
  main()
