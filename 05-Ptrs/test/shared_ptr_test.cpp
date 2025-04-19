#include "../shared_ptr.hpp"
#include <iostream>  // https://github.com/onqtam/doctest/issues/356
#include <thread>
#include <vector>
#include "doctest.h"

using ptrs::shared::shared_ptr;

TEST_CASE("shared_ptr copy constructor") {
    int* const raw_p = new int(10);
    const shared_ptr<int> p1(raw_p);

    REQUIRE(p1.get() == raw_p);
    if (raw_p != nullptr) {
        REQUIRE(p1);
        CHECK(&*p1 == raw_p);
    } else {
        CHECK(!p1);
    }
}

// ...

TEST_CASE("shared_ptr can be copy-constructed thread-safely") {
    struct Foo {
        std::string s = std::string(1'000'000, 'x');
    };

    const shared_ptr<Foo> p_orig(new Foo());

    constexpr int THREADS = 10;
    constexpr int OPERATIONS = 1'000'000;

    std::vector<shared_ptr<Foo>> ps(THREADS, p_orig);
    std::vector<std::thread> threads;
    for (int thread_id = 0; thread_id < THREADS; ++thread_id) {
        threads.emplace_back([&, thread_id]() {
            for (int i = 0; i < OPERATIONS; ++i) {
                const shared_ptr<Foo> p(ps[thread_id]);
                static_cast<void>(*p);
            }
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}
