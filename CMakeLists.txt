project (TRP)

cmake_minimum_required (VERSION 2.8.6)

IF(NOT WIN32)
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
	        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
endif()


set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

IF(WIN32)
	MESSAGE("Coucou win32")
	SET(ENV{SDL2} ${CMAKE_SOURCE_DIR}/Rototo/Extern/SDL2)
	SET(ENV{SDL2NET} ${CMAKE_SOURCE_DIR}/Rototo/Extern/SDL2_net)
	SET(ENV{SDL2MIXER} ${CMAKE_SOURCE_DIR}/Rototo/Extern/SDL2_mixer)
ENDIF(WIN32)

add_subdirectory (Rototo)
add_subdirectory (Samples)
add_subdirectory (Docs)
add_subdirectory (Tools)

INSTALL(DIRECTORY Data DESTINATION .)
SET(CPACK_GENERATOR "ZIP")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dominique Dumont") #required
 
INCLUDE(CPack)
