#include <iterator>
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../products/retail_product.hpp"

TEST_CASE("Default constructor") {
    mgw::retail_product rp;
    REQUIRE(rp.get_quantity() == 0);
    REQUIRE(rp.get_allowance() == 0);
    REQUIRE(rp.get_type() == "retail");
}

TEST_CASE("Constructor with arguments") {
    SECTION("Valid allowance") {
        mgw::retail_product rp(10, 100, "Widget", "ACME", "USA", 20);
        REQUIRE(rp.get_quantity() == 10);
        REQUIRE(rp.get_allowance() == 20);
        REQUIRE(rp.get_type() == "retail");
    }
    SECTION("Allowance > 100 throws") {
        REQUIRE_THROWS_AS(
            mgw::retail_product(10, 100, "Widget", "ACME", "USA", 101),
            std::invalid_argument
        );
    }
}

TEST_CASE("set_allowance") {
    mgw::retail_product rp(10, 100, "Widget", "ACME", "USA", 0);
    SECTION("Valid allowance") {
        REQUIRE_NOTHROW(rp.set_allowance(50));
        REQUIRE(rp.get_allowance() == 50);
    }
    SECTION("Allowance > 100 throws") {
        REQUIRE_THROWS_AS(rp.set_allowance(150), std::invalid_argument);
    }
}

TEST_CASE("add_to_storage") {
    mgw::retail_product rp(5, 100, "Widget", "ACME", "USA", 10);
    rp.add_to_storage(10);
    REQUIRE(rp.get_quantity() == 15);
}

TEST_CASE("sell") {
    mgw::retail_product rp(10, 100, "Widget", "ACME", "USA", 20);
    SECTION("Successful sell") {
        size_t revenue = rp.sell(3);
        REQUIRE(revenue == Catch::Approx(60.0f));
        REQUIRE(rp.get_quantity() == 7);
    }
    SECTION("Insufficient quantity throws") {
        REQUIRE_THROWS_AS(rp.sell(11), std::invalid_argument);
        REQUIRE(rp.get_quantity() == 10);
    }
}

TEST_CASE("print_Info") {
    mgw::retail_product rp(10, 100, "Widget", "ACME", "USA", 20);
    std::ostringstream oss;
    rp.print_Info(oss);
    REQUIRE(
        oss.str() == "[Name: Widget] | Quantity: 10 | Manufacturer: ACME (USA) | Price: 100 | Type: retail_product | Allowance: 20%"
    );
}

#include "../products/wholesale_product.hpp"

TEST_CASE("WholesaleProduct: Default constructor") {
    mgw::wholesale_product wp;
    REQUIRE(wp.get_quantity() == 0);
    REQUIRE(wp.get_type() == "wholesale");
}

TEST_CASE("WholesaleProduct: Constructor with arguments") {
    mgw::wholesale_product wp(100, 10.0f, "BulkWidget", "ACME", "USA", 5);
    REQUIRE(wp.get_quantity() == 100);
    REQUIRE(wp.get_type() == "wholesale");
}

TEST_CASE("WholesaleProduct: sell") {
    mgw::wholesale_product wp(100, 10.0f, "BulkWidget", "ACME", "USA", 5);
    SECTION("Valid sell") {
        size_t revenue = wp.sell(10); 
        REQUIRE(revenue == Catch::Approx(500.0f));
        REQUIRE(wp.get_quantity() == 50); // 100 - (10 * 5) = 50
    }
    SECTION("Insufficient quantity") {
        REQUIRE_THROWS_AS(wp.sell(25), std::invalid_argument);
        REQUIRE(wp.get_quantity() == 100); 
    }
}

TEST_CASE("WholesaleProduct: add_to_storage") {
    mgw::wholesale_product wp(20, 10.0f, "BulkWidget", "ACME", "USA", 5);
    wp.add_to_storage(3); 
    REQUIRE(wp.get_quantity() == 35);
}

TEST_CASE("Retail to Wholesale Conversion") {
    // Create a retail product
    mgw::retail_product rp(100, 10.0f, "Widget", "ACME", "USA", 20);

    SECTION("Valid conversion to wholesale") {
        size_t wholesale_size = 5;
        mgw::wholesale_product wp = rp.change_to_wholesale(wholesale_size);

        // Check wholesale product properties
        REQUIRE(wp.get_quantity() == rp.get_quantity());
        REQUIRE(wp.get_type() == "wholesale");

        // Wholesale-specific check
        REQUIRE(wp.get_wholesale_size() == wholesale_size); // Assuming there's a getter for wholesale_size
    }
}

