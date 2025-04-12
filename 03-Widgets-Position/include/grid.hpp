#ifndef GRID_
#define GRID_
#include <memory>
#include <vector>
#include "abstract_widgets.hpp"

namespace widgets {
class grid final : public container {
public:
    explicit grid(int, int);

    widget *add(std::unique_ptr<widget>, int, int);
    std::unique_ptr<widget> remove(int, int);
    [[nodiscard]] bool is_not_correct_index(int row, int column) const;

    [[nodiscard]] widget *get(int, int) const;
    [[nodiscard]] int rows() const;
    [[nodiscard]] int columns() const;
    [[nodiscard]] int width() const final;
    [[nodiscard]] int height() const final;

    void update_max_width_in_column(int);
    void update_max_height_in_row(int);
    void update_layout() final;

    widget *child_at(int, int) final;

private:
    int m_rows;
    int m_columns;
    std::vector<int> max_width_in_columns;
    std::vector<int> max_height_in_rows;
    int m_width;
    int m_height;
    std::vector<std::vector<std::unique_ptr<widget>>> m_children;
};

std::unique_ptr<grid> make_grid(int, int);
}  // namespace widgets
#endif
