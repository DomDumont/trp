.. _trp-build:

=====
Build
=====


Emscripten
==========

* Install Emscripten
* On Windows install also MinGW ::


	mkdir Build
	cd Build
	emconfigure cmake -G "MinGW Makefiles" ..
	cmake --build .


Visual
====== 

If visual is the only generator available on your machine 
use a simple cmake otherwise you may have to specify the generator
with the -G option .
In order to build a 64 bits version, it's also done through the generator.
On windows cmake have difficulties to find sdl2 so please add a SDL2 environment
variable on your system to indicate cmake where your sdl2 library is installed
 
 ::


	mkdir Build
	cd Build
	cmake ..
	cmake --build .

XCode
=====
 ::

	mkdir Build
	cd Build
	cmake -G "XCode" ..
	cmake --build .
