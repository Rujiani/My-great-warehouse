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
    protected:
    size_t quantity;
    size_t cost;
    const string name, firm, country, type;
    public:
    product(string tp = ""):quantity{}, cost{}, name{}, firm{}, country{}, type{tp}{};
    product(string tp, size_t q, float c, string n, string f, string cn) : 
        quantity(q), cost(c), name(n), firm(f), country(cn), type(tp){}

    const string& get_type() const {return type;}
    size_t get_quantity() const {return quantity;}
    void set_cost(const size_t new_cost) {cost = new_cost;}

    virtual ostream& print_Info(ostream &ost);

    virtual size_t sell(size_t amount) = 0;
    virtual void add_to_storage(size_t amount) = 0;
};
}

#endif