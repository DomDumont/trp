# - Locate SDL2_net library
# This module defines:
#  SDL2_NET_LIBRARIES, the name of the library to link against
#  SDL2_NET_INCLUDE_DIRS, where to find the headers
#  SDL2_NET_FOUND, if false, do not try to link against
#  SDL2_NET_VERSION_STRING - human-readable string containing the version of SDL2_net
#
# $SDL2 is an environment variable that would
# correspond to the ./configure --prefix=$SDL2
# used in building SDL2.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake
# module, but with modifications to recognize OS X frameworks and
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_path(SDL2_NET_INCLUDE_DIR SDL_net.h
  HINTS
    ENV SDL2NET
    ENV SDL2
  	PATH_SUFFIXES include/SDL2 include
	i686-w64-mingw32/include/SDL2
	x86_64-w64-mingw32/include/SDL2  	
	PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/include/SDL2
	/usr/include/SDL2
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt  
)

# Lookup the 64 bit libs on x64
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
find_library(SDL2_NET_LIBRARY
  NAMES SDL2_net
  HINTS
    ENV SDL2NET
    ENV SDL2
  	PATH_SUFFIXES lib64 lib
	lib/x64
	x86_64-w64-mingw32/lib
	PATHS
	/sw
	/opt/local
	/opt/csw
	/opt	
)
# On 32bit build find the 32bit libs
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
find_library(SDL2_NET_LIBRARY
  NAMES SDL2_net
  HINTS
    ENV SDL2NET
    ENV SDL2
  	PATH_SUFFIXES lib64 lib
	lib/x86
	i686-w64-mingw32/lib
	PATHS
		/sw
		/opt/local
		/opt/csw
		/opt	
)	
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
if(SDL2_NET_INCLUDE_DIR AND EXISTS "${SDL2_NET_INCLUDE_DIR}/SDL_net.h")
  file(STRINGS "${SDL2_NET_INCLUDE_DIR}/SDL_net.h" SDL2_NET_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_NET_MAJOR_VERSION[ \t]+[0-9]+$")
  file(STRINGS "${SDL2_NET_INCLUDE_DIR}/SDL_net.h" SDL2_NET_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_NET_MINOR_VERSION[ \t]+[0-9]+$")
  file(STRINGS "${SDL2_NET_INCLUDE_DIR}/SDL_net.h" SDL2_NET_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_NET_PATCHLEVEL[ \t]+[0-9]+$")
  string(REGEX REPLACE "^#define[ \t]+SDL_NET_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_NET_VERSION_MAJOR "${SDL2_NET_VERSION_MAJOR_LINE}")
  string(REGEX REPLACE "^#define[ \t]+SDL_NET_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_NET_VERSION_MINOR "${SDL2_NET_VERSION_MINOR_LINE}")
  string(REGEX REPLACE "^#define[ \t]+SDL_NET_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_NET_VERSION_PATCH "${SDL2_NET_VERSION_PATCH_LINE}")
  set(SDL2_NET_VERSION_STRING ${SDL2_NET_VERSION_MAJOR}.${SDL2_NET_VERSION_MINOR}.${SDL2_NET_VERSION_PATCH})
  unset(SDL2_NET_VERSION_MAJOR_LINE)
  unset(SDL2_NET_VERSION_MINOR_LINE)
  unset(SDL2_NET_VERSION_PATCH_LINE)
  unset(SDL2_NET_VERSION_MAJOR)
  unset(SDL2_NET_VERSION_MINOR)
  unset(SDL2_NET_VERSION_PATCH)
endif()

set(SDL2_NET_LIBRARIES ${SDL2_NET_LIBRARY})
set(SDL2_NET_INCLUDE_DIRS ${SDL2_NET_INCLUDE_DIR})

#include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
#
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_net
                                  REQUIRED_VARS SDL2_NET_LIBRARIES SDL2_NET_INCLUDE_DIRS
                                  VERSION_VAR SDL2_NET_VERSION_STRING)

mark_as_advanced(SDL_NET_LIBRARY SDL_NET_INCLUDE_DIR)
