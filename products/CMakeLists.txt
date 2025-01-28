add_library(product product.hpp product.cpp)

add_library(retail_product retail_product.hpp retail_product.cpp)

target_link_libraries(retail_product product)