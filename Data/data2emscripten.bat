python D:\Tools\Emscripten\emscripten\tag-1.34.11\tools\file_packager.py trp.data --preload settings.xml helpers themes gamedata-naked gamedata-sample --js-output=trp.js
copy trp.js ..\BuildEmscripten\Samples\HelloWorld\trp.js
copy trp.data ..\BuildEmscripten\Samples\HelloWorld\trp.data
pause
