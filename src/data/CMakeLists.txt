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

SET( DATA_BUS_SOURCES 
      Bus.cpp 
   )
      
SET( DATA_BUS_HEADERS
      Bus.h
   )

SET( DATA_BUS_INCLUDES
   )

SET( DATA_BUS_LIBRARIES
    )

ADD_LIBRARY( karma_data_bus SHARED ${DATA_BUS_SOURCES} ${DATA_BUS_HEADERS} )

  # If running tests, make and run them.
  IF( RUN_TESTS )
    BUILD_TEST( TARGET karma_data_bus )
  ENDIF()

INSTALL( FILES   ${DATA_BUS_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/data )
INSTALL( TARGETS karma_data_bus EXPORT karma_lib 
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}/data 
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR}/data )
