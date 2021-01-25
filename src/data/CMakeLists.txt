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

SET( IRIS_BUS_SOURCES 
      Bus.cpp 
   )
      
SET( IRIS_BUS_HEADERS
      Bus.h
   )

SET( IRIS_BUS_INCLUDES
   )

SET( IRIS_BUS_LIBRARIES
    )

ADD_LIBRARY( iris_bus SHARED ${IRIS_BUS_SOURCES} ${IRIS_BUS_HEADERS} )

  # If running tests, make and run them.
  IF( RUN_TESTS )
    BUILD_TEST( TARGET iris_bus )
  ENDIF()

INSTALL( FILES   ${IRIS_BUS_HEADERS} DESTINATION ${INCLUDE_DIR}/data )
INSTALL( TARGETS  iris_bus EXPORT Iris 
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}/data 
                 ARCHIVE  DESTINATION ${EXPORT_LIB_DIR}/data
                 RUNTIME  DESTINATION ${EXPORT_LIB_DIR}/data
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )
