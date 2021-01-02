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

set( KARMA_MODULE_SOURCES 
     Module.cpp
     Loader.cpp
     Graph.cpp
     Manager.cpp
   )
     
set( KARMA_MODULE_HEADERS
     Module.h
     Loader.h
     Manager.h
     Graph.h
   )

set( KARMA_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${CONFIGURATION_DIR}
    )

set( KARMA_MODULE_LIBRARIES
     karma_io
     stdc++fs
     karma_data_bus
     karma_configuration
     karma_log
   )

add_library               ( karma_module SHARED   ${KARMA_MODULE_SOURCES}      ${KARMA_MODULE_HEADERS} )
target_include_directories( karma_module PRIVATE  ${KARMA_MODULE_INCLUDE_DIRS}                         )
target_link_libraries     ( karma_module PUBLIC   ${KARMA_MODULE_LIBRARIES}                            )

INSTALL( FILES ${KARMA_MODULE_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR} )

INSTALL( TARGETS karma_module EXPORT karma_lib
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}/node 
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR}/node )