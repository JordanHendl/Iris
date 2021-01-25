FIND_PACKAGE( Threads REQUIRED )

SET( IRIS_DRIVER_SOURCES 
      Driver.cpp
      main.cpp
   )

SET( IRIS_DRIVER_HEADERS
      Driver.h
   )

SET( IRIS_DRIVER_INCLUDE_DIRS
   )

SET( IRIS_DRIVER_LIBRARIES
     iris
     stdc++fs
     pthread
     ${CMAKE_DL_LIBS}
    )

ADD_EXECUTABLE            ( iris_exe         ${IRIS_DRIVER_SOURCES} ${IRIS_DRIVER_HEADERS} )
TARGET_LINK_LIBRARIES     ( iris_exe PUBLIC  ${IRIS_DRIVER_LIBRARIES}                      )
TARGET_INCLUDE_DIRECTORIES( iris_exe PRIVATE ${IRIS_DRIVER_INCLUDE_DIRS}                   )

INSTALL( TARGETS iris_exe EXPORT Iris 
                 LIBRARY  DESTINATION ${EXPORT_BIN_DIR} 
                 ARCHIVE  DESTINATION ${EXPORT_BIN_DIR}
                 RUNTIME  DESTINATION ${EXPORT_BIN_DIR}
                 INCLUDES DESTINATION ${EXPORT_INCLUDE_DIR} )