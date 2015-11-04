# Define target name
set (TARGET_NAME HelloWorld)

option(BUILD_HTML "Build HTML instead of JS " OFF)

file(GLOB_RECURSE HelloWorld_SOURCES "./*.cpp")


include_directories("../../Rototo/Includes")


add_executable (HelloWorld ${HelloWorld_SOURCES})
target_link_libraries(HelloWorld Rototo)
install (TARGETS HelloWorld DESTINATION bin )

if (EMSCRIPTEN)
	if (BUILD_HTML)
		SET(CMAKE_EXECUTABLE_SUFFIX ".html")
		message("Build html instead of js")
	else()
		message("Build js only")
	endif()
	add_definitions(-s USE_SDL=2  -O1 -Wno-warn-absolute-paths -Wno-format-security )	
	set_target_properties(HelloWorld PROPERTIES LINK_FLAGS "-s USE_SDL=2  -s DEMANGLE_SUPPORT=1 -O1")
endif()


