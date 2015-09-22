# Define target name
set (TARGET_NAME HelloWorld)

file(GLOB_RECURSE HelloWorld_SOURCES "./*.cpp")

if (NOT DEFINED EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIR})
endif()

include_directories("../../Rototo/Sources")
include_directories("../../Rototo/Extern/stb")

add_executable (HelloWorld ${HelloWorld_SOURCES})
target_link_libraries(HelloWorld Rototo)

if (EMSCRIPTEN)
	# SET(CMAKE_EXECUTABLE_SUFFIX ".html")
	set_target_properties(HelloWorld PROPERTIES LINK_FLAGS "-s USE_SDL=2")
endif()


