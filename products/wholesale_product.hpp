#ifndef WHOLESALE_PRODUCT_HPP_
#define WHOLESALE_PRODUCT_HPP_

#include "product.hpp"
namespace mgw {

class retail_product;

class wholesale_product : public product{
    private:
    size_t wholesale_size;
    public:
    wholesale_product():product("wholesale"), wholesale_size{}{}

    wholesale_product(size_t q, size_t c, string n, string f, string cn, size_t ws):
    product("wholesale", q, c, n, f, cn), wholesale_size(ws){}

    void set_wholesale_size(size_t num){wholesale_size = num;}
    size_t get_wholesale_size()const{return wholesale_size;}

    size_t sell(size_t amount) override;
    void add_to_storage(size_t amount) override{quantity += amount * wholesale_size;}

    retail_product change_to_retail(size_t allowance);

    string get_Info()const override;
};

}

#endif