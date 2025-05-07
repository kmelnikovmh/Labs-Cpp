#ifndef FIB_HPP_
#define FIB_HPP_

#include <exception>
#include <fstream>
#include <iostream>
#include <my_bigint.hpp>
#include <string>
#include <vector>

namespace fibonacci {

struct fibonacci_error : std::runtime_error {
    explicit fibonacci_error(const std::string& comment);
};

struct unknown_cmd_error : fibonacci_error {
    explicit unknown_cmd_error(const std::string& comment);
};

struct invalid_cmd_format_error : fibonacci_error {
    explicit invalid_cmd_format_error();
};

struct unable_output_file_error : fibonacci_error {
    explicit unable_output_file_error();
};

class Interpreter {
private:
    enum class COMMANDS { OUTPUT_IN_CONSOLE, OUTPUT_IN_FILE, EXIT, UNKNOWN };

    static COMMANDS get_cmd(const std::string& str_cmd);
    static std::vector<std::string> read_command();

    static void output_fibonacci(const std::string& filepath, int n);

public:
    static void start();
};

class FibCalc {
private:
    static std::vector<std::vector<lab_bigint::bigint>> matrix_2x2_multiply(
        const std::vector<std::vector<lab_bigint::bigint>>& lhs,
        const std::vector<std::vector<lab_bigint::bigint>>& rhs);

    static std::vector<std::vector<lab_bigint::bigint>> matrix_pow(
        std::vector<std::vector<lab_bigint::bigint>> origin,
        int n);

    static lab_bigint::bigint calc_fibonacci(int n);

    friend class Interpreter;
};
}  // namespace fibonacci
#endif