TEST_CASE("Wholesale to Retail Conversion") {
    // Create a wholesale product
    mgw::wholesale_product wp(50, 15.0f, "BulkWidget", "ACME", "USA", 10);

    SECTION("Valid conversion to retail") {
        size_t allowance = 25;
        mgw::retail_product rp = wp.change_to_retail(allowance);

        // Check retail product properties
        REQUIRE(rp.get_quantity() == wp.get_quantity());
        REQUIRE(rp.get_type() == "retail");

        // Retail-specific check
        REQUIRE(rp.get_allowance() == allowance); // Assuming there's a getter for allowance
    }
}

#include "../logic/warehouse.hpp"

TEST_CASE("Warehouse: Register product") {
    mgw::warehouse wh;

    SECTION("Register new wholesale product") {
        mgw::product_components pc{
            10,     // quantity
            100,    // cost
            5,      // num (wholesale size)
            "Bolt", // name
            "ACME", // firm
            "USA",  // country
            "wholesale"
        };
        std::string cipher = "WH-123";

        // Should not throw for a new wholesale product
        REQUIRE_NOTHROW(wh.register_product(cipher, pc));
    }

    SECTION("Register new retail product") {
        mgw::product_components pc{
            5,     // quantity
            50,    // cost
            20,    // num (allowance)
            "Nut", // name
            "ACME",
            "USA",
            "retail"
        };
        std::string cipher = "RT-999";

        // Should not throw for a new retail product
        REQUIRE_NOTHROW(wh.register_product(cipher, pc));
    }

    SECTION("Re-register existing product adds to storage") {
        // First registration
        mgw::product_components pc1{
            10,       // quantity
            100,      // cost
            5,        // num
            "Bolt", 
            "ACME",
            "USA",
            "wholesale"
        };
        std::string cipher = "WH-123";
        REQUIRE_NOTHROW(wh.register_product(cipher, pc1));

        // Second registration with the same cipher
        mgw::product_components pc2{
            7,        // quantity to add
            200,      // new cost (ignored by add_to_storage logic)
            10,       // new num (ignored by add_to_storage logic)
            "Bolt2", 
            "ACME2",
            "USA2",
            "wholesale"
        };
        // Should not throw; should add 7 more items to existing product.
        REQUIRE_NOTHROW(wh.register_product(cipher, pc2));
    }

    SECTION("Invalid product type throws") {
        mgw::product_components pc{
            10,
            100,
            1,
            "ItemX",
            "TestFirm",
            "Russia",
            "unknown_type" // invalid
        };
        std::string cipher = "BAD-001";

        // Should throw std::invalid_argument
        REQUIRE_THROWS_AS(wh.register_product(cipher, pc), std::invalid_argument);
    }
}

TEST_CASE("sell_product: Retail product - successful sell") {
    mgw::warehouse wh;
    mgw::product_components comp;
    comp.type = "retail";
    comp.quantity = 10;
    comp.cost = 100;
    comp.name = "Widget";
    comp.firm = "ACME";
    comp.country = "USA";
    comp.num = 20;
    wh.register_product("ABC123", comp);
    size_t revenue = wh.sell_product("ABC123", 2);
    REQUIRE(revenue == 40);
}

TEST_CASE("sell_product: Wholesale product - successful sell") {
    mgw::warehouse wh;
    mgw::product_components comp;
    comp.type = "wholesale";
    comp.quantity = 50;
    comp.cost = 10;
    comp.name = "BulkWidget";
    comp.firm = "BulkFirm";
    comp.country = "China";
    comp.num = 5;
    wh.register_product("BULK5", comp);
    size_t revenue = wh.sell_product("BULK5", 3);
    REQUIRE(revenue == 150);
}

TEST_CASE("sell_product: Product not found") {
    mgw::warehouse wh;
    REQUIRE_THROWS_AS(wh.sell_product("MISSING", 1), std::invalid_argument);
}

TEST_CASE("sell_product: Insufficient quantity") {
    mgw::warehouse wh;
    mgw::product_components comp;
    comp.type = "retail";
    comp.quantity = 1;
    comp.cost = 50;
    comp.name = "LowStockItem";
    comp.firm = "MiniCo";
    comp.country = "USA";
    comp.num = 10;
    wh.register_product("LOW", comp);
    REQUIRE_THROWS_AS(wh.sell_product("LOW", 3), std::invalid_argument);
}

