#include "product.hpp"
#include <format>

namespace mgw {
    ostream& product::print_Info(ostream &ost){
        ost << std::format(
            "[Name: {}] | Quantity: {} | Manufacturer: {} ({}) | Price: {} | Type: {}",
            name, quantity, firm, country, cost, type);
        return ost;
    }
}