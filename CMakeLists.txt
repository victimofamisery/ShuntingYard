cmake_minimum_required(VERSION 3.24)
set(PROJECT_NAME ShuntingYard)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} main.cpp shuntingyard.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(
  hello_test
  hello_test.cpp
  shuntingyard.cpp
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)