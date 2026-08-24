cmake_minimum_required(VERSION 3.20)
project(Raft LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(test client/src/test.cpp)
target_include_directories(test PUBLIC include/)
target_link_libraries(test PRIVATE Boost::boost Threads::Threads)
target_compile_definitions(test PRIVATE BOOST_ASIO_STANDALONE=0 BOOST_ERROR_CODE_HEADER_ONLY)