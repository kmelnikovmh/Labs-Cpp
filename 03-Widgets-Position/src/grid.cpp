#include "../include/grid.hpp"
#include <numeric>

namespace widgets {
grid::grid(int row, int column)
    : m_rows(row), m_columns(column), m_width(0), m_height(0) {
    m_children.resize(row);
    for (int i = 0; i < row; ++i) {
        m_children[i].resize(column);
        for (int j = 0; j < column; ++j) {
            m_children[i][j] = nullptr;
        }
    }
    max_height_in_rows = std::vector<int>(row, 0);
    max_width_in_columns = std::vector<int>(column, 0);
}

bool grid::is_not_correct_index(int row, int column) const {
    return m_rows <= row || row < 0 || m_columns <= column || column < 0;
}

widget *grid::add(std::unique_ptr<widget> ptr, int row, int column) {
    if (is_not_correct_index(row, column)) {
        return nullptr;
    }
    m_children[row][column] = std::move(ptr);
    set_parent(m_children[row][column].get(), this);
    update_max_width_in_column(column);
    update_max_height_in_row(row);
    return (m_children[row][column]).get();
}

std::unique_ptr<widget> grid::remove(int row, int column) {
    if (is_not_correct_index(row, column) ||
        m_children[row][column] == nullptr) {
        return nullptr;
    }
    set_parent(m_children[row][column].get(), nullptr);
    std::unique_ptr<widget> ptr = std::move(m_children[row][column]);
    m_children[row][column] = nullptr;
    update_max_width_in_column(column);
    update_max_height_in_row(row);
    return ptr;
}

widget *grid::get(int row, int column) const {
    if (is_not_correct_index(row, column)) {
        return nullptr;
    }
    return m_children[row][column].get();
}

int grid::rows() const {
    return m_rows;
}

int grid::columns() const {
    return m_columns;
}

int grid::width() const {
    return m_width;
}

int grid::height() const {
    return m_height;
}

void grid::update_max_width_in_column(int column) {
    max_width_in_columns[column] = 0;
    for (int i = 0; i < m_rows; ++i) {
        if (m_children[i][column] != nullptr) {
            max_width_in_columns[column] = std::max(
                max_width_in_columns[column], m_children[i][column]->width()
            );
        }
    }
    m_width = std::accumulate(
        max_width_in_columns.begin(), max_width_in_columns.end(), 0
    );
}

void grid::update_max_height_in_row(int row) {
    max_height_in_rows[row] = 0;
    for (int j = 0; j < m_columns; ++j) {
        if (m_children[row][j] != nullptr) {
            max_height_in_rows[row] =
                std::max(max_height_in_rows[row], m_children[row][j]->height());
        }
    }
    m_height = std::accumulate(
        max_height_in_rows.begin(), max_height_in_rows.end(), 0
    );
}

void grid::update_layout() {
    max_height_in_rows = std::vector(m_rows, 0);
    max_width_in_columns = std::vector(m_columns, 0);
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_columns; ++j) {
            if (m_children[i][j] != nullptr) {
                max_width_in_columns[j] = std::max(
                    max_width_in_columns[j], m_children[i][j]->width()
                );
                max_height_in_rows[i] =
                    std::max(max_height_in_rows[i], m_children[i][j]->height());
            }
        }
    }
    m_width = std::accumulate(
        max_width_in_columns.begin(), max_width_in_columns.end(), 0
    );
    m_height = std::accumulate(
        max_height_in_rows.begin(), max_height_in_rows.end(), 0
    );
}

widget *grid::child_at(int x, int y) {
    if (this->widget::child_at(x, y) == nullptr) {
        return nullptr;
    }
    int curr_height = 0;
    int i = 0;
    for (; i < m_rows; ++i) {
        const int i_height = max_height_in_rows[i];
        if (curr_height <= y && y < curr_height + i_height) {
            break;
        }
        curr_height += i_height;
    }
    int curr_width = 0;
    int j = 0;
    for (; j < m_columns; ++j) {
        const int j_width = max_width_in_columns[j];
        if (curr_width <= x && x < curr_width + j_width) {
            break;
        }
        curr_width += j_width;
    }
    std::unique_ptr<widget> &ptr = m_children[i][j];
    if (ptr && x < curr_width + ptr->width() &&
        y < curr_height + ptr->height()) {
        return ptr->child_at(x - curr_width, y - curr_height);
    }
    return nullptr;
}

std::unique_ptr<grid> make_grid(int rows, int columns) {
    return std::make_unique<grid>(rows, columns);
}
}  // namespace widgets
