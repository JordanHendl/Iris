set( KARMA_SOURCES 
    Karma.cpp
   )
      
set( KARMA_HEADERS
     Karma.h
   )

set( KARMA_INCLUDE_DIRS
   )

set( KARMA_LIBRARIES
     karma_module
     karma_io
     karma_configuration
     karma_data_bus
     karma_log
    )

set( KARMA_DEPS
     karma_module
     karma_io
     karma_configuration
     karma_data_bus
     karma_log
    )

ADD_LIBRARY               ( karma SHARED  ${KARMA_SOURCES}     ${KARMA_HEADERS} )
TARGET_LINK_LIBRARIES     ( karma PUBLIC  ${KARMA_LIBRARIES}                    )
TARGET_INCLUDE_DIRECTORIES( karma PRIVATE ${KARMA_INCLUDE_DIRS}                 )
ADD_DEPENDENCIES          ( karma ${KARMA_DEPS}                                 ) 

set( CMAKE_EXPORT_PACKAGE_REGISTRY ON )
INSTALL( TARGETS karma EXPORT karma_lib DESTINATION ${KARMA_LIB_INSTALL_DIR}                        )
INSTALL( EXPORT  karma_lib              DESTINATION ${KARMA_LIB_INSTALL_DIR} FILE KarmaConfig.cmake )
INSTALL( FILES   Karma.h                DESTINATION ${KARMA_HEADER_INSTALL_DIR}                     )