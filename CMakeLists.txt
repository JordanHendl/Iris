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

LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules" )
LIST( APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/modules" )

INCLUDE( Message   )
INCLUDE( BuildTest )

# Build options
OPTION( WERROR        "Whether or not warnings are treated as errors." ON  )
OPTION( BUILD_DOCS    "Whether or not to build doxygen documentation"  ON  )
OPTION( BUILD_RELEASE "Whether or not the to build for release.     "  OFF )
OPTION( RUN_TESTS     "Whether or not tests should be built and run "  ON  )

PROJECT( Karma CXX )

# Print build options
MESSAGE( STATUS "" ) 
MESSAGE( INFO "Build Options:" ) 
MESSAGE( INFO "├─WERROR        ${WERROR}       " )
MESSAGE( INFO "├─BUILD DOCS    ${BUILD_DOCS}   " )
MESSAGE( INFO "├─RUN TESTS     ${RUN_TESTS}    " )
MESSAGE( INFO "└─BUILD RELEASE ${BUILD_RELEASE}" )
MESSAGE( STATUS "" ) 

IF( BUILD_RELEASE  )
    MESSAGE( INFO "-Building for release" )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )
ELSE()
    MESSAGE( INFO "-Building for debug" )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
ENDIF()

# Set build config.
set(CMAKE_CXX_STANDARD          17 )
set(CMAKE_CXX_STANDARD_REQUIRED ON )

SET( ARCHITECTURE "64bit" CACHE STRING "The system architecture."              )
SET( CXX_STANDARD "17"    CACHE STRING "The C++ standard to use for building." )
SET( MAJOR        "0"     CACHE STRING "The major version of this build."      )
SET( MINOR        "1"     CACHE STRING "The minor version of this build."      )
SET( BRANCH       "1"     CACHE STRING "The branch version of this build."     )
SET( REVISION     "0"     CACHE STRING "The revision of this build."           )
SET( PROJECT_VERSION "${MAJOR}.${MINOR}.${BRANCH}.${REVISION}" )

# Print build configurations.
MESSAGE( INFO " " ) 
MESSAGE( INFO "Build Configuration:"                 )
MESSAGE( INFO "├─ARCHITECTURE:   ${ARCHITECTURE}"    )
MESSAGE( INFO "├─CXX_STANDARD:   ${CXX_STANDARD}"    )
MESSAGE( INFO "└─VERSION:        ${PROJECT_VERSION}" )
MESSAGE( INFO "  ├─MAJOR:        ${MAJOR}"           )
MESSAGE( INFO "  ├─MINOR:        ${MINOR}"           )
MESSAGE( INFO "  ├─BRANCH:       ${BRANCH}"          )
MESSAGE( INFO "  └─REVISION:     ${REVISION}"        )

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

INCLUDE( KPackage ) 

SET( BIN_DIR ${CMAKE_BINARY_DIR}/bin ) 
SET( INSTALL_BIN_DIR     "bin"                          )
SET( INSTALL_INCLUDE_DIR "include/${CMAKE_PROJECT_NAME}")
SET( INSTALL_LIB_DIR     "lib64"                        )
SET( INSTALL_DOC_DIR     "doc"                          )
SET( INSTALL_CONFIG_DIR  "config"                       )

SET( EXPORT_BIN_DIR     "${CPACK_PACKAGING_INSTALL_PREFIX}/${INSTALL_BIN_DIR}"     )
SET( EXPORT_INCLUDE_DIR "${CPACK_PACKAGING_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR}" )
SET( EXPORT_LIB_DIR     "${CPACK_PACKAGING_INSTALL_PREFIX}/${INSTALL_LIB_DIR}"     )
SET( EXPORT_DOC_DIR     "${CPACK_PACKAGING_INSTALL_PREFIX}/${INSTALL_DOC_DIR}"     )
SET( EXPORT_CONFIG_DIR  "${CPACK_PACKAGING_INSTALL_PREFIX}/${INSTALL_CONFIG_DIR}"  )

# Set output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib64 )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib64 )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin   )
set(DOCUMENTATION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/doc   )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/src ) 

ADD_SUBDIRECTORY( src )

INSTALL( EXPORT karma_lib FILE KarmaConfig.cmake DESTINATION ${INSTALL_CONFIG_DIR} COMPONENT dev )