# 
# Copyright (C) 2020 Jordan Hendl
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

# 
# File:   CMakeLists.h
# Author: Jordan Hendl
# 
# Created on January 2nd, 2020, 11:30 PM
# 

CMAKE_MINIMUM_REQUIRED( VERSION 3.8.0 )

LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )


# Build options
OPTION( WERROR        "Whether or not warnings are treated as errors." ON  )
OPTION( BUILD_DOCS    "Whether or not to build doxygen documentation"  ON  )
OPTION( BUILD_TESTS   "Whether or not tests should be built. "         ON  )
OPTION( RUN_TESTS     "Whether or not tests should be run."            ON  )
OPTION( BUILD_RELEASE "Whether or not the to build for release.     "  OFF )

PROJECT( Iris CXX )
INCLUDE( Message   )
INCLUDE( BuildTest )

# Print build options
MESSAGE( STATUS "" ) 
MESSAGE( INFO "Build Options:" ) 
MESSAGE( INFO "├─WERROR        ${WERROR}       " )
MESSAGE( INFO "├─BUILD DOCS    ${BUILD_DOCS}   " )
MESSAGE( INFO "├─BUILD TESTS   ${BUILD_TESTS}  " )
MESSAGE( INFO "├─RUN   TESTS   ${RUN_TESTS}    " )
MESSAGE( INFO "└─BUILD RELEASE ${BUILD_RELEASE}" )
MESSAGE( STATUS "" ) 

IF( BUILD_RELEASE  )
    MESSAGE( INFO "Building for release" )
    IF( MSVC )
      ADD_COMPILE_OPTIONS( /W3 /02 )
    ELSEIF( UNIX AND NOT APPLE )
      ADD_COMPILE_OPTIONS( -Wall -Wextra -pedantic -Werror -fPIC -O3 -Wno-cast-function-type )
    ENDIF()
ELSE()
    MESSAGE( INFO "Building for debug" )
    IF( MSVC )
      ADD_COMPILE_OPTIONS( /DEBUG )
    ELSEIF( UNIX AND NOT APPLE )
      ADD_COMPILE_OPTIONS( -Wall -Wextra -pedantic -Werror -fPIC -g -Wno-cast-function-type )
    ENDIF()
ENDIF()

# Set build config.
set(CMAKE_CXX_STANDARD          17 )
set(CMAKE_CXX_STANDARD_REQUIRED ON )

# Set build config.
SET( ARCHITECTURE "64bit" CACHE STRING "The system architecture."                     )
SET( CXX_STANDARD "17"    CACHE STRING "The C++ standard to use for building."        )
SET( MAJOR        "0"     CACHE STRING "The major version of this build."             )
SET( MINOR        "0"     CACHE STRING "The minor version of this build."             )
SET( BRANCH       "0"     CACHE STRING "The branch version of this build."            )
SET( REVISION     "0"     CACHE STRING "The revision of this build."                  )
SET( GENERATOR    "DEB"   CACHE STRING "The Package Generator to use for this build." )

IF( WIN32 )
  SET( INSTALL_LOCATION  "C:\\Program Files"         CACHE STRING "The default NSIS install location of this library" )
  SET( CMAKE_PREFIX_PATH "C:\\Program Files\\Athena" CACHE STRING "The default path to look for dependancies."        )
ELSEIF( UNIX AND NOT APPLE )
  SET( CMAKE_PREFIX_PATH "/usr/local/lib/Athena;" CACHE STRING "The default path to look for dependancies." )
  SET( INSTALL_LOCATION  "/usr/local/lib"         CACHE STRING "The RPM install location of this library."  ) 
ENDIF()

SET( PROJECT_VERSION "${MAJOR}.${MINOR}.${BRANCH}.${REVISION}" )

