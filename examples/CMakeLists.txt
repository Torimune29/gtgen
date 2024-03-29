cmake_minimum_required(VERSION 3.14)

option(ENABLE_GOOGLETESTS "Include tests in the library. Setting this to OFF will remove all googletest related code.
                        Tests in tests/*.cpp will still be enabled." ON)

# List all files containing tests. (Change as needed)
set(TESTFILES # All .cpp files in tests/
    free_function/test_code.cpp
    namespace_function/test_code.cpp
    class_function/test_code.cpp
    class_inherit_function/test_code.cpp
)

set(TEST_MAIN examples_${LIBRARY_NAME}) # Default name for test executable (change if you wish).
set(TEST_RUNNER_PARAMS "") # Any arguemnts to feed the test runner (change as needed).

# --------------------------------------------------------------------------------
# Make Tests (no change needed).
# --------------------------------------------------------------------------------
add_executable(${TEST_MAIN} ${TESTFILES})

target_link_libraries(${TEST_MAIN} PRIVATE pthread gtest_main gmock_main)
set_target_properties(${TEST_MAIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
target_set_warnings(${TEST_MAIN} ENABLE ALL DISABLE Annoying) # Set warnings (if needed).

set_target_properties(${TEST_MAIN} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_test(

    # Use some per-module/project prefix so that it is easier to run only tests for this module
    NAME ${LIBRARY_NAME}.${TEST_MAIN}
    COMMAND ${TEST_MAIN} ${TEST_RUNNER_PARAMS})

# Adds a 'coverage' target, global.
include(${PROJECT_SOURCE_DIR}/examples/cmake/examples.cmake)

# referring source tree class definition for testing
target_compile_definitions(${TEST_MAIN} PUBLIC SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../")
