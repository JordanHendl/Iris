SET( DATA_BUS_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/data   )
SET( CONFIGURATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

ADD_SUBDIRECTORY( config    )
ADD_SUBDIRECTORY( data      )
ADD_SUBDIRECTORY( interface )
ADD_SUBDIRECTORY( node      )
ADD_SUBDIRECTORY( io        )
ADD_SUBDIRECTORY( log       )
ADD_SUBDIRECTORY( profiling )

IF( ${DOXYGEN_FOUND} AND ${BUILD_DOC} )
    # set input and output files
    SET(DOXYGEN_IN  ${DOXYGEN_CONFIG_DIR}/doxygen.cfg                 )
    SET(DOXYGEN_OUT ${DOCUMENTATION_OUTPUT_DIRECTORY}/doc/doxygen.cfg )
    
    SET( DOC_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/ )

    # request to configure the file
    CONFIGURE_FILE( ${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY )

    MESSAGE( "Generating Karma documentation" )

    # note the option ALL which allows to build the docs together with the application
    ADD_CUSTOM_TARGET( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

ELSEIF( ${BUILD_DOC} )
  MESSAGE("Doxygen need to be installed to generate documentation")

ENDIF( ${DOXYGEN_FOUND} AND ${BUILD_DOC} )