TEST_CASE("Warehouse operations (integer money)") {
    mgw::warehouse wh;

    mgw::product_components pc_wh;
    pc_wh.quantity = 10;
    pc_wh.cost = 100;
    pc_wh.num = 5;
    pc_wh.name = "BulkWidget";
    pc_wh.firm = "ACME";
    pc_wh.country = "USA";
    pc_wh.type = "wholesale";

    mgw::product_components pc_r;
    pc_r.quantity = 5;
    pc_r.cost = 50;
    pc_r.num = 20;
    pc_r.name = "Widget";
    pc_r.firm = "SomeFirm";
    pc_r.country = "USA";
    pc_r.type = "retail";

    SECTION("Register wholesale") {
        REQUIRE_NOTHROW(wh.register_product("W1", pc_wh));
        REQUIRE(wh.sell_product("W1", 1) == (1 * pc_wh.cost * pc_wh.num));
    }

    SECTION("Register retail") {
        REQUIRE_NOTHROW(wh.register_product("R1", pc_r));
        REQUIRE(wh.sell_product("R1", 2) == ((2 * pc_r.cost * pc_r.num) / 100));
    }

    SECTION("Re-register same product") {
        wh.register_product("R2", pc_r);
        mgw::product_components pc_r2 = pc_r;
        pc_r2.quantity = 3;
        REQUIRE_NOTHROW(wh.register_product("R2", pc_r2));
        REQUIRE(wh.sell_product("R2", 8) > 0);
        REQUIRE_THROWS_AS(wh.sell_product("R2", 1), std::invalid_argument);
    }

    SECTION("Invalid product type") {
        mgw::product_components invalid_type = pc_r;
        invalid_type.type = "unknown";
        REQUIRE_THROWS_AS(wh.register_product("X1", invalid_type), std::invalid_argument);
    }

    SECTION("Selling unknown product") {
        REQUIRE_THROWS_AS(wh.sell_product("Unknown", 5), std::invalid_argument);
    }

    SECTION("get_report") {
        wh.register_product("W1", pc_wh);
        wh.register_product("R1", pc_r);
        std::string report = wh.get_report();
        REQUIRE(report.find("BulkWidget") != std::string::npos);
        REQUIRE(report.find("Widget") != std::string::npos);
    }

    SECTION("missing_products") {
        wh.register_product("R1", pc_r);
        mgw::product_components pc_zero = pc_r;
        pc_zero.quantity = 0;
        pc_zero.name = "Emptywidget";
        wh.register_product("R2", pc_zero);
        std::string missing = wh.missing_products();
        REQUIRE(missing.find("Emptywidget") != std::string::npos);
        REQUIRE(missing.find("Widget") == std::string::npos);
    }
}

#include "../container/unordered_map.hpp"
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using mgc::HashMap;

// ----------------------------------------------------------------------------
// Static assertions checking that the iterators satisfy the bidirectional concept.
// (This is analogous to the sample using std::contiguous_iterator for matrix iterators.)
static_assert(std::bidirectional_iterator<typename HashMap<int, int>::iterator>,
              "HashMap::iterator does not satisfy bidirectional_iterator");
static_assert(std::bidirectional_iterator<typename HashMap<int, int>::const_iterator>,
              "HashMap::const_iterator does not satisfy bidirectional_iterator");
// ----------------------------------------------------------------------------

TEST_CASE("Insertion and Lookup", "[HashMap]") {
    HashMap<int, std::string> map;
    REQUIRE(map.size() == 0);

    // Insert some elements
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    REQUIRE(map.size() == 3);

    // Lookup via find()
    auto it = map.find(2);
    REQUIRE(it != map.end());
    REQUIRE(it->second == "two");

    // Check operator[]: if key not found, a new element is created with default value.
    map[4] = "four";
    REQUIRE(map.size() == 4);
    REQUIRE(map.find(4) != map.end());
    REQUIRE(map[4] == "four");

    // Update existing key using insert.
    map.insert(2, "TWO");
    REQUIRE(map.find(2)->second == "TWO");
}

TEST_CASE("Erase", "[HashMap]") {
    HashMap<int, std::string> map;
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    REQUIRE(map.size() == 3);

    // Remove an existing key.
    map.erase(2);
    REQUIRE(map.size() == 2);
    REQUIRE(map.find(2) == map.end());

    // Attempting to erase a non-existent key should leave the size unchanged.
    map.erase(42);
    REQUIRE(map.size() == 2);
}

