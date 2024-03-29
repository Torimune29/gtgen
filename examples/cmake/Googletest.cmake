if(ENABLE_GOOGLETESTS)
    add_definitions(-DENABLE_GOOGLETEST_IN_LIBRARY)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    set(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
    set(GMOCK_INCLUDE_DIR ${gmock_SOURCE_DIR}/include)
    include_directories(${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})
endif()
