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

set( IRIS_IO_SOURCES 
     ObjectLoader.cpp
   )

set( IRIS_IO_HEADERS
     ObjectLoader.h
   )

set( IRIS_IO_INCLUDE_DIRS
   )

set( IRIS_IO_LIBRARIES
    ${CMAKE_DL_LIBS}
    iris_log
   )


add_library               ( iris_io SHARED  ${IRIS_IO_SOURCES} ${IRIS_IO_HEADERS} )
target_link_libraries     ( iris_io PUBLIC  ${IRIS_IO_LIBRARIES}                  )
target_include_directories( iris_io PRIVATE ${IRIS_IO_INCLUDE_DIRS}               )

# If running tests, make and run them.
IF( RUN_TESTS )
  BUILD_TEST( TARGET iris_io )
ENDIF()

INSTALL( FILES ObjectLoader.h DESTINATION ${HEADER_INSTALL_DIR}/io COMPONENT devel )
INSTALL( TARGETS iris_io EXPORT Iris COMPONENT release
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR} 
                 ARCHIVE  DESTINATION ${EXPORT_LIB_DIR} 
                 RUNTIME  DESTINATION ${EXPORT_LIB_DIR} 
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )