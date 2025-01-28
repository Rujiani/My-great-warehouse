add_library(warehouse warehouse.hpp warehouse.cpp)

target_link_libraries(warehouse product retail_product wholesale_product)