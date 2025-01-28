#include "retail_product.hpp"
#include <stdexcept>

namespace mgw {

size_t retail_product::sell(size_t num){
    if(get_quantity() < num)
        throw std::invalid_argument("Error: Insufficient quantity");
    quantity -= num;
    return num * static_cast<size_t>(static_cast<float>(cost) * allowance * 0.01);
}

}