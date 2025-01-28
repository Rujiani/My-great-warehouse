#include "product.hpp"
#include <format>
namespace mgw {
    string product::get_Info()const{
        return std::format(
            "[Name: {}] | Quantity: {} | Manufacturer: {} ({}) | Price: {} | Type: {}_product",
            name, quantity, firm, country, cost, type);
    }
}