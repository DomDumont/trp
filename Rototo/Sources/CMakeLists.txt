set (TARGET_NAME Rototo)



# Define source files
define_source_files (GLOB_CPP_PATTERNS *.cpp GLOB_H_PATTERNS *.h)

include_directories("../extern/SDL/include")
include_directories("../extern/stb")
include_directories("../extern/PugiXml/src")

# Setup target
setup_library ()

install(TARGETS Rototo DESTINATION ${DEST_RUNTIME_DIR})