#include "warehouse.hpp"
#include "../products/wholesale_product.hpp"
#include "../products/retail_product.hpp"
#include <stdexcept>
#include <algorithm>
#include <execution>

namespace mgw {

void warehouse::register_product(const string &cipher, const product_components &pr){
    auto pos = product_table.find(cipher);
    if(pos != product_table.end()){
        //add product check
        product_table[cipher]->add_to_storage(pr.quantity);
    }
    else if(pr.type == "wholesale"){
        product_table[cipher] = std::make_shared<wholesale_product>(wholesale_product(
            pr.quantity, pr.cost, pr.name, pr.firm, pr.country, pr.num
        ));
    }
    else if(pr.type == "retail"){
        product_table[cipher] = std::make_shared<retail_product>(retail_product(
            pr.quantity, pr.cost, pr.name, pr.firm, pr.country, pr.num
        ));
    }
    else{
        throw std::invalid_argument("Error: Incorrect product type");
    }
}

size_t warehouse::sell_product(const string &cipher, const size_t num) {
	auto pos = product_table.find(cipher);
	if (pos != product_table.end())
		return (*pos).second->sell(num);
	else
		throw std::invalid_argument("Error: No such product");
}

string warehouse::get_report()const{
    string result;
    for(auto &i : product_table){
        result += (i.second->get_Info() + '\n');
    }
    return result;
}

string warehouse::missing_products()const{
    string result;
    std::for_each(std::execution::par, product_table.begin(), product_table.end(), [&result](auto &x) {
        if(x.second->get_quantity() == 0){
            result += (x.second->get_name() + '\n');
        }
    });
    return result;
}

}