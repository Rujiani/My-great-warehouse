#include "retail_product.hpp"
#include <stdexcept>

namespace mgw {

float retail_product::sell(size_t num){
    if(get_quantity() < num)
        throw std::invalid_argument("Error: Insufficient quantity");
    quantity -= num;
    return static_cast<float>(num) * (cost * allowance * 0.01);
}

}