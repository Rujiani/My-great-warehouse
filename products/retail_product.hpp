#ifndef RETAIL_PRODUCT_HPP_
#define RETAIL_PRODUCT_HPP_

#include "product.hpp"
#include <stdexcept>

namespace mgw {

class retail_product : public product{
    size_t allowance;
    public:

    retail_product():product{"retail_product"}, allowance{}{}
    
    retail_product(size_t q, float c, string n, string f, string cn, ssize_t a):
    product("retail_product", q, c, n, f, cn), allowance(a){
        if(allowance > 100)
            throw std::invalid_argument("Error: Allowance сan't exceed one hundred");
    }

    void set_allowance(size_t newAl){
        if(newAl > 100) 
            throw std::invalid_argument("Error: Allowance сan't exceed one hundred");
        allowance = newAl;
    }

    size_t get_allowance() const{return allowance;}

    void add_to_storage(size_t num) override{
        quantity += num;
    }

    float sell(size_t num) override;

};

}

#endif