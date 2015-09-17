project (TRP)

cmake_minimum_required (VERSION 2.8.6)


set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

IF(WIN32)
	MESSAGE("Coucou win32")
	SET(ENV{SDL2} ${CMAKE_SOURCE_DIR}/Rototo/Extern/SDL2)
ENDIF(WIN32)

add_subdirectory (Rototo)
