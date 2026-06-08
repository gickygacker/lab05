cmake_minimum_required(VERSION 3.14)
project(banking)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(banking STATIC Account.cpp Transaction.cpp)
target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(banking PROPERTIES POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(banking_tests tests/test_account.cpp tests/test_transaction.cpp)
target_link_libraries(banking_tests PRIVATE banking gtest_main gmock_main)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(banking PRIVATE --coverage)
  target_compile_options(banking_tests PRIVATE --coverage)
  target_link_options(banking_tests PRIVATE --coverage)
endif()

enable_testing()
add_test(NAME banking_tests COMMAND banking_tests)
