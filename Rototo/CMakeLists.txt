file(GLOB_RECURSE Rototo_SOURCES "Sources/*.cpp")



if (NOT DEFINED EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIR})
endif()
include_directories("Extern/stb")
include_directories("Extern/PugiXml/src")




add_library (Rototo ${Rototo_SOURCES})


if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

if (UNIX)
	add_definitions(-DTRP_LINUX)
endif(UNIX)

if (EMSCRIPTEN)
	add_definitions(-s USE_SDL=2 -Wno-warn-absolute-paths -Wno-format-security )
endif()

target_link_libraries(Rototo PugiXml)
target_link_libraries(Rototo ${SDL2_LIBRARY})

add_subdirectory(Extern)

