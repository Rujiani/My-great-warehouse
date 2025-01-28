find_package(Catch2)

add_executable(tests test.cpp ../products/product.cpp ../products/retail_product.cpp ../products/wholesale_product.cpp ../logic/warehouse.cpp)
target_link_libraries(tests Catch2::Catch2WithMain gcov)

target_compile_options(tests PRIVATE --coverage -std=c++20 -Wall -Wextra -Wconversion)
