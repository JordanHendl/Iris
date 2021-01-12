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
set( KARMA_TEST_MODULE_SOURCES 
     TestModule.cpp
   )
     
set( KARMA_TEST_MODULE_HEADERS
     TestModule.h
   )

set( KARMA_TEST_MODULE_INCLUDE_DIRS
    )

set( KARMA_TEST_MODULE_LIBRARIES
     karma_module
     karma_data_bus
     karma_log
   )

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_module )
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_module )
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_module )

ADD_LIBRARY               ( karma_test_module SHARED   ${KARMA_TEST_MODULE_SOURCES} ${KARMA_TEST_MODULE_HEADERS} )
TARGET_INCLUDE_DIRECTORIES( karma_test_module PRIVATE  ${KARMA_TEST_MODULE_INCLUDE_DIRS}                         )
TARGET_LINK_LIBRARIES     ( karma_test_module PUBLIC   ${KARMA_TEST_MODULE_LIBRARIES}                            )
