# Helper function to configure the test maker's global state.
FUNCTION( CONFIGURE_TEST )
  BUILD_TEST( ${ARGV} )
ENDFUNCTION()

# Function to help manage building tests.
FUNCTION( BUILD_TEST )
  
  CMAKE_POLICY( SET CMP0057 NEW )

  # Test configurations.
  SET( VARIABLES 
        TARGET
        DEPENDS
     )
  
  # For each argument provided.
  FOREACH( ARG ${ARGV} )
    
    # If argument is one of the variables, set it.
    IF( "${ARG}" IN_LIST VARIABLES )
      SET( STATE ${ARG} )
    ELSE()
      # If our state is a variable, set that variables value
      IF( "${${STATE}}" )
        SET( ${STATE} ${ARG} )
      ELSE()
        LIST( APPEND ${STATE} ${ARG} )
      ENDIF()

      # If our state is a setter, set the value in the parent scope as well
      IF( "${STATE}" IN_LIST CONFIGS )
        SET( ${STATE} ${${STATE}} PARENT_SCOPE )
      ENDIF()
    ENDIF()
  ENDFOREACH()

    IF( TARGET )
      IF( BUILD_TESTS )
        FIND_PACKAGE( Athena REQUIRED )
        # Add Test executable.
        ADD_EXECUTABLE       ( "${TARGET}_test" Test.cpp                         )
        TARGET_LINK_LIBRARIES( "${TARGET}_test" ${TARGET} athena ${DEPENDANCIES} )

        IF( RUN_TESTS )
          # If we should run tests, add custom command to run them after the fact.
          ADD_CUSTOM_COMMAND(
            POST_BUILD
            OUTPUT ${TARGET}_test_execution
            COMMAND ${TARGET}_test
            WORKING_DIRECTORY ${BUILD_DIR}/${TEST_DIR}
          )
      
          ADD_CUSTOM_TARGET(
            ${TARGET}_test_flag ALL
            DEPENDS ${TARGET}_test_execution
          )
        ENDIF()
      ENDIF()
    ENDIF()
ENDFUNCTION()