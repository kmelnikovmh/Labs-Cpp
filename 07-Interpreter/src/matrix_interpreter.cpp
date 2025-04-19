#include "matrix_interpreter.hpp"

namespace matrix_interpreter {

matrix_interpreter_error::matrix_interpreter_error(const std::string& comment)
    : std::runtime_error(comment) {};

unknown_command_error::unknown_command_error(std::string& command)
    : matrix_interpreter_error("Unknown command: '" + command + "'") {};

invalid_command_format_error::invalid_command_format_error()
    : matrix_interpreter_error("Invalid command format") {};

is_not_register_error::is_not_register_error(std::string& token)
    : matrix_interpreter_error("'" + token + "' is not a register") {};

unable_to_open_file_error::unable_to_open_file_error(std::string& file)
    : matrix_interpreter_error("Unable to open file '" + file + "'") {};

invalid_file_format_error::invalid_file_format_error()
    : matrix_interpreter_error("Invalid file format") {};

invalid_command_input_function_add_or_mul::invalid_command_input_function_add_or_mul()
    : matrix_interpreter_error("Invalid commad input function add_or_mul") {};

const int REGISTER_SIZE = 10;

MatrixInterpreter::Commands MatrixInterpreter::get_commands(const std::string& str) {
    if (str == "exit") {
        return Commands::EXIT;
    } else if (str == "load") {
        return Commands::LOAD;
    } else if (str == "print") {
        return Commands::PRINT;
    } else if (str == "elem") {
        return Commands::ELEM;
    } else if (str == "add") {
        return Commands::ADD;
    } else if (str == "mul") {
        return Commands::MUL;
    }
    return Commands::PASS;
}

std::vector<Matrix>& MatrixInterpreter::get_interpreter_register() {
    static std::vector<Matrix> interpreter_register = std::vector<Matrix>(REGISTER_SIZE, Matrix());
    return interpreter_register;
}

void MatrixInterpreter::start_interpreter() {
    while (std::cin) {
        try {
            std::vector<std::string> tokens = read_command();
            int register_int_one = 0;
            int register_int_two = 0;
            int row = 0;
            int col = 0;
            switch (get_commands(tokens[0])) {
                case (Commands::PASS):
                    break;
                case (Commands::EXIT):
                    return;
                    break;
                case (Commands::LOAD):
                    register_int_one = process_register(tokens[1]);
                    load(register_int_one, tokens[2]);
                    break;
                case (Commands::PRINT):
                    register_int_one = process_register(tokens[1]);
                    print(register_int_one);
                    break;
                case (Commands::ELEM):
                    register_int_one = process_register(tokens[1]);
                    row = process_number_element(tokens[2]);
                    col = process_number_element(tokens[3]);
                    elem(register_int_one, row, col);
                    break;
                case (Commands::ADD):
                    register_int_one = process_register(tokens[1]);
                    register_int_two = process_register(tokens[2]);
                    add_or_mul(Commands::ADD, register_int_one, register_int_two);
                    break;
                case (Commands::MUL):
                    register_int_one = process_register(tokens[1]);
                    register_int_two = process_register(tokens[2]);
                    add_or_mul(Commands::MUL, register_int_one, register_int_two);
                    break;
            }
        } catch (matrix_interpreter_error& e) {
            std::cout << e.what() << "\n";
        } catch (matrix_error& e) {
            std::cout << e.what() << "\n";
        } catch (std::bad_alloc&) {
            std::cout << "Unable to allocate memory\n";
        } catch (std::ios_base::failure&) {
            std::cout << "Invalid file format\n";
        } catch (std::exception& e) {
            std::cout << e.what() << "\n";
        }
    }
};

std::vector<std::string> MatrixInterpreter::read_command() {
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
        throw invalid_command_format_error();
    }
    const int count_tokens = static_cast<int>(tokens.size()) - 1;
    const Commands command = get_commands(tokens[0]);
    if (command == Commands::PASS) {
        throw unknown_command_error(tokens[0]);
    } else if ((command == Commands::EXIT && count_tokens != 0) ||
               (command == Commands::LOAD && count_tokens != 2) ||
               (command == Commands::PRINT && count_tokens != 1) ||
               (command == Commands::ELEM && count_tokens != 3) ||
               (command == Commands::ADD && count_tokens != 2) ||
               (command == Commands::MUL && count_tokens != 2)) {
        throw invalid_command_format_error();
    }

    return tokens;
}

int MatrixInterpreter::process_register(std::string& my_register) {
    if (my_register.size() != 2 || my_register[0] != '$' || std::isdigit(my_register[1]) == 0) {
        throw is_not_register_error(my_register);
    }
    return static_cast<int>(my_register[1]) - 48;
}

int MatrixInterpreter::process_number_element(const std::string& my_number) {
    int count_zero = 0;
    for (int i = 0; i < static_cast<int>(my_number.size()); ++i) {
        if (my_number[i] == '0') {
            ++count_zero;
        }
        if (isdigit(my_number[i]) == 0 || (i - count_zero) > 7) {
            throw invalid_command_format_error();
        }
    }
    const int result = std::stoi(my_number);
    if (1'000'000 < result) {
        throw invalid_command_format_error();
    }
    return result;
}

void MatrixInterpreter::load(int register_int, std::string& file) {
    Matrix& matrix = get_interpreter_register()[register_int];

    const std::filesystem::path filename(file);
    std::ifstream cin_file(filename);
    if (!cin_file) {
        throw unable_to_open_file_error(file);
    }
    cin_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    int rows_in_file = 0;
    int cols_in_file = 0;
    cin_file >> rows_in_file >> cols_in_file;
    if (rows_in_file * cols_in_file == 0) {
        rows_in_file = 0;
        cols_in_file = 0;
    }

    Matrix buff_matrix(rows_in_file, cols_in_file);
    if (rows_in_file == 0 && cols_in_file == 0) {
        matrix = buff_matrix;
        return;
    }

    int buff_element = 0;
    for (int i = 0; i < rows_in_file; ++i) {
        for (int j = 0; j < cols_in_file; ++j) {
            cin_file >> buff_element;
            buff_matrix.at(i, j) = buff_element;
        }
    }

    matrix = buff_matrix;
}

void MatrixInterpreter::print(int register_int) {
    Matrix& matrix = get_interpreter_register()[register_int];

    for (int i = 0; i < matrix.get_rows(); ++i) {
        for (int j = 0; j < matrix.get_cols(); ++j) {
            std::cout << matrix.at(i, j);
            if (j != matrix.get_cols() - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

void MatrixInterpreter::elem(int register_int, int row, int col) {
    std::cout << get_interpreter_register()[register_int].at(row, col) << "\n";
}

void MatrixInterpreter::add_or_mul(Commands operation, int register_int_one, int register_int_two) {
    Matrix& matrix_one = get_interpreter_register()[register_int_one];
    Matrix& matrix_two = get_interpreter_register()[register_int_two];
    if (operation == Commands::ADD) {
        Matrix::sum(matrix_one, matrix_two);
    } else if (operation == Commands::MUL) {
        Matrix::multiplication(matrix_one, matrix_two);
    } else {
        throw invalid_command_input_function_add_or_mul();
    }
}

}  // namespace matrix_interpreter
