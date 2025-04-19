#include "bank.hpp"
#include <iostream>
#include <thread>

namespace bank {
// cppcheck-suppress passedByValue
// NOLINTNEXTLINE(performance-unnecessary-value-param, -warnings-as-errors)
transfer_error::transfer_error(std::string str) : std::runtime_error(str) {
}

self_transfer_error::self_transfer_error() : transfer_error("") {
}

not_enough_funds_error::not_enough_funds_error(int amount_xts, int balance)
    : transfer_error("Not enough funds: " + std::to_string(balance) + " XTS available, " +
                     std::to_string(amount_xts) + " XTS requested") {
}

out_of_range::out_of_range(int amount_xts)
    : transfer_error("Can't transfer " + std::to_string(amount_xts) + " XTS. It's foo") {
}

// transaction
// cppcheck-suppress passedByValue
transaction::transaction(const user* counterparty,
                         int balance_delta_xts,
                         std::string comment) noexcept
    : counterparty(counterparty),
      balance_delta_xts(balance_delta_xts),
      comment(std::move(comment)) {
}

bool operator==(const bank::transaction& a, const bank::transaction& b) {
    return a.counterparty == b.counterparty && a.balance_delta_xts == b.balance_delta_xts &&
           a.comment == b.comment;
}

bool operator!=(const bank::transaction& a, const bank::transaction& b) {
    return !(a == b);
}

// ledger
user& ledger::get_or_create_user(const std::string& name) {
    const std::unique_lock l(m_ledger_mtx);
    auto [it_user, inserted] = m_users.emplace(name, std::make_unique<user>(name));
    return *it_user->second;
}

// user
// cppcheck-suppress passedByValue
user::user(std::string name) noexcept
    : m_name(std::move(name)),
      m_balance(START_BALANCE),
      m_transactions({transaction(nullptr, m_balance, "Initial deposit for " + m_name)}) {
}

const std::string& user::name() const noexcept {
    return m_name;
}

int user::balance_xts() const {
    return m_balance;
}

void user::transfer(user& counterparty, int amount_xts, const std::string& comment) {
    if (&counterparty == this) {
        throw self_transfer_error();
    } else if (amount_xts < 0) {
        throw out_of_range(amount_xts);
    }

    const std::scoped_lock sl(m_user_mtx, counterparty.m_user_mtx);
    if (INT_MAX < static_cast<int64_t>(amount_xts) + counterparty.m_balance) {
        throw out_of_range(amount_xts);
    } else if (amount_xts > m_balance) {
        throw not_enough_funds_error(amount_xts, m_balance);
    }

    m_balance -= amount_xts;
    counterparty.m_balance += amount_xts;
    counterparty.m_transactions.emplace_back(this, amount_xts, comment);
    m_transactions.emplace_back(&counterparty, -amount_xts, comment);

    this->m_condvar.notify_one();
    counterparty.m_condvar.notify_one();
}

[[nodiscard]] user_transactions_iterator user::monitor() const {
    const std::unique_lock l(m_user_mtx);
    return user_transactions_iterator(this);
}

// user_transactions_iterator
user_transactions_iterator::user_transactions_iterator(const user* user) noexcept
    : m_user(user), m_last_transaction_index(user->m_transactions.size()) {
}

transaction user_transactions_iterator::wait_next_transaction() {
    std::unique_lock l(m_user->m_user_mtx);

    while (m_user->m_transactions.size() <= m_last_transaction_index) {
        m_user->m_condvar.wait(l);
    }

    return m_user->m_transactions[m_last_transaction_index++];
}

}  // namespace bank
