set( KARMA_CONFIG_HEADERS
      Configuration.h
      Parser.h
   )

set( KARMA_CONFIG_SOURCES
      Configuration.cpp
      Parser.cpp
   )

set( KARMA_CONFIG_INCLUDES 
     ${DATA_BUS_DIR}
     ${CONFIGURATION_DIR}
   )

set( KARMA_CONFIG_LIBRARIES 
     karma_data_bus
     karma_log
   )

set( KARMA_CONFIG_DEPS
     karma_data_bus
     karma_log
   )

add_library               ( karma_configuration SHARED  ${KARMA_CONFIG_SOURCES} ${KARMA_CONFIG_HEADERS} )
target_include_directories( karma_configuration PRIVATE ${KARMA_CONFIG_INCLUDES}                        ) 
target_link_libraries     ( karma_configuration PUBLIC ${KARMA_CONFIG_LIBRARIES}                       ) 
add_dependencies          ( karma_configuration         ${KARMA_CONFIG_DEPS}                            )
add_executable       ( karma_config_test test.cpp      )
target_link_libraries( karma_config_test karma_configuration )

INSTALL( TARGETS karma_configuration EXPORT karma_lib DESTINATION ${KARMA_LIB_INSTALL_DIR}    )
INSTALL( TARGETS karma_configuration                  DESTINATION ${KARMA_LIB_INSTALL_DIR}    )
INSTALL( FILES ${KARMA_CONFIG_HEADERS}                DESTINATION ${KARMA_HEADER_INSTALL_DIR} )