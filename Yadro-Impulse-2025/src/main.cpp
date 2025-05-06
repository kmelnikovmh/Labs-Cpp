#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <tape.hpp>
#include <sort.hpp>

namespace {

struct interpreter_error : std::runtime_error {
    explicit interpreter_error(const std::string& comment) : std::runtime_error(comment) {};
};

struct unknown_cmd_error : interpreter_error {
    explicit unknown_cmd_error(const std::string& comment)
        : interpreter_error("Unknown input command: " + comment + "\n") {};
};

struct invalid_cmd_format_error : interpreter_error {
    explicit invalid_cmd_format_error() : interpreter_error("Invalid command format") {};
};

enum class COMMANDS { SORT, MAX_FILES, MAX_RAM, EXIT, UNKNOWN };

COMMANDS get_cmd(const std::string& str_cmd) {
    if (str_cmd == "sort") {
        return COMMANDS::SORT;
    } else if (str_cmd == "max-files") {
        return COMMANDS::MAX_FILES;
    } else if (str_cmd == "max-ram") {
        return COMMANDS::MAX_RAM;
    } else if (str_cmd == "exit") {
        return COMMANDS::EXIT;
    }
    return COMMANDS::UNKNOWN;
}

bool is_integer(const std::string& str) {
    try {
        std::stoi(str);
        return true;
    } catch (const std::runtime_error& e) {
        return false;
    }
}

std::vector<std::string> read_command() {
    std::string zero_string;
    std::vector<std::string> tokens;
    tokens.reserve(5);

    if (!std::getline(std::cin, zero_string)) {
        tokens.emplace_back("exit");
        return tokens;
    };

    std::istringstream stream(zero_string);
    std::string buff_str;
    while (stream >> buff_str) {
        tokens.push_back(buff_str);
    }

    if (tokens.empty()) {
        throw invalid_cmd_format_error();
    }

    const int count_tokens = static_cast<int>(tokens.size());
    COMMANDS command = get_cmd(tokens[0]);
    if (command == COMMANDS::UNKNOWN) {
        throw unknown_cmd_error(tokens[0]);
    } else if ((command == COMMANDS::SORT && count_tokens != 3) ||
               (command == COMMANDS::MAX_FILES && count_tokens != 2 && !is_integer(tokens[1])) ||
               (command == COMMANDS::MAX_RAM && count_tokens != 2 && !is_integer(tokens[1])) ||
               (command == COMMANDS::EXIT && count_tokens != 1)) {
        throw invalid_cmd_format_error();
    }

    return tokens;
}
}
 
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect arguments - wait <config_path>\n";
        return 1;
    }

    std::string config_path = argv[1];
    std::string tmp_path = "../tmp/";
    int max_files = 4;
    int max_ram = 32;  // Our M

    while (std::cin) {
        try {
            std::vector<std::string> cmd_tokens = read_command();
            switch (get_cmd(cmd_tokens[0])) {
                case COMMANDS::SORT: {
                    tape_sort::SortTape m_sort_tape(
                        cmd_tokens[1], cmd_tokens[2], config_path, tmp_path, max_files, max_ram);
                    m_sort_tape.external_sort();
                    break;
                }
                case COMMANDS::MAX_FILES:
                    max_files = std::stoi(cmd_tokens[1]);
                    break;
                case COMMANDS::MAX_RAM:
                    max_ram = std::stoi(cmd_tokens[1]);
                    break;
                case COMMANDS::EXIT:
                    return 0;
                    break;
                case COMMANDS::UNKNOWN:
                    break;
            }
        } catch (interpreter_error& e) {
            std::cout << e.what() << "\n";
        } catch (tape_sort::tape_error& e) {
            std::cout << e.what() << "\n";
        } catch (std::ios_base::failure& e) {
            std::cout << e.what() << "\n";
        } catch (std::runtime_error& e) {
            std::cout << "An unexpected error:\n" << "  - " << e.what() << "\n";
        }
    }
}
