CMAKE_MINIMUM_REQUIRED( VERSION 3.8.0 )
PROJECT( karma LANGUAGES CXX )

#Build Options
OPTION( BUILD_DOC      "Whether or not documentation should be build"                       OFF ) 
OPTION( RELEASE        "Whether or not to build with optimizations for release."            OFF ) 
OPTION( WERROR         "Whether warnings should be treated as errors."                      ON  )

SET( MAJOR_VERSION 0 )
SET( MINOR_VERSION 1 )
SET( PATCH_VERSION 0 )

SET( CMAKE_CXX_STANDARD 17 )
SET(CMAKE_SKIP_BUILD_RPATH FALSE)


MESSAGE( "Warnings as Errors  : ${WERROR}"                                          ) 
MESSAGE( "Build Documentation : ${BUILD_DOC}"                                       ) 
MESSAGE( "Release             : ${RELEASE}"                                         ) 
MESSAGE( "Version             : ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}" ) 

IF( WERROR STREQUAL "OFF" )

    MESSAGE( "-Compiling without warnings as errors." )

    # which compilers to use for C and C++
    SET(CMAKE_C_FLAGS   "-fPIC" )
    SET(CMAKE_CXX_FLAGS "-std=c++17 -lstdc++fs -fPIC")

ELSE()
    MESSAGE( "-Compiling with warnings as errors." )

    SET(CMAKE_C_FLAGS   "-fPIC -Werror -Wall"                       )
    SET(CMAKE_CXX_FLAGS "-std=c++17 -lstdc++fs -fPIC -Werror -Wall ")

ENDIF( WERROR STREQUAL "OFF" )

IF( RELEASE STREQUAL "ON" )
    MESSAGE( "-Building for release" )
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O3"   )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )
ELSE()
    MESSAGE( "-Building for debug" )
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -g"   )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g" )
ENDIF( RELEASE STREQUAL "ON" )


#Application information.
SET( ENGINE_NAME              "Karma"                       )
SET( VENDOR                   "Jorda Hendl"                 ) 
SET( INSTALL_SUMMARY          "Karma Engine - Installation" )
SET( INSTALL_DIR              "./karma"                     )
SET( LIB_INSTALL_DIR          "./karma/lib64"               )
SET( BIN_INSTALL_DIR          "./karma/bin"                 )
SET( INC_INSTALL_DIR          "./karma/include"             )
SET( KARMA_LIB_INSTALL_DIR    "karma/lib"                   )
SET( KARMA_HEADER_INSTALL_DIR "karma/include"               )
SET( KARMA_MODULE_INSTALL_DIR "karma/module"                )
SET( KARMA_INSTALL_DIR        "karma"                       )

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" )

# Set output directories.
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  )
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  )
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  )
SET(DOCUMENTATION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs )

#Create output directory for documentation.
FILE( MAKE_DIRECTORY ${DOCUMENTATION_OUTPUT_DIRECTORY} )

#CPack options
SET( CPACK_PACKAGE_NAME                ${ENGINE_NAME}        )
SET( CPACK_SOURCE_GENERATOR "TGZ;ZIP"                        )
SET( CPACK_PACKAGE_VENDOR              ${VENDOR}             )
SET( CPACK_PACKAGE_DESCRIPTION_SUMMARY ${INSTALL_SUMMARY}    )
SET( CPACK_PACKAGE_VERSION_MAJOR       ${MAJOR_VERSION}      )
SET( CPACK_PACKAGE_VERSION_MINOR       ${MINOR_VERSION}      )
SET( CPACK_PACKAGE_VERSION_PATCH       ${PATCH_VERSION}      )
SET( CPACK_PACKAGE_INSTALL_DIRECTORY   "./"                  )
SET( CMAKE_INSTALL_PREFIX              "./"                  ) 
SET( PROJECT_SRCS       ${CMAKE_WORKING_DIRECTORY}/src       )
SET( DOXYGEN_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doxygen  )
SET( CMAKE_INSTALL_RPATH FALSE                               )
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/karma/lib" )

SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )
LIST( FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/karma/lib" isSystemDir )
IF( "${isSystemDir}" STREQUAL "-1" )
    SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
ENDIF( "${isSystemDir}" STREQUAL "-1" )

INCLUDE     ( CPack   )
FIND_PACKAGE( Doxygen )

INSTALL( FILES setup.json        DESTINATION ${KARMA_INSTALL_DIR} ) 
INSTALL( FILES karma_config.json DESTINATION ${KARMA_INSTALL_DIR} ) 

ADD_SUBDIRECTORY( src )