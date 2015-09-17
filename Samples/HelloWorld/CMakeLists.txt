# Define target name
set (TARGET_NAME HelloWorld)

file(GLOB_RECURSE HelloWorld_SOURCES "./*.cpp")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories("../../Rototo/Sources")
add_executable (HelloWorld ${HelloWorld_SOURCES})
target_link_libraries(HelloWorld Rototo)
target_link_libraries(HelloWorld ${SDL2_LIBRARY})



