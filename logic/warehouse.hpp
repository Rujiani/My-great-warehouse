#ifndef WAREHOUSE_HPP_
#define WAREHOUSE_HPP_

#include "../products/product.hpp"
#include <memory>
#include <string>
#include <unordered_map>

using std::string;
namespace mgw {

struct product_components {
	size_t quantity, cost, num;
	string name, firm, country, type;
};

class warehouse {
  	std::unordered_map<string, std::shared_ptr<product>> product_table;

public:
  	warehouse() = default;

  	void register_product(const string &cipher, const product_components &pr);

	size_t sell_product(const string &cipher, const size_t num);

	string get_report()const;

	string missing_products()const;
};

} // namespace mgw

#endif