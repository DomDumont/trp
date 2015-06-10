project (Rototo)

cmake_minimum_required (VERSION 2.8.6)

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
include (common)

set (DEST_RUNTIME_DIR bin)


add_subdirectory (Sources)
add_subdirectory (extern)
add_subdirectory (Samples)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "T.R.P.")
SET(CPACK_PACKAGE_VENDOR "DomDom")


include(CPack)

