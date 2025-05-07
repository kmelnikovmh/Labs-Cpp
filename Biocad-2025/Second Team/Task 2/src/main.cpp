#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <primers.hpp>

namespace primers {

struct unknown_cmd_error : std::runtime_error {
    explicit unknown_cmd_error(const std::string& comment)
        : std::runtime_error("Unknown input command: " + comment + "\n") {};
};

struct invalid_cmd_format_error : std::runtime_error {
    explicit invalid_cmd_format_error() : std::runtime_error("Invalid command format") {};
};

enum class COMMANDS { OUTPUT_IN_CONSOLE, OUTPUT_IN_FILE, SWITCH, EXIT, UNKNOWN };

COMMANDS get_cmd(const std::string& str_cmd) {
    if (str_cmd == "exit") {
        return COMMANDS::EXIT;
    } else if (str_cmd == "console") {
        return COMMANDS::OUTPUT_IN_CONSOLE;
    } else if (str_cmd == "file") {
        return COMMANDS::OUTPUT_IN_FILE;
    } else if (str_cmd == "switch") {
        return COMMANDS::SWITCH;
    }
    return COMMANDS::UNKNOWN;
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
    } else if ((command == COMMANDS::OUTPUT_IN_CONSOLE && count_tokens != 2) ||
               (command == COMMANDS::OUTPUT_IN_FILE && count_tokens != 3) ||
               (command == COMMANDS::SWITCH && count_tokens != 2 &&
                (tokens[1] != "greedy" || tokens[1] != "ilp")) ||
               (command == COMMANDS::EXIT && count_tokens != 1)) {
        throw invalid_cmd_format_error();
    }

    return tokens;
}

void write_ans(std::ostream& stream,
               const std::vector<std::pair<std::string, std::unordered_set<int>>>& ans) {
    stream << "Primers:\n";
    for (auto& [str, cover_set] : ans) {
        stream << "  " << str << "\n   ";
        for (auto id : cover_set) {
            stream << " " << id;
        }
        stream << "\n";
    }
}

void output_primers(const std::string& file_out_path,
                    const std::string& file_data_path,
                    const std::string& ans_mode) {
    Primers m_primers;
    m_primers.read_dna(file_data_path);
    std::vector<std::pair<std::string, std::unordered_set<int>>> ans;
    if (ans_mode == "greedy") {
        ans = m_primers.greedy_cover();
    } else {
        ans = m_primers.ilp_cover();
    }

    if (file_out_path == "-") {
        write_ans(std::cout, ans);
    } else {
        const std::filesystem::path filename(file_out_path);
        std::ofstream cin_file(filename);
        if (!cin_file.is_open()) {
            throw unable_output_file_error();
        }

        write_ans(cin_file, ans);

        cin_file.close();
    }
}
}  // namespace primers

int main() {
    std::string ans_mode = "greedy";

    while (std::cin) {
        try {
            std::vector<std::string> cmd_tokens = primers::read_command();
            switch (primers::get_cmd(cmd_tokens[0])) {
                case primers::COMMANDS::OUTPUT_IN_CONSOLE:
                    primers::output_primers("-", cmd_tokens[1], ans_mode);
                    break;
                case primers::COMMANDS::OUTPUT_IN_FILE:
                    primers::output_primers(cmd_tokens[1], cmd_tokens[2], ans_mode);
                    break;
                case primers::COMMANDS::SWITCH:
                    ans_mode = cmd_tokens[1];
                    break;
                case primers::COMMANDS::EXIT:
                    return 0;
                    break;
                case primers::COMMANDS::UNKNOWN:
                    break;
            }
        } catch (std::exception& e) {
            std::cout << e.what() << "\n";
        }
    }
}
