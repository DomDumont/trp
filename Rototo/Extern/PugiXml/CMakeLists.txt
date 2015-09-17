# Define target name
set (TARGET_NAME PugiXml)

file(GLOB_RECURSE PugiXml_SOURCES "*.cpp")

add_library (PugiXml ${PugiXml_SOURCES})


