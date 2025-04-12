#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include "bank.hpp"
#include "boost/asio.hpp"
#include "boost/asio/ip/address.hpp"

enum class Commands { BALANCE, TRANSFER, TRANSACTION, MONITOR, UNKNOWN };

Commands get_command(const std::string &str) {
    if (str == "balance") {
        return Commands::BALANCE;
    } else if (str == "transfer") {
        return Commands::TRANSFER;
    } else if (str == "transactions") {
        return Commands::TRANSACTION;
    } else if (str == "monitor") {
        return Commands::MONITOR;
    }
    return Commands::UNKNOWN;
}

bank::user &
hello_user(boost::asio::ip::tcp::iostream &client, bank::ledger &m_ledger) {
    client << "What is your name?\n";
    std::string client_name;
    client >> client_name;
    client << "Hi " << client_name << "\n";
    return m_ledger.get_or_create_user(client_name);
}

void balance(
    boost::asio::ip::tcp::iostream &client,
    const bank::user &client_user
) {
    client << client_user.balance_xts() << "\n";
}

void transfer(
    boost::asio::ip::tcp::iostream &client,
    bank::user &client_user,
    bank::ledger &m_ledger
) {
    std::string counterparty;
    int amount = 0;
    std::string comment;
    client >> counterparty >> amount;
    std::getline(client, comment);
    comment.erase(0, 1);
    try {
        client_user.transfer(
            m_ledger.get_or_create_user(counterparty), amount, comment
        );
        client << "OK\n";
    } catch (bank::transfer_error &e) {
        client << e.what() << "\n";
    }
}

bank::user_transactions_iterator transaction(
    boost::asio::ip::tcp::iostream &client,
    const bank::user &client_user
) {
    size_t count_transactions = 0;
    client >> count_transactions;
    bank::user_transactions_iterator client_iterator =
        client_user.snapshot_transactions([&](auto &m_transaction,
                                              int m_balance) {
            client << "CPTY\tBAL\tCOMM\n";
            size_t i = 0;
            if (count_transactions >= m_transaction.size()) {
                i = 0;
            } else {
                i = m_transaction.size() - count_transactions;
            }
            for (; i < m_transaction.size(); ++i) {
                if (m_transaction[i].counterparty == nullptr) {
                    client << "-\t" << m_transaction[i].balance_delta_xts
                           << "\t" << m_transaction[i].comment << "\n";
                } else {
                    client << m_transaction[i].counterparty->name() << "\t"
                           << m_transaction[i].balance_delta_xts << "\t"
                           << m_transaction[i].comment << "\n";
                }
            }
            client << "===== BALANCE: " << m_balance << " XTS =====\n";
        });
    return client_iterator;
}

void monitor(
    boost::asio::ip::tcp::iostream &client,
    const bank::user &client_user
) {
    bank::user_transactions_iterator client_iterator =
        transaction(client, client_user);
    while (true) {
        const bank::transaction client_next_transaction =
            client_iterator.wait_next_transaction();
        client << client_next_transaction.counterparty->name() << "\t"
               << client_next_transaction.balance_delta_xts << "\t"
               << client_next_transaction.comment << "\n";
    }
}

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

int main(int argc, char *argv[]) {
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif
    if (argc != 3) {
        std::cout << "Invalid input parameters\n";
        return 1;
    }

    boost::asio::io_context io_context;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic,-warnings-as-errors)
    const std::string port(argv[1]);
    boost::asio::ip::tcp::acceptor acceptor(
        io_context,
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v4(), static_cast<uint16_t>(std::stoi(port))
        )
    );
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic,-warnings-as-errors)
    const std::string filepath(argv[2]);
    const std::filesystem::path filename(filepath);
    std::ofstream cin_file(filename);
    if (!cin_file.is_open()) {
        std::cerr << "Unable to store port to file " << filename << "\n";
        return 0;
    }
    cin_file << acceptor.local_endpoint().port();
    cin_file.close();

    std::cout << "Listening at " << acceptor.local_endpoint() << "\n";

    bank::ledger m_ledger;
    while (true) {
        boost::asio::ip::tcp::socket s = acceptor.accept();
        std::thread([s = std::move(s), &m_ledger]() mutable {
            std::cout << "Connected " << s.remote_endpoint() << " --> "
                      << s.local_endpoint() << "\n";
            boost::asio::ip::tcp::iostream client(std::move(s));

            bank::user &client_user = hello_user(client, m_ledger);

            std::string client_zero_string;
            while (client >> client_zero_string) {
                const Commands command = get_command(client_zero_string);
                switch (command) {
                    case Commands::BALANCE:
                        balance(client, client_user);
                        break;
                    case Commands::TRANSFER:
                        transfer(client, client_user, m_ledger);
                        break;
                    case Commands::TRANSACTION:
                        transaction(client, client_user);
                        break;
                    case Commands::MONITOR: {
                        monitor(client, client_user);
                        break;
                    }
                    case Commands::UNKNOWN:
                        client << "Unknown command: '" << client_zero_string
                               << "'\n";
                        break;
                }
            }

            std::cout << "Disconnected " << client.socket().remote_endpoint()
                      << " --> " << client.socket().local_endpoint() << "\n";
        }).detach();
    }
}
