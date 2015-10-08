file(GLOB_RECURSE Rototo_SOURCES "Sources/*.cpp")
file(GLOB_RECURSE Rototo_HEADERS "Sources/*.h" "Includes/*.h")




if (NOT DEFINED EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIR})
	message( "SDL2 include path found = " ${SDL2_INCLUDE_DIR})
endif()

include_directories("Extern/stb")
include_directories("Extern/PugiXml/src")
include_directories("Includes")
include_directories("Extern/AngelScript/sdk/angelscript/include")





add_library (Rototo ${Rototo_SOURCES} ${Rototo_HEADERS})






if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DTRP_WIN32)
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
target_link_libraries(Rototo angelscript)


add_subdirectory(Extern)