TEST_CASE("Clear", "[HashMap]") {
    HashMap<int, int> map;
    for (int i = 0; i < 10; ++i) {
        map.insert(i, i * i);
    }
    REQUIRE(map.size() == 10);
    map.clear();
    REQUIRE(map.size() == 0);
    for (int i = 0; i < 10; ++i) {
        REQUIRE(map.find(i) == map.end());
    }
}

TEST_CASE("Rehash", "[HashMap]") {
    // Create a map with a small initial capacity and a low load factor threshold,
    // so that rehashing is triggered.
    HashMap<int, int> map(5, 0.75);
    for (int i = 0; i < 20; ++i) {
        map.insert(i, i + 10);
    }
    REQUIRE(map.size() == 20);
    for (int i = 0; i < 20; ++i) {
        auto it = map.find(i);
        REQUIRE(it != map.end());
        REQUIRE(it->second == i + 10);
    }

    // Manually rehash and verify that all elements remain intact.
    map.rehash(50);
    for (int i = 0; i < 20; ++i) {
        auto it = map.find(i);
        REQUIRE(it != map.end());
        REQUIRE(it->second == i + 10);
    }
}

TEST_CASE("Iterators", "[HashMap]") {
    HashMap<int, std::string> map;
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    // Test forward iteration.
    std::vector<int> keys;
    for (auto it = map.begin(); it != map.end(); ++it) {
        keys.push_back(it->first);
    }
    REQUIRE(keys.size() == 3);
    // Expected order (insertion order): 1, 2, 3.
    REQUIRE(keys[0] == 1);
    REQUIRE(keys[1] == 2);
    REQUIRE(keys[2] == 3);

    // Test reverse iteration using the pre-decrement operator.
    std::vector<int> rev_keys;
    for (auto it = map.end(); it != map.begin(); ) {
        --it;
        rev_keys.push_back(it->first);
    }
    REQUIRE(rev_keys.size() == 3);
    REQUIRE(rev_keys[0] == 3);
    REQUIRE(rev_keys[1] == 2);
    REQUIRE(rev_keys[2] == 1);

    // Test standard algorithm std::find_if to search for an element.
    auto found = std::find_if(map.begin(), map.end(),
                              [](const std::pair<const int, std::string>& p) {
                                  return p.second == "two";
                              });
    REQUIRE(found != map.end());
    REQUIRE(found->first == 2);
}

TEST_CASE("Copy Constructor and Copy Assignment", "[HashMap]") {
    HashMap<int, int> map;
    for (int i = 0; i < 5; ++i) {
        map.insert(i, i * 10);
    }
    REQUIRE(map.size() == 5);

    // Test copy constructor.
    HashMap<int, int> map_copy(map);
    REQUIRE(map_copy.size() == 5);
    for (int i = 0; i < 5; ++i) {
        auto it = map_copy.find(i);
        REQUIRE(it != map_copy.end());
        REQUIRE(it->second == i * 10);
    }

    // Test copy assignment.
    HashMap<int, int> map_assigned;
    map_assigned = map;
    REQUIRE(map_assigned.size() == 5);
    for (int i = 0; i < 5; ++i) {
        auto it = map_assigned.find(i);
        REQUIRE(it != map_assigned.end());
        REQUIRE(it->second == i * 10);
    }
}

TEST_CASE("Move Constructor and Move Assignment", "[HashMap]") {
    // Prepare a map for moving.
    HashMap<int, int> map;
    for (int i = 0; i < 5; ++i) {
        map.insert(i, i + 1);
    }
    REQUIRE(map.size() == 5);

    // Test move constructor.
    HashMap<int, int> moved_map(std::move(map));
    REQUIRE(moved_map.size() == 5);
    for (int i = 0; i < 5; ++i) {
        auto it = moved_map.find(i);
        REQUIRE(it != moved_map.end());
        REQUIRE(it->second == i + 1);
    }
    // After moving, the original map should be empty.
    REQUIRE(map.size() == 0);

    // Test move assignment.
    HashMap<int, int> map2;
    for (int i = 10; i < 15; ++i) {
        map2.insert(i, i + 2);
    }
    REQUIRE(map2.size() == 5);
    HashMap<int, int> moved_assign;
    moved_assign = std::move(map2);
    REQUIRE(moved_assign.size() == 5);
    for (int i = 10; i < 15; ++i) {
        auto it = moved_assign.find(i);
        REQUIRE(it != moved_assign.end());
        REQUIRE(it->second == i + 2);
    }
    REQUIRE(map2.size() == 0);
}

