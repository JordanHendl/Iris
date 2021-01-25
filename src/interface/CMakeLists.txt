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

SET( IRIS_SOURCES 
    Iris.cpp
   )
      
SET( IRIS_HEADERS
     Iris.h
   )

SET( IRIS_INCLUDE_DIRS
   )

SET( IRIS_LIBRARIES
     iris_module
     iris_io
     iris_configuration
     iris_bus
     iris_log
    )

SET( IRIS_DEPS
     iris_module
     iris_io
     iris_configuration
     iris_bus
     iris_log
    )

ADD_LIBRARY               ( iris SHARED  ${IRIS_SOURCES}     ${IRIS_HEADERS} )
TARGET_LINK_LIBRARIES     ( iris PUBLIC  ${IRIS_LIBRARIES}                   )
TARGET_INCLUDE_DIRECTORIES( iris PRIVATE ${IRIS_INCLUDE_DIRS}                )
ADD_DEPENDENCIES          ( iris ${IRIS_DEPS}                                ) 

set( CMAKE_EXPORT_PACKAGE_REGISTRY ON )

INSTALL( FILES   Iris.h DESTINATION ${INCLUDE_DIR} )
INSTALL( TARGETS iris EXPORT Iris 
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}
                 ARCHIVE  DESTINATION ${EXPORT_LIB_DIR}
                 RUNTIME  DESTINATION ${EXPORT_LIB_DIR}
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )