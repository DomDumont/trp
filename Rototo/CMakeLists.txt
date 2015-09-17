file(GLOB_RECURSE Rototo_SOURCES "Sources/*.cpp")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories("Extern/stb")
include_directories("Extern/PugiXml/src")
add_library (Rototo ${Rototo_SOURCES})


