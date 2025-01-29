#ifndef WAREHOUSE_HPP_
#define WAREHOUSE_HPP_

#include "../products/product.hpp"
#include <memory>
#include <string>
#include <unordered_map>

using std::string;

namespace mgw {

/**
 * @struct product_components
 * @brief Represents the components needed to register a product.
 */
struct product_components {
    size_t quantity; ///< Quantity of the product.
    size_t cost;     ///< Cost per unit.
    size_t num;      ///< Additional identifier or batch size.
    string name;     ///< Name of the product.
    string firm;     ///< Manufacturer name.
    string country;  ///< Country of manufacture.
    string type;     ///< Product type (wholesale/retail).
};

/**
 * @class warehouse
 * @brief Represents a warehouse that manages a collection of products.
 */
class warehouse {
    std::unordered_map<string, std::shared_ptr<product>> product_table; ///< Storage for products, mapped by their cipher.

public:
    /**
     * @brief Default constructor.
     * 
     * Initializes an empty warehouse.
     */
    warehouse() = default;

    /**
     * @brief Registers a new product in the warehouse.
     * 
     * If a product with the given cipher already exists, its details are updated.
     * 
     * @param cipher Unique identifier for the product.
     * @param pr Struct containing product details.
     */
    void register_product(const string &cipher, const product_components &pr);

    /**
     * @brief Processes the sale of a product.
     * 
     * @param cipher Unique identifier of the product to be sold.
     * @param num The number of units (or wholesale batches) to sell.
     * @return The total sale price.
     * @throws std::runtime_error If the product does not exist or there is insufficient stock.
     */
    size_t sell_product(const string &cipher, const size_t num);

    /**
     * @brief Generates a report containing all available products in the warehouse.
     * 
     * @return A formatted string listing all products and their details.
     */
    string get_report() const;

    /**
     * @brief Lists all products that are out of stock.
     * 
     * @return A formatted string containing the names of products with zero quantity.
     */
    string missing_products() const;
};

} // namespace mgw

#endif // WAREHOUSE_HPP_
