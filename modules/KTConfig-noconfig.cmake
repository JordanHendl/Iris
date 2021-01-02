#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "karma_test" for configuration ""
set_property(TARGET karma_test APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(karma_test PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "/usr/local/KT/lib64/libkarma_test.so"
  IMPORTED_SONAME_NOCONFIG "libkarma_test.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS karma_test )
list(APPEND _IMPORT_CHECK_FILES_FOR_karma_test "/usr/local/KT/lib64/libkarma_test.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
