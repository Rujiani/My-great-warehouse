#include "wholesale_product.hpp"
#include <format>
namespace mgw {

size_t wholesale_product::sell(size_t amount){
    if(quantity < amount * wholesale_size)
        throw std::invalid_argument("Error: Insufficient quantity");
    quantity -= amount * wholesale_size;
    return amount * wholesale_size * cost;
}

string wholesale_product::get_Info()const{
    return std::format("{} | Wholesale_size: {}", product::get_Info(), wholesale_size);
}

}