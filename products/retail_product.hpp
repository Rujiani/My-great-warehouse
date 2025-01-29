#ifndef RETAIL_PRODUCT_HPP_
#define RETAIL_PRODUCT_HPP_

#include "product.hpp"
#include <stdexcept>

namespace mgw {

class wholesale_product;

/**
 * @class retail_product
 * @brief Represents a retail product in the warehouse.
 * 
 * Inherits from the `product` class and includes an additional 
 * allowance percentage that affects the retail price.
 */
class retail_product : public product {
    size_t allowance; ///< The retail markup percentage.

public:
    /**
     * @brief Default constructor.
     * 
     * Initializes a retail product with zero quantity, cost, and allowance.
     */
    retail_product() : product{"retail"}, allowance{} {}

    /**
     * @brief Parameterized constructor.
     * 
     * @param q Quantity of the product.
     * @param c Cost per unit.
     * @param n Product name.
     * @param f Manufacturer name.
     * @param cn Country of manufacture.
     * @param a Allowance (markup percentage).
     * @throws std::invalid_argument If the allowance exceeds 100%.
     */
    retail_product(size_t q, size_t c, string n, string f, string cn, size_t a) 
        : product("retail", q, c, n, f, cn), allowance(a) {
        if (allowance > 100)
            throw std::invalid_argument("Error: Allowance can't exceed one hundred");
    }

    /**
     * @brief Sets a new allowance (markup percentage).
     * 
     * @param newAl New allowance value.
     * @throws std::invalid_argument If the new allowance exceeds 100%.
     */
    void set_allowance(size_t newAl) {
        if (newAl > 100)
            throw std::invalid_argument("Error: Allowance can't exceed one hundred");
        allowance = newAl;
    }

    /**
     * @brief Gets the current allowance percentage.
     * @return The current allowance value.
     */
    size_t get_allowance() const { return allowance; }

    /**
     * @brief Adds stock to the storage.
     * 
     * @param num The number of additional units to add.
     */
    void add_to_storage(size_t num) override {
        quantity += num;
    }

    /**
     * @brief Processes a sale of the retail product.
     * 
     * @param num The number of units to sell.
     * @return The total sale price including the markup.
     * @throws std::runtime_error If the requested quantity exceeds available stock.
     */
    size_t sell(size_t num) override;

    /**
     * @brief Converts the retail product into a wholesale product.
     * 
     * @param wholesale_size The batch size for the wholesale conversion.
     * @return A new `wholesale_product` instance.
     */
    wholesale_product change_to_wholesale(size_t wholesale_size);

    /**
     * @brief Retrieves detailed information about the retail product.
     * @return A formatted string containing product details.
     */
    string get_Info() const override;
};

} // namespace mgw

#endif // RETAIL_PRODUCT_HPP_
