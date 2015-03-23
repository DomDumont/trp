cd ../projects/android
call ndk-build.cmd clean
call ndk-build.cmd
call ant clean release
call ant release
cd ../../tools
pause
