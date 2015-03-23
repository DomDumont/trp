# Define target name
set (TARGET_NAME HelloWorld)

find_package (Rototo REQUIRED)

define_source_files()
# Setup target with resource copying
setup_main_executable ()

