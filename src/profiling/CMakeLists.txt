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

SET( KARMA_PROFILING_SOURCES 
     Timer.cpp
   )
      
SET( KARMA_PROFILING_HEADERS
     Timer.h
   )

SET( KARMA_PROFILING_INCLUDES
   )

SET( KARMA_PROFILING_LIBRARIES
    )

ADD_LIBRARY( karma_profiling SHARED ${KARMA_PROFILING_SOURCES} ${KARMA_PROFILING_HEADERS} )

INSTALL( FILES ${KARMA_PROFILING_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR}/profiling )
INSTALL( TARGETS karma_profiling EXPORT karma_lib 
                 LIBRARY  DESTINATION ${EXPORT_LIB_DIR}/profiling 
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR}/profiling )