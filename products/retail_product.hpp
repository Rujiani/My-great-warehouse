#ifndef RETAIL_PRODUCT_HPP_
#define RETAIL_PRODUCT_HPP_

#include "product.hpp"
#include <stdexcept>

namespace mgw {

class wholesale_product;

class retail_product : public product{
    size_t allowance;
    public:

    retail_product():product{"retail"}, allowance{}{}
    
    retail_product(size_t q, size_t c, string n, string f, string cn, size_t a):
    product("retail", q, c, n, f, cn), allowance(a){
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

    size_t sell(size_t num) override;

    wholesale_product change_to_wholesale(size_t wholesale_size);

    string get_Info()const override;
};

}

#endif