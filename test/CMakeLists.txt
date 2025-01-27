find_package(Catch2)

add_executable(tests test.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -std=c++20")