TEST_CASE("Operator[] Default Construction", "[HashMap]") {
    HashMap<std::string, int> map;
    // Accessing a non-existent key should create a new element with a default value.
    int val = map["new"];
    REQUIRE(val == 0);
    map["new"] = 42;
    REQUIRE(map["new"] == 42);
}

TEST_CASE("Swap", "[HashMap]") {
    HashMap<int, int> map1;
    HashMap<int, int> map2;
    map1.insert(1, 100);
    map1.insert(2, 200);
    map2.insert(3, 300);

    map1.swap(map2);

    // After swap, map1 should contain the elements originally in map2.
    REQUIRE(map1.size() == 1);
    auto it = map1.find(3);
    REQUIRE(it != map1.end());
    REQUIRE(it->second == 300);

    // And map2 should contain the elements originally in map1.
    REQUIRE(map2.size() == 2);
    it = map2.find(1);
    REQUIRE(it != map2.end());
    REQUIRE(it->second == 100);
    it = map2.find(2);
    REQUIRE(it != map2.end());
    REQUIRE(it->second == 200);
}

TEST_CASE("Standard Algorithms", "[HashMap]") {
    HashMap<int, int> map;
    for (int i = 1; i <= 5; ++i) {
        map.insert(i, i);
    }

    // Sum values using std::accumulate.
    int sum = std::accumulate(map.begin(), map.end(), 0,
                              [](int acc, const std::pair<const int, int>& p) {
                                  return acc + p.second;
                              });
    REQUIRE(sum == 15);

    // Instead of reverse_copy (which requires assignability between mismatched types),
    // use std::transform with reverse iterators to extract the keys.
    std::vector<int> keys;
    std::transform(std::make_reverse_iterator(map.end()),
                   std::make_reverse_iterator(map.begin()),
                   std::back_inserter(keys),
                   [](const std::pair<const int, int>& p) { return p.first; });
    // If the insertion order is: 1, 2, 3, 4, 5 then the reversed keys should be: 5, 4, 3, 2, 1.
    REQUIRE(keys.size() == 5);
    REQUIRE(keys[0] == 5);
    REQUIRE(keys[1] == 4);
    REQUIRE(keys[2] == 3);
    REQUIRE(keys[3] == 2);
    REQUIRE(keys[4] == 1);
}

TEST_CASE("Const Iterator Functionality", "[HashMap]") {
    // Create a HashMap and insert some elements.
    HashMap<int, std::string> map;
    map.insert(10, "ten");
    map.insert(20, "twenty");
    map.insert(30, "thirty");

    // Create a const reference to the map.
    const HashMap<int, std::string>& c_map = map;

    // Test cbegin(), begin(), cend(), and end() for const access.
    auto it1 = c_map.cbegin();
    auto it2 = c_map.begin();
    auto it3 = c_map.end();
    auto it4 = c_map.cend();

    // Verify that cbegin() and begin() return equivalent iterators.
    REQUIRE(it1 == it2);
    // And that end() and cend() return equivalent iterators.
    REQUIRE(it3 == it4);

    // Test dereference operators: operator* and operator->.
    // Since the map maintains insertion order (first inserted element is at head),
    // the first inserted element is (10, "ten").
    REQUIRE(it1->first == 10);
    REQUIRE(it1->second == "ten");
    REQUIRE((*it1).first == 10);
    REQUIRE((*it1).second == "ten");

    // Test pre-increment and post-increment.
    auto it_inc = c_map.cbegin();
    auto it_post = it_inc++; // post-increment: returns original iterator.
    // it_post should still point to the first element.
    REQUIRE(it_post->first == 10);
    // Now it_inc should point to the second element.
    REQUIRE(it_inc->first == 20);

    // Test pre-decrement and post-decrement on a const_iterator.
    // Start with an iterator at end().
    auto it_dec = c_map.cend();
    --it_dec;  // pre-decrement: should move to the last element.
    REQUIRE(it_dec->first == 30);

    // Test post-decrement.
    auto it_post_dec = it_dec--;  // it_post_dec gets the current value (30),
                                   // then it_dec moves to the previous element.
    REQUIRE(it_post_dec->first == 30);
    REQUIRE(it_dec->first == 20);

    // Test equality and inequality.
    auto begin_it = c_map.begin();
    auto another_it = c_map.begin();
    REQUIRE(begin_it == another_it);
    ++another_it;
    REQUIRE(begin_it != another_it);

    // Test the const version of find.
    auto find_it = c_map.find(20);
    REQUIRE(find_it != c_map.end());
    REQUIRE(find_it->second == "twenty");

    // Note: Dereferencing c_map.end() is undefined, so we only check equality.
}