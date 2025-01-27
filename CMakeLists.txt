cmake_minimum_required(3.15.0)

add_subdirectory(test)
add_subdirectory(products)

target_link_libraries(tests product)

