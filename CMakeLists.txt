cmake_minimum_required(VERSION 3.15.0)

project(warehouse VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(products)
add_subdirectory(test)

target_link_libraries(tests retail_product)

