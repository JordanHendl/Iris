set( KARMA_MODULE_SOURCES 
     Module.cpp
     Loader.cpp
     Graph.cpp
     Manager.cpp
   )
     
set( KARMA_MODULE_HEADERS
     Module.h
     Loader.h
     Manager.h
     Graph.h
   )

set( KARMA_MODULE_INCLUDE_DIRS
      ${DATA_BUS_DIR}
      ${CONFIGURATION_DIR}
    )

set( KARMA_MODULE_LIBRARIES
     karma_io
     stdc++fs
     karma_data_bus
     karma_configuration
     karma_log
   )

add_library               ( karma_module SHARED   ${KARMA_MODULE_SOURCES}      ${KARMA_MODULE_HEADERS} )
target_include_directories( karma_module PRIVATE  ${KARMA_MODULE_INCLUDE_DIRS}                         )
target_link_libraries     ( karma_module PUBLIC   ${KARMA_MODULE_LIBRARIES}                            )

INSTALL( TARGETS karma_module EXPORT karma_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )