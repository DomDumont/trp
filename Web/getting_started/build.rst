.. _trp-build:

=====
Build
=====


Emscripten
==========

* Install Emscripten
* On Windows install also MinGW 

 ::

	mkdir Build
	cd Build
	emconfigure cmake -G "MinGW Makefiles" ..
	cmake --build .


Windows 
=======

First you have to install SDL2 visual dev libraries.
and add an environment variable called "SDL2" to your system


`SDL2 <https://www.libsdl.org/download-2.0.php>`_

and SDL2-Net dev libraries for visual also

`SDL2_NET <https://www.libsdl.org/projects/SDL_net/>`_

Then, in order to build Ratata (the I.D.E.) you must install QT5

`QT5 <http://www.qt.io/download/>`_

if you don't use the regular install directory, you may need to change the cmake files;



 ::

	mkdir Build
	cd Build
	cmake -G "Visual Studio 12 2013 Win64" ..


If visual is the only ins available on your machine 
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


You can also clean

 ::

	cmake --build . --target clean

or install TRP

 ::

	cmake --build . --target install

XCode
=====
 ::

	mkdir Build
	cd Build
	cmake -G "XCode" ..
	cmake --build .
