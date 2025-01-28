add_library(product product.hpp product.cpp)

add_library(retail_product retail_product.hpp retail_product.cpp convert.cpp)
add_library(wholesale_product wholesale_product.hpp wholesale_product.cpp convert.cpp)

target_link_libraries(retail_product product)
target_link_libraries(wholesale_product product)
