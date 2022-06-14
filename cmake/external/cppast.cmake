find_package(cppast QUIET)
if(NOT cppast_FOUND)
  message(STATUS "Fetching cppast")

  include(FetchContent)
  FetchContent_Declare(
    cppast
    GIT_REPOSITORY https://github.com/foonathan/cppast
    GIT_TAG        v0.1.0
  )


  FetchCOntent_GetProperties(cppast)
  if(NOT cppast_POPULATED)
    FetchContent_Populate(cppast)
    # for excluding type_safe make install
    add_subdirectory(${cppast_SOURCE_DIR} ${cppast_BINARY_DIR} EXCLUDE_FROM_ALL)
    target_set_warnings(cppast DISABLE ALL DISABLE Annoying) # No warnings.
    # for inculde.
    target_include_directories(${LIBRARY_NAME} SYSTEM PRIVATE ${cppast_SOURCE_DIR}/cppast/include)
  endif()
endif()

target_link_libraries(${LIBRARY_NAME} PRIVATE cppast)
target_compile_options(${LIBRARY_NAME} PRIVATE "-Wno-weak-vtables")  # for cppast virtual on headerfile inline.
