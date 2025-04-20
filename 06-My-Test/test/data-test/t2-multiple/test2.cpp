#include "../../../mytest.hpp"

TEST_CASE("Test2.cpp test case 1") {
    CHECK(true), CHECK(2 * 2 == 5);
}

namespace Test2 {
TEST_CASE("Test2.cpp test case 2. With multiple failures and messages") {
    CHECK_MESSAGE(2 * 2 == 4, "test2-msg1");
    CHECK_MESSAGE(2 * 2 == 5, "test2-msg2");
    CHECK(true), CHECK(2 * 2 == 4);
    CHECK(2 * 2 == 5), CHECK(true);
}
}  // namespace Test2
