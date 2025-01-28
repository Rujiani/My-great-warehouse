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
        float revenue = rp.sell(3);
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
        oss.str() == "[Name: Widget] | Quantity: 10 | Manufacturer: ACME (USA) | Price: 100 | Type: retail_product"
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
        float revenue = wp.sell(10); 
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
        float allowance = 25.0f;
        mgw::retail_product rp = wp.change_to_retail(allowance);

        // Check retail product properties
        REQUIRE(rp.get_quantity() == wp.get_quantity());
        REQUIRE(rp.get_type() == "retail");

        // Retail-specific check
        REQUIRE(rp.get_allowance() == allowance); // Assuming there's a getter for allowance
    }
}