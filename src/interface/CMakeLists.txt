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

SET( KARMA_SOURCES 
    Karma.cpp
   )
      
SET( KARMA_HEADERS
     Karma.h
   )

SET( KARMA_INCLUDE_DIRS
   )

SET( KARMA_LIBRARIES
     karma_module
     karma_io
     karma_configuration
     karma_data_bus
     karma_log
    )

SET( KARMA_DEPS
     karma_module
     karma_io
     karma_configuration
     karma_data_bus
     karma_log
    )

ADD_LIBRARY               ( karma SHARED  ${KARMA_SOURCES}     ${KARMA_HEADERS} )
TARGET_LINK_LIBRARIES     ( karma PUBLIC  ${KARMA_LIBRARIES}                    )
TARGET_INCLUDE_DIRECTORIES( karma PRIVATE ${KARMA_INCLUDE_DIRS}                 )
ADD_DEPENDENCIES          ( karma ${KARMA_DEPS}                                 ) 

set( CMAKE_EXPORT_PACKAGE_REGISTRY ON )

INSTALL( FILES   Karma.h                DESTINATION ${INSTALL_INCLUDE_DIR} )
INSTALL( TARGETS karma EXPORT karma_lib 
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR} 
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )