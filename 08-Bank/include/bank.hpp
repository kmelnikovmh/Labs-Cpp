#ifndef BANK_
#define BANK_

#include <condition_variable>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace bank {
struct transfer_error : std::runtime_error {
    explicit transfer_error(std::string str);
};

struct self_transfer_error : transfer_error {
    explicit self_transfer_error();
};

struct not_enough_funds_error : transfer_error {
    explicit not_enough_funds_error(int amount_xts, int balance);
};

struct out_of_range : transfer_error {
    explicit out_of_range(int amount_xts);
};

class user;
class user_transactions_iterator;

struct transaction {
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes,
    // -warnings-as-errors)
    const user* const counterparty;
    const int balance_delta_xts;
    const std::string comment;
    // NOLINTEND(misc-non-private-member-variables-in-classes,
    // -warnings-as-errors)

    transaction(const user* counterparty, int balance_delta_xts, std::string comment) noexcept;

    friend bool operator==(const bank::transaction& a, const bank::transaction& b);
    friend bool operator!=(const bank::transaction& a, const bank::transaction& b);
};

class ledger {
private:
    std::unordered_map<std::string, std::unique_ptr<user>> m_users;
    std::mutex m_ledger_mtx;

public:
    ledger() = default;
    user& get_or_create_user(const std::string& name);
};

class user {
private:
    static const int START_BALANCE = 100;
    const ledger m_ledger;
    const std::string m_name;
    int m_balance;
    std::vector<transaction> m_transactions;
    mutable std::mutex m_user_mtx;
    mutable std::condition_variable m_condvar;

    friend class user_transactions_iterator;

public:
    explicit user(std::string name) noexcept;

    const std::string& name() const noexcept;
    int balance_xts() const;
    void transfer(user& counterparty, int amount_xts, const std::string& comment);

    template <typename function>
    user_transactions_iterator snapshot_transactions(const function& func) const;
    [[nodiscard]] user_transactions_iterator monitor() const;
};

class user_transactions_iterator {
private:
    const user* m_user;  // второй const запретит копирование
    std::size_t m_last_transaction_index;
    friend class user;

public:
    explicit user_transactions_iterator(const user* user) noexcept;
    transaction wait_next_transaction();
};

template <typename function>
user_transactions_iterator user::snapshot_transactions(const function& func) const {
    const std::unique_lock l(m_user_mtx);
    func(m_transactions, m_balance);
    return user_transactions_iterator(this);
}

}  // namespace bank
#endif
