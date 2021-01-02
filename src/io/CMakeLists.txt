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

set( KARMA_IO_SOURCES 
     ObjectLoader.cpp
   )

set( KARMA_IO_HEADERS
     ObjectLoader.h
   )

set( KARMA_IO_INCLUDE_DIRS
   )

set( KARMA_IO_LIBRARIES
   )


add_library               ( karma_io SHARED  ${KARMA_IO_SOURCES} ${KARMA_IO_HEADERS} )
target_link_libraries     ( karma_io PUBLIC  ${KARMA_IO_LIBRARIES}                   )
target_include_directories( karma_io PRIVATE ${KARMA_IO_INCLUDE_DIRS}                )

INSTALL( FILES ObjectLoader.h DESTINATION ${INSTALL_INCLUDE_DIR}/io )

INSTALL( TARGETS karma_io EXPORT karma_lib 
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}/io 
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR}/io )