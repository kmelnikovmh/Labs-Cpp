#include <iostream>  // https://github.com/onqtam/doctest/issues/356
#include "doctest.h"

#include "../unique_ptr.hpp"

using ptrs::unique::unique_ptr;

TEST_CASE("unique_ptr incomplete type operations") {
    [[maybe_unused]] auto not_invoked = []() {
        struct IncompleteType;
        unique_ptr<IncompleteType> &p1 = *new unique_ptr<IncompleteType>();

        static_cast<void>(p1.get());
    };
}

// ...

TEST_CASE("unique_ptr custom copyable deleter") {
    struct Deleter {
        int *counter;
        void operator()(int *) {
            ++*counter;
        }
    };

    int counter = 0;
    const Deleter d{&counter};
    {
        const unique_ptr<int, Deleter> p(new int(10), d);
        CHECK(counter == 0);
    }
    CHECK(counter == 1);
}

// ...
