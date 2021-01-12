IF( WIN32 )
 SET( CPACK_GENRATOR          "NSIS"                        )
 SET( CPACK_NSIS_PACKAGE_NAME install_${CMAKE_PROJECT_NAME} )
 SET( CPACK_NSIS_MODIFY_PATH ON                             )
 SET( CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_PROJECT_NAME} )
 SET( CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL OFF        )
 SET( CPACK_NSIS_MODIFY_PATH                     ON         )
ENDIF() 

IF( UNIX )
  SET( CPACK_GENERATOR      "RPM"                                        )
  SET( CPACK_PACKAGING_INSTALL_PREFIX "/usr/local/${CMAKE_PROJECT_NAME}" )
  SET( CPACK_RPM_SPEC_MORE_DEFINE     "%define _build_id_links none"     )
ENDIF() 

SET( CPACK_PACKAGE_VERSION ${PROJECT_VERSION}    )
SET( CPACK_PACKAGE_NAME    ${CMAKE_PROJECT_NAME} )

INCLUDE( CPack )