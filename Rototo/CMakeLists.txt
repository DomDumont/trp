file(GLOB_RECURSE Rototo_SOURCES "Sources/*.cpp")




find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories("Extern/stb")
include_directories("Extern/PugiXml/src")




add_library (Rototo ${Rototo_SOURCES})


if (WIN32)
	set_property(TARGET Rototo PROPERTY COMPILE_DEFINITIONS_DEBUG _CRT_SECURE_NO_WARNINGS )
	set_property(TARGET Rototo PROPERTY COMPILE_DEFINITIONS_RELEASE _CRT_SECURE_NO_WARNINGS) 
endif(WIN32)


add_subdirectory(Extern)

