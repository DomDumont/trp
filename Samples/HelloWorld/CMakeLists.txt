# Define target name
set (TARGET_NAME HelloWorld)

option(BUILD_HTML "Build HTML instead of JS " OFF)

file(GLOB_RECURSE HelloWorld_SOURCES "./*.cpp")

if (NOT DEFINED EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIR})
endif()

include_directories("../../Rototo/Includes")
include_directories("../../Rototo/Extern/AngelScript/sdk/angelscript/include")


add_executable (HelloWorld ${HelloWorld_SOURCES})
target_link_libraries(HelloWorld Rototo)

if (EMSCRIPTEN)
	if (BUILD_HTML)
		SET(CMAKE_EXECUTABLE_SUFFIX ".html")
		message("Build html instead of js")
	else()
		message("Build js only")
	endif()
	add_definitions(-s USE_SDL=2 -s ASSERTIONS=2 -Wno-warn-absolute-paths -Wno-format-security )	
	set_target_properties(HelloWorld PROPERTIES LINK_FLAGS "-s USE_SDL=2  -s DEMANGLE_SUPPORT=1 ")
endif()


