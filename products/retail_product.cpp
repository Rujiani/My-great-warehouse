#include "retail_product.hpp"
#include <format>
#include <stdexcept>

namespace mgw {

size_t retail_product::sell(size_t num){
    if(get_quantity() < num)
        throw std::invalid_argument("Error: Insufficient quantity");
    quantity -= num;
    return num * static_cast<size_t>(static_cast<float>(cost) * 
                            static_cast<float>(allowance) * 0.01);
}

string retail_product::get_Info()const{
    return std::format("{} | Allowance: {}%", product::get_Info(), allowance);
}

}