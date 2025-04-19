#include <sstream>
#include "../my_bigint.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using lab_bigint::bigint;

TEST_CASE("Constructor") {
    [[maybe_unused]] const bigint x("179");
}

TEST_CASE("operator==") {
    CHECK(!(bigint("0") == bigint("1")));
}

TEST_CASE("to_string()") {
    CHECK("0" == bigint("0").to_string());
}

// ...

TEST_CASE("operator+= and operator+") {
    bigint a;
    const bigint b = a += 10000;
    CHECK("10000" == a.to_string());
}

// ...

TEST_CASE("operator>>") {
    bigint a;
    std::stringstream("1234") >> a;
    CHECK(1234 == a);
}

// ...
