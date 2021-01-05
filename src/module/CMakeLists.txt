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

SET( KARMA_MODULE_SOURCES 
     Module.cpp
     Loader.cpp
     Graph.cpp
     Manager.cpp
   )
     
SET( KARMA_MODULE_HEADERS
     Module.h
     Loader.h
     Manager.h
     Graph.h
   )

SET( KARMA_MODULE_INCLUDE_DIRS
    )

SET( KARMA_MODULE_LIBRARIES
     karma_io
     stdc++fs
     karma_data_bus
     karma_configuration
     karma_log
     ${CMAKE_THREAD_LIBS_INIT}
   )

ADD_LIBRARY               ( karma_module SHARED   ${KARMA_MODULE_SOURCES} ${KARMA_MODULE_HEADERS} )
TARGET_INCLUDE_DIRECTORIES( karma_module PRIVATE  ${KARMA_MODULE_INCLUDE_DIRS}                    )
TARGET_LINK_LIBRARIES     ( karma_module PUBLIC   ${KARMA_MODULE_LIBRARIES}                       )

ADD_SUBDIRECTORY( test_module )

# If running tests, make and run them.
IF( RUN_TESTS )
  BUILD_TEST( TARGET karma_module )
ENDIF()

INSTALL( FILES ${KARMA_MODULE_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/module )

INSTALL( TARGETS karma_module EXPORT karma_lib
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}/module 
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )