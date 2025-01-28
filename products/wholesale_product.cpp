#include "wholesale_product.hpp"

namespace mgw {

size_t wholesale_product::sell(size_t amount){
    if(quantity < amount * wholesale_size)
        throw std::invalid_argument("Error: Insufficient quantity");
    quantity -= amount * wholesale_size;
    return amount * wholesale_size * cost;
}

}