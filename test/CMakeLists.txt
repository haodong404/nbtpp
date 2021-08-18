include(FetchContent)
FetchContent_Declare(gtest
        QUIET
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
        )
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

add_executable(nbtpp_test test.cpp)

target_link_libraries(nbtpp_test
        PRIVATE
        nbtpp
        gtest_main
        )
include(GoogleTest)
gtest_discover_tests(nbtpp_test)