# Print build configurations.
MESSAGE( INFO " " ) 
MESSAGE( INFO "Build Configuration:"                      )
MESSAGE( INFO "├─ARCHITECTURE      : ${ARCHITECTURE}"     )
MESSAGE( INFO "├─INSTALL GENERATOR : ${GENERATOR}"        )
MESSAGE( INFO "├─INSTALL LOCATION  : ${INSTALL_LOCATION}" )
MESSAGE( INFO "├─CXX_STANDARD      : ${CXX_STANDARD}"     )
MESSAGE( INFO "└─VERSION           : ${PROJECT_VERSION}"  )
MESSAGE( INFO "  ├─MAJOR           : ${MAJOR}"            )
MESSAGE( INFO "  ├─MINOR           : ${MINOR}"            )
MESSAGE( INFO "  ├─BRANCH          : ${BRANCH}"           )
MESSAGE( INFO "  └─REVISION        : ${REVISION}"         )

STRING( TIMESTAMP DATE "%Y%m%d" )
STRING( TIMESTAMP Time "%H%M%S" )

# Handle compiler information
STRING( TOLOWER ${CMAKE_CXX_COMPILER_ID} CXX_COMPILER_NAME )
SET( CXX_COMPILER  "${CXX_COMPILER_NAME}${CMAKE_CXX_COMPILER_VERSION}" )

SET( CLANG FALSE )
SET( INTEL FALSE )
SET( GNU   FALSE )
SET( MSVC  FALSE )
IF( CXX_COMPILER_NAME MATCHES "clang" )
  SET( CLANG TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "gnu" )
  SET( GNU TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "intel" )
  SET( INTEL TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "msvc" )
  SET( MSVC TRUE )
ENDIF()

MESSAGE( INFO " " ) 
MESSAGE( INFO  "System Configuration:"                     )
MESSAGE( INFO  "├─CXX_COMPILER_NAME: ${CXX_COMPILER_NAME}" )
MESSAGE( INFO  "├─CXX_COMPILER:      ${CXX_COMPILER}"      )
MESSAGE( INFO  "├─INTEL:             ${INTEL}"             )
MESSAGE( INFO  "├─GNU:               ${GNU}"               )
MESSAGE( INFO  "└─MSVC:              ${MSVC}"              )
MESSAGE( INFO " " ) 


IF( UNIX )
  SET( BIN_DIR     bin     )
  SET( INCLUDE_DIR include )
  SET( LIB_DIR     lib64   )
  SET( DOC_DIR     doc     )
ELSE()
  SET( BIN_DIR     "." )
  SET( INCLUDE_DIR "." )
  SET( LIB_DIR     "." )
  SET( DOC_DIR     "." )
ENDIF()

SET( BUILD_DIR    ${CMAKE_BINARY_DIR}                        )
SET( HEADER_INSTALL_DIR ${INCLUDE_DIR}/${CMAKE_PROJECT_NAME} )

SET( EXPORT_BIN_DIR     "${BIN_DIR}"     )
SET( EXPORT_INCLUDE_DIR "${INCLUDE_DIR}" )
SET( EXPORT_LIB_DIR     "${LIB_DIR}"     )
SET( EXPORT_DOC_DIR     "${DOC_DIR}"     )

# Set output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib64 )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib64 )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin   )
set(DOCUMENTATION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/doc   )

IF( MSVC )
    SET( WINDOWS_EXPORT_ALL_SYMBOLS       TRUE )
    SET( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE )
    SET( BUILD_SHARED_LIBS TRUE                )
ENDIF()


SET( CPACK_COMPONENTS_ALL                 release devel                                                                   )
SET( CPACK_COMPONENT_DEVEL_DISPLAY_NAME   "${CMAKE_PROJECT_NAME}-devel"                                                   )
SET( CPACK_COMPONENT_RELEASE_DISPLAY_NAME "${CMAKE_PROJECT_NAME}-release"                                                 )
SET( CPACK_COMPONENT_DEVEL_DESCRIPTION    "Development Headers & Runtime Libraries for the ${CMAKE_PROJECT_NAME} library" )
SET( CPACK_COMPONENT_RELEASE_DESCRIPTION  "Runtime Libraries for the ${CMAKE_PROJECT_NAME} library"                       )
SET( CPACK_COMPONENT_DEVEL_DEPENDS        release                                                                         )

INCLUDE( KPackage ) 
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/src ) 
ADD_SUBDIRECTORY( src )
INSTALL( EXPORT Iris FILE IrisConfig.cmake DESTINATION ${CMAKE_PROJECT_NAME} COMPONENT devel )
