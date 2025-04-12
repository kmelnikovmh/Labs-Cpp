#ifndef MATRIX_INTERPRETER_
#define MATRIX_INTERPRETER_

#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "matrix.hpp"

namespace matrix_interpreter {
struct matrix_interpreter_error : std::runtime_error {
    explicit matrix_interpreter_error(const std::string &comment);
};

struct unknown_command_error : matrix_interpreter_error {
    explicit unknown_command_error(std::string &command);
};

struct invalid_command_format_error : matrix_interpreter_error {
    explicit invalid_command_format_error();
};

struct is_not_register_error : matrix_interpreter_error {
    explicit is_not_register_error(std::string &token);
};

struct unable_to_open_file_error : matrix_interpreter_error {
    explicit unable_to_open_file_error(std::string &file);
};

struct invalid_file_format_error : matrix_interpreter_error {
    explicit invalid_file_format_error();
};

struct invalid_command_input_function_add_or_mul : matrix_interpreter_error {
    explicit invalid_command_input_function_add_or_mul();
};

class MatrixInterpreter {
    enum class Commands { PASS, EXIT, LOAD, PRINT, ELEM, ADD, MUL };
    static Commands get_commands(const std::string &str);

    static std::vector<Matrix> &get_interpreter_register();

    static std::vector<std::string> read_command();

    static int process_register(std::string &my_register);
    static int process_number_element(const std::string &my_number);

public:
    static void start_interpreter();
    static void load(int register_int, std::string &file);
    static void print(int register_int);
    static void elem(int register_int, int row, int col);
    static void
    add_or_mul(Commands operation, int register_int_one, int register_int_two);
};
}  // namespace matrix_interpreter
#endif
