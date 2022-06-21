find_package(cppcodegen QUIET)

if(NOT cppcodegen_FOUND)
  message(STATUS "Fetching cppcodegen")

  include(FetchContent)
  FetchContent_Populate(
    cppcodegen
    GIT_REPOSITORY https://github.com/Torimune29/cppcodegen
    GIT_TAG origin/main
    QUIET
    SUBBUILD_DIR ${FETCHCONTENT_BASE_DIR}/cppcodegen-subbuild
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/cppcodegen-src
    BINARY_DIR ${FETCHCONTENT_BASE_DIR}/cppcodegen-build
  )

  # for inculde.
  target_include_directories(${LIBRARY_NAME} SYSTEM PRIVATE ${cppcodegen_SOURCE_DIR}/include)
endif()