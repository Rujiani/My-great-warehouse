#include "retail_product.hpp"
#include "wholesale_product.hpp"

namespace mgw {
    wholesale_product retail_product::change_to_wholesale(size_t wholesale_size){
        return wholesale_product(quantity, cost, name, firm, country, wholesale_size);
    }

    retail_product wholesale_product::change_to_retail(float allowance){
        return retail_product(quantity, cost, name, firm, country, allowance);
    }
}