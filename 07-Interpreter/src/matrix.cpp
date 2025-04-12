#include "matrix.hpp"

namespace matrix_interpreter {
matrix_error::matrix_error(const std::string &comment)
    : std::runtime_error(comment){};

out_of_bounds_error::out_of_bounds_error()
    : matrix_error("Requested element is out of bounds"){};

dimension_mismatch_error::dimension_mismatch_error(int lhs, int rhs)
    : matrix_error(
          "Dimension mismatch: lhs=" + std::to_string(lhs) +
          ", rhs=" + std::to_string(rhs)
      ){};

Matrix::Matrix()
    : my_matrix(std::vector<std::vector<int>>(0, std::vector<int>(0))),
      my_rows(0),
      my_cols(0) {
}

Matrix::Matrix(int rows, int cols)
    : my_matrix(std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0))),
      my_rows(rows),
      my_cols(cols) {
}

int &Matrix::get_rows() {
    return my_rows;
}

int &Matrix::get_cols() {
    return my_cols;
}

int &Matrix::at(int row, int col) {
    if (row >= my_rows || col >= my_cols) {
        throw out_of_bounds_error();
    }
    return my_matrix.at(row).at(col);
}

void Matrix::sum(Matrix &lhs, Matrix &rhs) {
    if (lhs.get_rows() != rhs.get_rows()) {
        throw dimension_mismatch_error(lhs.get_rows(), rhs.get_rows());
    } else if (lhs.get_cols() != rhs.get_cols()) {
        throw dimension_mismatch_error(lhs.get_cols(), rhs.get_cols());
    }

    Matrix buff_matrix(lhs.get_rows(), lhs.get_cols());
    for (int i = 0; i < lhs.get_rows(); ++i) {
        for (int j = 0; j < lhs.get_cols(); ++j) {
            buff_matrix.at(i, j) = lhs.at(i, j) + rhs.at(i, j);
        }
    }
    lhs = std::move(buff_matrix);
}

void Matrix::multiplication(Matrix &lhs, Matrix &rhs) {
    if (lhs.get_cols() != rhs.get_rows()) {
        throw dimension_mismatch_error(lhs.get_cols(), rhs.get_rows());
    }

    Matrix buff_matrix(lhs.get_rows(), rhs.get_cols());
    for (int i = 0; i < lhs.get_rows(); ++i) {
        for (int j = 0; j < rhs.get_cols(); ++j) {
            int64_t buff_arithmetics = 0;
            for (int r = 0; r < lhs.get_cols(); ++r) {
                buff_arithmetics += static_cast<int64_t>(lhs.at(i, r)) *
                                    static_cast<int64_t>(rhs.at(r, j));
            }
            buff_matrix.at(i, j) = static_cast<int>(buff_arithmetics);
        }
    }
    lhs = std::move(buff_matrix);
}

}  // namespace matrix_interpreter
