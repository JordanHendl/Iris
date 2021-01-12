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

SET( KARMA_CONFIG_HEADERS
      Configuration.h
      Parser.h
   )

SET( KARMA_CONFIG_SOURCES
      Configuration.cpp
      Parser.cpp
   )

SET( KARMA_CONFIG_INCLUDES 
     ${DATA_BUS_DIR}
     ${CONFIGURATION_DIR}
   )

SET( KARMA_CONFIG_LIBRARIES 
     karma_data_bus
     karma_log
   )

SET( KARMA_CONFIG_DEPS
     karma_data_bus
     karma_log
   )

ADD_LIBRARY               ( karma_configuration SHARED  ${KARMA_CONFIG_SOURCES} ${KARMA_CONFIG_HEADERS} )
TARGET_INCLUDE_DIRECTORIES( karma_configuration PRIVATE ${KARMA_CONFIG_INCLUDES}                        ) 
TARGET_LINK_LIBRARIES     ( karma_configuration PUBLIC  ${KARMA_CONFIG_LIBRARIES}                       ) 
ADD_DEPENDENCIES          ( karma_configuration         ${KARMA_CONFIG_DEPS}                            )

# If running tests, make and run them.
IF( RUN_TESTS )
  BUILD_TEST( TARGET karma_configuration )
ENDIF()

INSTALL( FILES ${KARMA_CONFIG_HEADERS} DESTINATION ${INCLUDE_DIR}/config )
INSTALL( TARGETS karma_configuration EXPORT karma_lib 
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}/config
                 ARCHIVE  DESTINATION ${EXPORT_LIB_DIR}/config
                 RUNTIME  DESTINATION ${EXPORT_LIB_DIR}/config
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )