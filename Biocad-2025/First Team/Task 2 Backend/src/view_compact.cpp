#include <view.hpp>

namespace json_viewer {

class view_compact final : View {
private:

public:
    void print_table(const std::vector<JsonRow> &row_array) const final {
        size_t max_path = 0;
        size_t max_type = 0;
        size_t max_value = 0;
        for (const auto& row : row_array) {
            max_path = std::max(max_path, row.path.length());
            max_type = std::max(max_type, row.type.length());
            max_value = std::max(max_value, row.value.length());
        }

        std::cout << std::left << std::setw(max_path + 4) << "Path"
                  << std::setw(max_value + 2) << "Value"
                  << std::setw(max_type + 2) << "Type\n";
        std::cout << std::string(max_path + 2, '-') << " "
                  << std::string(max_value + 2, '-') << " "
                  << std::string(max_type + 2, '-') << "\n";

        for (const auto& row : row_array) {
            std::cout << std::setw(max_path + 4) << row.path
                      << std::setw(max_value + 2) << row.value
                      << std::setw(max_type + 2) << row.type
                      << "\n";
        }
        std::cout << "\n";
    }
};
}  // namespace json_viewer

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)
extern "C" BOOST_SYMBOL_EXPORT json_viewer::view_compact Views;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)
json_viewer::view_compact Views;

