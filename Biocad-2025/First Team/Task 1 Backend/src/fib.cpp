#include <cctype>
#include <fib.hpp>
#include <sstream>

namespace fibonacci {
fibonacci_error::fibonacci_error(const std::string& comment) : std::runtime_error(comment) {};

unknown_cmd_error::unknown_cmd_error(const std::string& comment)
    : fibonacci_error::fibonacci_error("Unknown input command: " + comment + "\n") {};

invalid_cmd_format_error::invalid_cmd_format_error()
    : fibonacci_error::fibonacci_error("Invalid command format") {};
    
unable_output_file_error::unable_output_file_error()
    : fibonacci_error::fibonacci_error("Unable output answer to file") {};

// Interpreter
void Interpreter::start() {
    while (std::cin) {
        try {
            std::vector<std::string> cmd_tokens = read_command();
            switch (get_cmd(cmd_tokens[0])) {
                case COMMANDS::OUTPUT_IN_CONSOLE:
                    output_fibonacci("-", std::stoi(cmd_tokens[0]));
                    break;
                case COMMANDS::OUTPUT_IN_FILE:
                    output_fibonacci(cmd_tokens[1], std::stoi(cmd_tokens[2]));
                    break;
                case COMMANDS::EXIT:
                    return;
                    break;
                case COMMANDS::UNKNOWN:
                    break;
            }
        } catch (fibonacci_error& e) {
            std::cout << e.what() << "\n";
        } catch (std::exception& e) {
            std::cout << e.what() << "\n";
            return;
        }
    }
}

std::vector<std::string> Interpreter::read_command() {
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
    COMMANDS command;
    try {
        command = get_cmd(tokens[0]);
    } catch (const std::invalid_argument& e) {
        if (count_tokens == 1) {
            throw invalid_cmd_format_error();
        } else {
            throw invalid_cmd_format_error();
        }
    }

    if (command == COMMANDS::UNKNOWN) {
        throw unknown_cmd_error(tokens[0]);
    } else if ((command == COMMANDS::OUTPUT_IN_CONSOLE && count_tokens != 1) ||
               (command == COMMANDS::OUTPUT_IN_FILE && count_tokens != 3) ||
               (command == COMMANDS::EXIT && count_tokens != 1)) {
        throw invalid_cmd_format_error();
    }

    return tokens;
}

Interpreter::COMMANDS Interpreter::get_cmd(const std::string& str_cmd) {
    if (str_cmd == "exit") {
        return COMMANDS::EXIT;
    } else if (str_cmd == "file") {
        return COMMANDS::OUTPUT_IN_FILE;
    } else if (std::stoi(str_cmd)) {
        return COMMANDS::OUTPUT_IN_CONSOLE;
    }
    return COMMANDS::UNKNOWN;
}

void Interpreter::output_fibonacci(const std::string& filepath, int n) {
    lab_bigint::bigint ans = FibCalc::calc_fibonacci(n);

    if (filepath == "-") {
        std::cout << ans << "\n";
    } else {
        const std::filesystem::path filename(filepath);
        std::ofstream cin_file(filename);
        if (!cin_file.is_open()) {
            throw unable_output_file_error();
        }
        cin_file << ans << "\n";
        cin_file.close();
    }
}

// FibCalc
lab_bigint::bigint FibCalc::calc_fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    std::vector<std::vector<lab_bigint::bigint>> origin = {{1, 1}, {1, 0}};
    std::vector<std::vector<lab_bigint::bigint>> result_matrix = matrix_pow(origin, n - 1);
    return result_matrix[0][0];
}

std::vector<std::vector<lab_bigint::bigint>> FibCalc::matrix_pow(
    std::vector<std::vector<lab_bigint::bigint>> origin,
    int n) {
    std::vector<std::vector<lab_bigint::bigint>> result_matrix(2,
                                                               std::vector<lab_bigint::bigint>(2));
    result_matrix[0][0] = 1;
    result_matrix[0][1] = 0;
    result_matrix[1][0] = 0;
    result_matrix[1][1] = 1;

    while (n > 0) {
        if (n % 2 == 1) {
            result_matrix = matrix_2x2_multiply(result_matrix, origin);
        }
        origin = matrix_2x2_multiply(origin, origin);
        n /= 2;
    }
    return result_matrix;
}

std::vector<std::vector<lab_bigint::bigint>> FibCalc::matrix_2x2_multiply(
    const std::vector<std::vector<lab_bigint::bigint>>& lhs,
    const std::vector<std::vector<lab_bigint::bigint>>& rhs) {
    std::vector<std::vector<lab_bigint::bigint>> result(2, std::vector<lab_bigint::bigint>(2));

    result[0][0] = lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0];
    result[0][1] = lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1];
    result[1][0] = lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0];
    result[1][1] = lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1];

    return result;
}
}  // namespace fibonacci
