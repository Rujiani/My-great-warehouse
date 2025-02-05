add_library(warehouse warehouse.hpp warehouse.cpp)
find_package(TBB REQUIRED)
target_link_libraries(warehouse product retail_product wholesale_product TBB::tbb)