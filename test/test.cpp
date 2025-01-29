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