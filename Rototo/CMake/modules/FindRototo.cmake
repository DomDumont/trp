if (Rototo_FOUND)
    # All the subprojects should use the same Rototo library, so only need to search on the first (sub)project that requires Rototo library
    return ()
endif ()


if (NOT Rototo_LIB_TYPE STREQUAL Rototo_FOUND_LIB_TYPE)
    unset (Rototo_LIBRARIES CACHE)
    set (Rototo_FOUND_LIB_TYPE ${Rototo_LIB_TYPE} CACHE INTERNAL "Lib type when Rototo library was last found")

    # Rototo prefers static library type by default while CMake prefers shared one, so we need to change CMake preference to agree with Rototo
    if (NOT Rototo_LIB_TYPE STREQUAL SHARED)
        list (REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
    endif ()
endif ()

set (PATH_SUFFIX Rototo)
message ("Coucou " ${CMAKE_PROJECT_NAME} ) 
if (CMAKE_PROJECT_NAME STREQUAL Rototo AND TARGET Rototo)
    # Library location is already known to be in the build tree
    set (Rototo_HOME ${CMAKE_BINARY_DIR})
    set (Rototo_INCLUDE_DIRS ${Rototo_HOME}/include ${Rototo_HOME}/include/${PATH_SUFFIX}/ThirdParty)
    set (Rototo_LIBRARIES Rototo)
    set (FOUND_MESSAGE "Found Rototo: as CMake target")
else ()
    # Library location would be searched (based on Rototo_HOME variable if provided and in system-wide default location)
    if (NOT Rototo_HOME AND DEFINED ENV{Rototo_HOME})
        file (TO_CMAKE_PATH "$ENV{Rototo_HOME}" Rototo_HOME)
    endif ()
    # Rototo_HOME variable should be an absolute path, so use a non-rooted search even when we are cross-compiling
    if (Rototo_HOME)
        list (APPEND CMAKE_PREFIX_PATH ${Rototo_HOME})
        set (SEARCH_OPT NO_CMAKE_FIND_ROOT_PATH)
    endif ()

    if (NOT Rototo_64BIT)
        # For 32-bit, force to search in 'lib' path even when the host system defaulted to use 'lib64'
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
    elseif (WIN32)
        # For 64-bit, force to search in 'lib64' path even when the Windows platform is not defaulted to use it
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
    endif ()
    find_path (Rototo_INCLUDE_DIRS Rototo.h PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
    find_library (Rototo_LIBRARIES NAMES Rototo ${Rototo_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
    if (WIN32)
        # For Windows platform, give a second chance to search for a debug version of the library
        find_library (Rototo_LIBRARIES_DBG NAMES Rototo_d ${Rototo_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
        # If both the non-debug and debug version of the libraries are found then use them both
        set (Rototo_LIBRARIES_REL ${Rototo_LIBRARIES})
        # Otherwise, Rototo_LIBRARIES variable should have the path to either one of the version
        if (Rototo_LIBRARIES)
            if (Rototo_LIBRARIES_DBG)
                list (APPEND Rototo_LIBRARIES ${Rototo_LIBRARIES_DBG})
            endif ()
        else ()
            set (Rototo_LIBRARIES ${Rototo_LIBRARIES_DBG})
        endif ()
    endif ()
    if (Rototo_INCLUDE_DIRS)
        set (BASE_INCLUDE_DIR ${Rototo_INCLUDE_DIRS})   # Preserve the base include dir because the original variable will be turned into a list below
        get_filename_component (PATH ${Rototo_INCLUDE_DIRS} PATH)
        set (Rototo_INCLUDE_DIRS ${PATH} ${Rototo_INCLUDE_DIRS}/ThirdParty)
        if (NOT Rototo_HOME)
            # Rototo_HOME is not set when using SDK installed on system-wide default location, so set it now
            get_filename_component (PATH ${PATH} PATH)
            set (Rototo_HOME ${PATH})
        endif ()
    endif ()
endif ()

if (Rototo_INCLUDE_DIRS AND Rototo_LIBRARIES)
    set (Rototo_FOUND 1)
    if (NOT FOUND_MESSAGE)
        set (FOUND_MESSAGE "Found Rototo: ${Rototo_LIBRARIES}")
    endif ()
    include (FindPackageMessage)
    FIND_PACKAGE_MESSAGE (Rototo ${FOUND_MESSAGE} "[${Rototo_LIBRARIES}][${Rototo_INCLUDE_DIRS}]")
    set (Rototo_HOME ${Rototo_HOME} CACHE PATH "Path to Rototo build tree or SDK installation location (external project only)" FORCE)
elseif (Rototo_FIND_REQUIRED)
    if (ANDROID)
        set (NOT_FOUND_MESSAGE "For Android platform, double check if you have specified to use the same ANDROID_ABI as the Rototo library in the build tree or SDK.")
    endif ()
    message (FATAL_ERROR
        "Could not find Rototo library in Rototo build tree or SDK installation. "
        "Use Rototo_HOME environment variable or build option to specify the location of the build tree or SDK installation. ${NOT_FOUND_MESSAGE}")
endif ()

mark_as_advanced (Rototo_INCLUDE_DIRS Rototo_LIBRARIES Rototo_LIBRARIES_REL Rototo_LIBRARIES_DBG URHO3D_HOME)