#include <iostream>
#include <sstream>

#include "../my_bigint.hpp"  // NOLINT(bugprone-suspicious-include)

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#define TASK_1
#define TASK_2
#define TASK_3

// NOLINTBEGIN(readability-function-cognitive-complexity)
// NOLINTBEGIN(misc-use-anonymous-namespace)

using lab_bigint::bigint;

#ifdef TASK_1
TEST_CASE("Constructor") {
    [[maybe_unused]] const bigint x("179");

    // ...
}

TEST_CASE("operator==") {
    CHECK(!(bigint("0") == bigint("1")));
    
    // ...
}

TEST_CASE("operator!=") {
    CHECK(bigint("0") != bigint("1"));
    
    // ...
}

TEST_CASE("operator<") {
    CHECK(bigint("0") < bigint("1"));
    
    // ...
}

TEST_CASE("operator>") {
    CHECK(!(bigint("0") > bigint("1")));
    
    // ...
}

TEST_CASE("operator<=") {
    CHECK(bigint("0") <= bigint("1"));
    
    // ...
}

TEST_CASE("operator>=") {
    CHECK(!(bigint("0") >= bigint("1")));
    
    // ...
}

bool check_to_string(const std::string &value) {
    return value == bigint(value).to_string();
}

TEST_CASE("to_string()") {
    CHECK(check_to_string("0"));
    
    // ...
}

TEST_CASE("Leading zeros do not matter") {
    CHECK(bigint("00000001") == bigint("1"));
    
    // ...
}

TEST_CASE("Conversion to unsigned int works") {
    CHECK(static_cast<unsigned int>(bigint("12345678")) == 12345678);
    
    // ...
}
#endif  // TASK_1

#ifdef TASK_2
TEST_CASE("operator+= and operator+") {
    bigint a;
    const bigint b = a += 10000;
    CHECK("10000" == a.to_string());
    
    // ...
}

TEST_CASE("operator+ with leading zeros") {
    CHECK(bigint("12345") == bigint("00012000") + 345);

    // ...
}

TEST_CASE("operator+= to itself") {
    bigint a = 12345;
    a += a;
    CHECK(24690 == a);

    // ...
}

TEST_CASE("operator-= and operator-") {
    bigint a("1000000000000000000000000010000");
    const bigint b = a -= 1;
    CHECK("1000000000000000000000000009999" == a.to_string());
    
    // ...
}

TEST_CASE("operator- with leading zeros") {
    CHECK(123000 == bigint("123456") - bigint("000456"));

    // ...
}

TEST_CASE("assignment operators") {
    const bigint a("1234");
    const bigint b("56");

    CHECK(a == bigint(a));
    CHECK(1290 == a + b);
    
    // ...
}

TEST_CASE("operator++ and operator++(int)") {
    bigint a("999999999999998");
    bigint b = ++a;
    CHECK("999999999999999" == a.to_string());
    CHECK("999999999999999" == b.to_string());
   
    // ...
}

TEST_CASE("operator-- and operator--(int)") {
    bigint a("1000000000000001");
    bigint b = --a;
    CHECK("1000000000000000" == a.to_string());
    CHECK("1000000000000000" == b.to_string());

    // ...
}
#endif  // TASK_2

#ifdef TASK_3
TEST_CASE("operator>>") {
    bigint a;

    std::stringstream("1234") >> a;
    CHECK(1234 == a);
    
    // ...
}

bool check_output(const std::string &value) {
    std::stringstream ss;
    ss << bigint(value);
    return value == ss.str();
}

TEST_CASE("operator<<") {
    CHECK(check_output("0"));
    
    // ...
}
#endif  // TASK_3

// NOLINTEND(misc-use-anonymous-namespace)
// NOLINTEND(readability-function-cognitive-complexity)
