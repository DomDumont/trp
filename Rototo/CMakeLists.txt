file(GLOB_RECURSE Rototo_SOURCES "Sources/*.cpp")
file(GLOB_RECURSE Rototo_HEADERS "Sources/*.h" "Includes/*.h")

option(ROTOTO_USE_AUDIO "Rototo has audio features " OFF)
option(ROTOTO_USE_BINDING "Rototo has binding features " ON)


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
    SET(TRP_USE_BINDING 1)
    add_definitions(-DTRP_USE_BINDING)
endif()

if (ROTOTO_USE_AUDIO)
    SET(TRP_USE_AUDIO 1)
    add_definitions(-DTRP_USE_AUDIO)
endif()


add_library (Rototo ${Rototo_SOURCES} ${Rototo_HEADERS})






if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	set(TRP_WIN32 1)
	add_definitions(-DTRP_WIN32)
endif(WIN32)

if (UNIX)
	set(TRP_LINUX 1)
	add_definitions(-DTRP_LINUX)
endif(UNIX)

if (EMSCRIPTEN)
	set(TRP_EMSCRIPTEN 1)
	add_definitions(-DTRP_EMSCRIPTEN)
	add_definitions( -s USE_SDL=2 -s ASSERTIONS=2 -Wno-warn-absolute-paths -Wno-format-security )
endif()

target_link_libraries(Rototo PugiXml)
target_link_libraries(Rototo ${SDL2_LIBRARY})
target_link_libraries(Rototo angelscript)


add_subdirectory(Extern)

configure_file(Sources/Config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Includes/Config.h)
