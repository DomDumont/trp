# Set project name
#project (Rototo-Samples)

#include_directories (${Rototo_INCLUDE_DIRS})
include_directories("../Sources")
include_directories("../Extern/SDL/include")
# Define dependency libs
set (INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (HelloWorld)

