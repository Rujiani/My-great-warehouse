#ifndef PRODUCT_HPP_
#define PRODUCT_HPP_

#include <string>
#include <cstdlib>
#include <ostream>

using std::string;
using std::ostream;

// My Great Warehouse
namespace mgw {

/**
 * @class product
 * @brief Base class representing a product in the warehouse.
 */
class product {
protected:
    size_t quantity;  ///< Quantity of the product in stock.
    size_t cost;      ///< Cost per unit of the product.
    const string name;    ///< Name of the product.
    const string firm;    ///< Manufacturer of the product.
    const string country; ///< Country of the manufacturer.
    const string type;    ///< Type of product (wholesale/retail).

public:
    /**
     * @brief Default constructor.
     * @param tp Product type (default is an empty string).
     */
    product(string tp = "") : quantity{}, cost{}, name{}, firm{}, country{}, type{tp} {}

    /**
     * @brief Parameterized constructor.
     * @param tp Product type.
     * @param q Quantity of the product.
     * @param c Cost per unit.
     * @param n Name of the product.
     * @param f Manufacturer of the product.
     * @param cn Country of the manufacturer.
     */
    product(string tp, size_t q, size_t c, string &n, string &f, string &cn)
        : quantity(q), cost(c), name(n), firm(f), country(cn), type(tp) {}

    /**
     * @brief Gets the product type.
     * @return A constant reference to the product type.
     */
    const string& get_type() const { return type; }

    /**
     * @brief Gets the product name.
     * @return A constant reference to the product name.
     */
    const string& get_name() const { return name; }

    /**
     * @brief Gets the quantity of the product in stock.
     * @return The quantity of the product.
     */
    size_t get_quantity() const { return quantity; }

    /**
     * @brief Sets a new cost for the product.
     * @param new_cost The new cost of the product.
     */
    void set_cost(const size_t new_cost) { cost = new_cost; }

    /**
     * @brief Retrieves product information as a formatted string.
     * @return A string containing detailed product information.
     */
    virtual string get_Info() const;

    /**
     * @brief Prints product information to the specified output stream.
     * @param ost The output stream to print to.
     * @return A reference to the modified output stream.
     */
    ostream& print_Info(ostream &ost) {
        ost << get_Info();
        return ost;
    }

    /**
     * @brief Processes a sale of the product.
     * @param amount The amount of product to sell.
     * @return The total sale cost.
     * @throws std::runtime_error If the requested quantity exceeds available stock.
     */
    virtual size_t sell(size_t amount) = 0;

    /**
     * @brief Adds a specified amount of product to the stock.
     * @param amount The amount of product to add.
     */
    virtual void add_to_storage(size_t amount) = 0;
};

} // namespace mgw

#endif // PRODUCT_HPP_
