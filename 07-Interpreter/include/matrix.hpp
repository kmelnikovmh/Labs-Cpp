#ifndef MATRIX_
#define MATRIX_

#include <stdexcept>
#include <string>
#include <vector>

namespace matrix_interpreter {
struct matrix_error : std::runtime_error {
    explicit matrix_error(const std::string& comment);
};

struct out_of_bounds_error : matrix_error {
    explicit out_of_bounds_error();
};

struct dimension_mismatch_error : matrix_error {
    explicit dimension_mismatch_error(int lhs, int rhs);
};

class Matrix {
    std::vector<std::vector<int>> my_matrix;
    int my_rows;
    int my_cols;

public:
    Matrix();
    Matrix(int rows, int cols);

    int& get_rows();
    int& get_cols();

    int& at(int row, int col);
    static void sum(Matrix& lhs, Matrix& rhs);
    static void multiplication(Matrix& lhs, Matrix& rhs);
};
}  // namespace matrix_interpreter
#endif
