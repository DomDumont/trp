file(GLOB_RECURSE Rototo_SOURCES "Sources/*.cpp")
file(GLOB_RECURSE Rototo_HEADERS "Sources/*.h" "Includes/*.h")


option(ROTOTO_USE_AUDIO "Rototo has audio features " OFF)
option(ROTOTO_USE_BINDING "Rototo has audio features " ON)


if (NOT DEFINED EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIR})
	message( "SDL2 include path found = " ${SDL2_INCLUDE_DIR})
endif()
include_directories("Extern/stb")
include_directories("Extern/PugiXml/src")
include_directories("Includes")

if (ROTOTO_USE_BINDING)
	include_directories("Extern/AngelScript/sdk/angelscript/include")
endif()




add_library (Rototo ${Rototo_SOURCES} ${Rototo_HEADERS})


if (ROTOTO_USE_AUDIO)
	add_definitions(-DTRP_USE_AUDIO)	
endif()

if (ROTOTO_USE_BINDING)
	add_definitions(-DTRP_USE_BINDING)	
endif()




if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

if (UNIX)
	add_definitions(-DTRP_LINUX)
endif(UNIX)

if (EMSCRIPTEN)
	add_definitions(-DTRP_EMSCRIPTEN)
	add_definitions( -s USE_SDL=2 -Wno-warn-absolute-paths -Wno-format-security )
endif()

target_link_libraries(Rototo PugiXml)
target_link_libraries(Rototo ${SDL2_LIBRARY})
if (ROTOTO_USE_BINDING)
	target_link_libraries(Rototo angelscript)
endif()

add_subdirectory(Extern)

