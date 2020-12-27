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

INSTALL( TARGETS karma_io EXPORT karma_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )
EXPORT ( PACKAGE karma_io                                                       )
INSTALL( TARGETS karma_io DESTINATION ${KARMA_LIB_INSTALL_DIR}                  )
INSTALL( FILES ObjectLoader.h DESTINATION ${KARMA_HEADER_INSTALL_DIR}           )