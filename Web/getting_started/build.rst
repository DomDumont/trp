.. _trp-build:

=====
Build
=====

Attention, c'est énorme si ça marche.

Emscripten
==========

* Install Emscripten
* On Windows install also MinGW

::
	mkdir Build
	cd Build
	cmake -G "MinGW Makefiles" ..
	cmake --build .


Visual
======

::
	mkdir Build
	cd Build
	cmake ..
	cmake --build .
