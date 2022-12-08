cmake_minimum_required(VERSION 3.14)

project(OrderMatchingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Library
file(GLOB_RECURSE SRC_FILES "OrderMatcher/*.hh" "OrderMatcher/*.cc")
add_library(OrderMatcher ${SRC_FILES})
#set_target_properties(OrderMatcher PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}" FOLDER "libraries")
target_include_directories(OrderMatcher PUBLIC "${PROJECT_BINARY_DIR}")


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(orderbook test/orderbook_test.cc)

target_link_libraries(orderbook GTest::gtest_main)
target_link_libraries(orderbook OrderMatcher)


include(GoogleTest)
gtest_discover_tests(orderbook)


# @Zhejian modify to add executable for main integrator function
# cmake_minimum_required(VERSION 3.16)
# project(Order_Matching_Engine)

# set(CMAKE_CXX_STANDARD 20)

# #add_executable(Order_Matching_Engine main.cpp)

# add_executable(Order_Test order_test.cc order.cc order.hh)

# add_executable(Order_Book_Test orderbook_test.cc
#         order.cc order.hh
#         orderbook.cc orderbook.hh
#         central_order_book.cc central_order_book.hh
#         ordermatching.cc)
