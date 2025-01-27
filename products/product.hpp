#ifndef PRODUCT_HPP_
#define PRODUCT_HPP_

#include <string>
#include <cstdlib>
#include <ostream>

using std::string;
using std::ostream;

//my great warehouse
namespace mgw {

class product{
    private:
    size_t quantity, cost;
    string name, firm, country, type;
    public:
    product():quantity(0), cost(0), name{}, firm{}, country{}, type{}{};
    product(size_t q, size_t c, string n, string f, string cn, string tp) : 
        quantity(q), cost(c), name(n), firm(f), country(cn), type(tp){}

    string get_type() const {return type;}
    size_t get_quantity() const {return quantity;}
    void set_cost(const size_t new_cost) {cost = new_cost;}

    virtual ostream& print_Info(ostream &ost);

    virtual size_t sell(size_t amount) = 0;
    virtual size_t add_to_storage(size_t amount) = 0;
};
}

#endif