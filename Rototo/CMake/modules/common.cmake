include(CMakeParseArguments)


# Macro for defining source files with optional arguments as follows:
#  GLOB_CPP_PATTERNS <list> - Use the provided globbing patterns for CPP_FILES instead of the default *.cpp
#  GLOB_H_PATTERNS <list> - Use the provided globbing patterns for H_FILES instead of the default *.h
#  EXCLUDE_PATTERNS <list> - Use the provided patterns for excluding matched source files
#  EXTRA_CPP_FILES <list> - Include the provided list of files into CPP_FILES result
#  EXTRA_H_FILES <list> - Include the provided list of files into H_FILES result
#  PCH <value> - Enable precompiled header support on the defined source files using the specified header file
#  PARENT_SCOPE - Glob source files in current directory but set the result in parent-scope's variable ${DIR}_CPP_FILES and ${DIR}_H_FILES instead
#  RECURSE - Option to glob recursively
#  GROUP - Option to group source files based on its relative path to the corresponding parent directory (only works when PARENT_SCOPE option is not in use)
macro (define_source_files)
    # Parse the arguments
    cmake_parse_arguments (ARG "PARENT_SCOPE;RECURSE;GROUP" "PCH" "EXTRA_CPP_FILES;EXTRA_H_FILES;GLOB_CPP_PATTERNS;GLOB_H_PATTERNS;EXCLUDE_PATTERNS" ${ARGN})

    # Source files are defined by globbing source files in current source directory and also by including the extra source files if provided
    if (NOT ARG_GLOB_CPP_PATTERNS)
        set (ARG_GLOB_CPP_PATTERNS *.cpp)    # Default glob pattern
    endif ()
    if (NOT ARG_GLOB_H_PATTERNS)
        set (ARG_GLOB_H_PATTERNS *.h)
    endif ()
    if (ARG_RECURSE)
        set (ARG_RECURSE _RECURSE)
    else ()
        unset (ARG_RECURSE)
    endif ()
    file (GLOB${ARG_RECURSE} CPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${ARG_GLOB_CPP_PATTERNS})
    file (GLOB${ARG_RECURSE} H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${ARG_GLOB_H_PATTERNS})
    if (ARG_EXCLUDE_PATTERNS)
        set (CPP_FILES_WITH_SENTINEL ";${CPP_FILES};")  # Stringify the lists
        set (H_FILES_WITH_SENTINEL ";${H_FILES};")
        foreach (PATTERN ${ARG_EXCLUDE_PATTERNS})
            foreach (LOOP RANGE 1)
                string (REGEX REPLACE ";${PATTERN};" ";;" CPP_FILES_WITH_SENTINEL "${CPP_FILES_WITH_SENTINEL}")
                string (REGEX REPLACE ";${PATTERN};" ";;" H_FILES_WITH_SENTINEL "${H_FILES_WITH_SENTINEL}")
            endforeach ()
        endforeach ()
        set (CPP_FILES ${CPP_FILES_WITH_SENTINEL})      # Convert strings back to lists, extra sentinels are harmless
        set (H_FILES ${H_FILES_WITH_SENTINEL})
    endif ()
    list (APPEND CPP_FILES ${ARG_EXTRA_CPP_FILES})
    list (APPEND H_FILES ${ARG_EXTRA_H_FILES})

    set (SOURCE_FILES ${CPP_FILES} ${H_FILES})

    message ("SOURCE_FILES = " ${SOURCE_FILES})

    # Optionally enable PCH
    if (ARG_PCH)
        enable_pch (${ARG_PCH})
    endif ()

    # Optionally accumulate source files at parent scope
    if (ARG_PARENT_SCOPE)
        get_filename_component (NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
        set (${NAME}_CPP_FILES ${CPP_FILES} PARENT_SCOPE)
        set (${NAME}_H_FILES ${H_FILES} PARENT_SCOPE)
    # Optionally put source files into further sub-group (only works when PARENT_SCOPE option is not in use)
    elseif (ARG_GROUP)
        foreach (CPP_FILE ${CPP_FILES})
            get_filename_component (PATH ${CPP_FILE} PATH)
            if (PATH)
                string (REPLACE / \\ PATH ${PATH})
                source_group ("Source Files\\${PATH}" FILES ${CPP_FILE})
            endif ()
        endforeach ()
        foreach (H_FILE ${H_FILES})
            get_filename_component (PATH ${H_FILE} PATH)
            if (PATH)
                string (REPLACE / \\ PATH ${PATH})
                source_group ("Header Files\\${PATH}" FILES ${H_FILE})
            endif ()
        endforeach ()
    endif ()
endmacro ()



macro (setup_library)
    add_library (${TARGET_NAME} ${ARGN} ${SOURCE_FILES})
    setup_target ()
endmacro()


macro (setup_target)
    # Include directories
    include_directories (${INCLUDE_DIRS})
    define_dependency_libs (${TARGET_NAME})
    # Link libraries
    message( "LIBS = " ${LIBS})
    message( "ABSOLUTE_PATH_LIBS = " ${ABSOLUTE_PATH_LIBS})

    target_link_libraries (${TARGET_NAME} ${ABSOLUTE_PATH_LIBS} ${LIBS})
endmacro()


macro (setup_main_executable)
    # Parse extra arguments
    cmake_parse_arguments (ARG "NOBUNDLE;MACOSX_BUNDLE;WIN32" "" "" ${ARGN})

    if (WIN32)
        message("EXE_TYPE set to win32")
        set (EXE_TYPE WIN32)
    endif ()
    list (APPEND TARGET_PROPERTIES DEBUG_POSTFIX _d)
    setup_executable (${EXE_TYPE} ${ARG_UNPARSED_ARGUMENTS})
endmacro()    


macro (setup_executable)

    define_dependency_libs (Rototo)
    add_executable (${TARGET_NAME} ${ARG_UNPARSED_ARGUMENTS} ${SOURCE_FILES})
    setup_target ()
endmacro()    


macro (define_dependency_libs TARGET)

        # This variable value can either be 'ROTOTO' target or an absolute path to an actual static/shared ROTOTO library or empty (if we are building the library itself)
        # The former would cause CMake not only to link against the ROTOTO library but also to add a dependency to ROTOTO target
        if (Rototo_LIBRARIES)
            message("Rototo_LIBRARIES is defined")
            if (WIN32 AND Rototo_LIBRARIES_DBG AND Rototo_LIBRARIES_REL AND TARGET ${TARGET_NAME})
                # Special handling when both debug and release libraries are found
                message("Special handling when both debug and release libraries are found")
                target_link_libraries (${TARGET_NAME} debug ${ROTOTO_LIBRARIES_DBG} optimized ${ROTOTO_LIBRARIES_REL})
            else ()
                message("Rototo_LIBRARIES" ${Rototo_LIBRARIES})
                list (APPEND ABSOLUTE_PATH_LIBS ${Rototo_LIBRARIES})
            endif ()
        endif ()

endmacro()        
