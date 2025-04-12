#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "bank.hpp"

TEST_CASE("Create and get user") {
    bank::ledger l;
    bank::user &alice1 = l.get_or_create_user("Alice");
    l.get_or_create_user("Bob");
    bank::user &alice2 = l.get_or_create_user("Alice");
    CHECK(&alice1 == &alice2);
}

TEST_CASE("Snapshot initial transaction") {
    bank::ledger l;
    bank::user &alice = l.get_or_create_user("Alice");

    std::vector<bank::transaction> transactions_snapshot;
    SUBCASE("non-const") {
        const bank::user_transactions_iterator it =
            alice.snapshot_transactions([&](auto &transactions, int balance_xts) {
                transactions_snapshot = std::vector(transactions.begin(), transactions.end());
                CHECK(balance_xts == 100);
            });
        static_cast<void>(it);
    }

    CHECK(transactions_snapshot == std::vector{bank::transaction{nullptr, 100, "Initial deposit for Alice"}});
}

// ...
