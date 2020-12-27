SET( KARMA_LOG_SOURCES 
     Log.cpp
   )
      
SET( KARMA_LOG_HEADERS
     Log.h
   )

SET( KARMA_LOG_INCLUDES
   )

SET( KARMA_LOG_LIBRARIES
    )

ADD_LIBRARY( karma_log SHARED ${KARMA_LOG_SOURCES} ${KARMA_LOG_HEADERS} )

ADD_EXECUTABLE       ( karma_log_test test.cpp          )
TARGET_LINK_LIBRARIES( karma_log_test PUBLIC karma_log ) 

INSTALL( TARGETS karma_log EXPORT karma_lib DESTINATION ${KARMA_LIB_INSTALL_DIR} )
EXPORT ( PACKAGE karma_log                                                       )
INSTALL( TARGETS karma_log DESTINATION ${KARMA_LIB_INSTALL_DIR}                  )
