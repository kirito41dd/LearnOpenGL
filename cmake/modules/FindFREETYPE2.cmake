# Locate the freetype2 library
#
# This module defines the following variables:
#
# FREETYPE2_LIBRARY the name of the library;
# FREETYPE2_INCLUDE_DIR where to find glfw include files.
# FREETYPE2_FOUND true if both the FREETYPE2_LIBRARY and FREETYPE2_INCLUDE_DIR have been found.
#


set( _freetype2_HEADER_SEARCH_DIRS
"/usr/include/freetype2"
"/usr/local/include/freetype2"
"${CMAKE_SOURCE_DIR}/includes/freetype2"
"C:/Program Files (x86)/freetype/include/freetype2" )
set( _freetype2_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"${CMAKE_SOURCE_DIR}/lib"
"C:/Program Files (x86)/freetype/lib")


# Search for the header
FIND_PATH(FREETYPE2_INCLUDE_DIR "ft2build.h"
PATHS ${_freetype2_HEADER_SEARCH_DIRS} )

# Search for the library
FIND_LIBRARY(FREETYPE2_LIBRARY NAMES freetype
PATHS ${_freetype2_LIB_SEARCH_DIRS} )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREETYPE2 DEFAULT_MSG
FREETYPE2_LIBRARY FREETYPE2_INCLUDE_DIR)


IF(FREETYPE2_FOUND)
	IF(NOT FREETYPE2_FIND_QUIETLY)
	MESSAGE(STATUS "Found FREETYPE2: ${FREETYPE2_LIBRARY}")
	ENDIF(NOT FREETYPE2_FIND_QUIETLY)
ELSE(FREETYPE2_FOUND)
	IF(FREETYPE2_FIND_REQUIRED)
	MESSAGE(FATAL_ERROR "Could not find FREETYPE2")
	ENDIF(FREETYPE2_FIND_REQUIRED)
ENDIF(FREETYPE2_FOUND)