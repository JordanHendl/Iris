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

FIND_PACKAGE( Threads )
ADD_SUBDIRECTORY( test_module )

SET( IRIS_MODULE_SOURCES 
     Module.cpp
     Loader.cpp
     Graph.cpp
     Manager.cpp
   )
     
SET( IRIS_MODULE_HEADERS
     Module.h
     Loader.h
     Manager.h
     Graph.h
   )

SET( IRIS_MODULE_INCLUDE_DIRS
    )

SET( IRIS_MODULE_LIBRARIES
     iris_io
     iris_bus
     iris_configuration
     iris_log
     ${CMAKE_THREAD_LIBS_INIT}
   )

IF( UNIX AND NOT APPLE )
  LIST( APPEND IRIS_MODULE_LIBRARIES stdc++fs )
ENDIF()

ADD_LIBRARY               ( iris_module SHARED   ${IRIS_MODULE_SOURCES} ${IRIS_MODULE_HEADERS} )
TARGET_INCLUDE_DIRECTORIES( iris_module PRIVATE  ${IRIS_MODULE_INCLUDE_DIRS}                   )
TARGET_LINK_LIBRARIES     ( iris_module PUBLIC   ${IRIS_MODULE_LIBRARIES}                      )

FILE( COPY test_json.json DESTINATION ${BUILD_DIR}/bin )

# If running tests, make and run them.
BUILD_TEST( TARGET iris_module )

INSTALL( FILES ${IRIS_MODULE_HEADERS} DESTINATION ${HEADER_INSTALL_DIR}/module )

INSTALL( TARGETS iris_module EXPORT Iris
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR} 
                 RUNTIME  DESTINATION ${EXPORT_LIB_DIR}
                 ARCHIVE  DESTINATION ${EXPORT_LIB_DIR}
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )