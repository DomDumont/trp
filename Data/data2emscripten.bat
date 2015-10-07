python D:\Tools\Emscripten\emscripten\tag-1.34.11\tools\file_packager.py trp.data --preload settings.xml helpers themes gamedata-sample --js-output=trp.js
copy trp.js ..\Build\Samples\HelloWorld\trp.js
copy trp.data ..\Build\Samples\HelloWorld\trp.data
pause
