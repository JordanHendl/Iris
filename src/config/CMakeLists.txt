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

SET( IRIS_CONFIG_HEADERS
      Configuration.h
      Parser.h
   )

SET( IRIS_CONFIG_SOURCES
      Configuration.cpp
      Parser.cpp
   )

SET( IRIS_CONFIG_INCLUDES 
     ${DATA_BUS_DIR}
     ${CONFIGURATION_DIR}
   )

SET( IRIS_CONFIG_LIBRARIES 
     iris_bus
     iris_log
   )

SET( IRIS_CONFIG_DEPS
     iris_bus
     iris_log
   )

ADD_LIBRARY               ( iris_configuration SHARED  ${IRIS_CONFIG_SOURCES} ${IRIS_CONFIG_HEADERS} )
TARGET_INCLUDE_DIRECTORIES( iris_configuration PRIVATE ${IRIS_CONFIG_INCLUDES}                        ) 
TARGET_LINK_LIBRARIES     ( iris_configuration PUBLIC  ${IRIS_CONFIG_LIBRARIES}                       ) 
ADD_DEPENDENCIES          ( iris_configuration         ${IRIS_CONFIG_DEPS}                            )

# If running tests, make and run them.
IF( RUN_TESTS )
  BUILD_TEST( TARGET iris_configuration )
ENDIF()

INSTALL( FILES ${IRIS_CONFIG_HEADERS} DESTINATION ${INCLUDE_DIR}/config )
INSTALL( TARGETS iris_configuration EXPORT Iris 
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}
                 ARCHIVE  DESTINATION ${EXPORT_LIB_DIR}
                 RUNTIME  DESTINATION ${EXPORT_LIB_DIR}
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )