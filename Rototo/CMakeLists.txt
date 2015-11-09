file(GLOB_RECURSE Rototo_SOURCES "Sources/*.cpp")
file(GLOB_RECURSE Rototo_HEADERS "Sources/*.h" "Includes/*.h")

option(ROTOTO_USE_AUDIO "Rototo has audio features " ON)
option(ROTOTO_USE_BINDING "Rototo has binding features " ON)
option(ROTOTO_USE_NETWORK "Rototo has network features " ON)
option(ROTOTO_USE_WATCH "Rototo has watch features " ON)


if (NOT DEFINED EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIR})
	message( "SDL2 include path found = " ${SDL2_INCLUDE_DIR})

	find_package(SDL2_NET REQUIRED)
	include_directories(${SDL2_NET_INCLUDE_DIR})
	message( "SDL2 NET include path found = " ${SDL2_NET_INCLUDE_DIR})

	find_package(SDL2_MIXER REQUIRED)
	include_directories(${SDL2_MIXER_INCLUDE_DIR})
	message( "SDL2 MIXER include path found = " ${SDL2_MIXER_INCLUDE_DIR})

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

if (ROTOTO_USE_NETWORK)
    SET(TRP_USE_NETWORK 1)
    add_definitions(-DTRP_USE_NETWORK)
endif()

if (ROTOTO_USE_WATCH)
	include_directories("Extern/EFSW/include")
    SET(TRP_USE_WATCH 1)
    add_definitions(-DTRP_USE_WATCH)
endif()


add_library (Rototo ${Rototo_SOURCES} ${Rototo_HEADERS})



INSTALL(FILES
    "$ENV{SDL2}/lib/x64/SDL2.dll"
	"$ENV{SDL2NET}/lib/x64/SDL2_net.dll"
    DESTINATION bin
    )



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
	add_definitions( -s USE_SDL=2 -O2 -Wno-warn-absolute-paths -Wno-format-security )
endif()

target_link_libraries(Rototo PugiXml)
target_link_libraries(Rototo ${SDL2_LIBRARY})
target_link_libraries(Rototo ${SDL2_NET_LIBRARY})
target_link_libraries(Rototo ${SDL2_MIXER_LIBRARY})
target_link_libraries(Rototo angelscript)
target_link_libraries(Rototo efsw)


add_subdirectory(Extern)

configure_file(Sources/Config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Includes/Config.h)
