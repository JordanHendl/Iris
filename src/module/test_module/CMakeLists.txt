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
set( IRIS_TEST_MODULE_SOURCES 
     TestModule.cpp
   )
     
set( IRIS_TEST_MODULE_HEADERS
     TestModule.h
   )

set( IRIS_TEST_MODULE_INCLUDE_DIRS
    )

set( IRIS_TEST_MODULE_LIBRARIES
     iris_module
     iris_bus
     iris_log
   )

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_module )
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_module )
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_module )

ADD_LIBRARY               ( iris_test_module SHARED   ${IRIS_TEST_MODULE_SOURCES} ${IRIS_TEST_MODULE_HEADERS} )
TARGET_INCLUDE_DIRECTORIES( iris_test_module PRIVATE  ${IRIS_TEST_MODULE_INCLUDE_DIRS}                        )
TARGET_LINK_LIBRARIES     ( iris_test_module PUBLIC   ${IRIS_TEST_MODULE_LIBRARIES}                           )
