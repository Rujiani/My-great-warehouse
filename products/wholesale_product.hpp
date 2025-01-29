#ifndef WHOLESALE_PRODUCT_HPP_
#define WHOLESALE_PRODUCT_HPP_

#include "product.hpp"

namespace mgw {

class retail_product;

/**
 * @class wholesale_product
 * @brief Represents a wholesale product in the warehouse.
 * 
 * Inherits from the `product` class and includes an additional field for wholesale batch size.
 */
class wholesale_product : public product {
private:
    size_t wholesale_size; ///< The number of units in a wholesale batch.

public:
    /**
     * @brief Default constructor.
     * 
     * Initializes a wholesale product with zero quantity, cost, and batch size.
     */
    wholesale_product() : product("wholesale"), wholesale_size{} {}

    /**
     * @brief Parameterized constructor.
     * 
     * @param q Quantity of wholesale batches in stock.
     * @param c Cost per unit.
     * @param n Product name.
     * @param f Manufacturer name.
     * @param cn Country of manufacture.
     * @param ws Wholesale batch size (number of items per batch).
     */
    wholesale_product(size_t q, size_t c, string n, string f, string cn, size_t ws) 
        : product("wholesale", q, c, n, f, cn), wholesale_size(ws) {}

    /**
     * @brief Sets the wholesale batch size.
     * 
     * @param num The new batch size.
     */
    void set_wholesale_size(size_t num) { wholesale_size = num; }

    /**
     * @brief Gets the current wholesale batch size.
     * @return The current batch size.
     */
    size_t get_wholesale_size() const { return wholesale_size; }

    /**
     * @brief Processes a sale of the wholesale product.
     * 
     * @param amount The number of wholesale batches to sell.
     * @return The total sale price.
     * @throws std::runtime_error If the requested batch quantity exceeds available stock.
     */
    size_t sell(size_t amount) override;

    /**
     * @brief Adds stock to the storage.
     * 
     * @param amount The number of additional wholesale batches.
     * The total quantity added is `amount * wholesale_size`.
     */
    void add_to_storage(size_t amount) override { quantity += amount * wholesale_size; }

    /**
     * @brief Converts the wholesale product into a retail product.
     * 
     * @param allowance The markup percentage for retail conversion.
     * @return A new `retail_product` instance.
     */
    retail_product change_to_retail(size_t allowance);

    /**
     * @brief Retrieves detailed information about the wholesale product.
     * @return A formatted string containing product details.
     */
    string get_Info() const override;
};

} // namespace mgw

#endif // WHOLESALE_PRODUCT_HPP_
