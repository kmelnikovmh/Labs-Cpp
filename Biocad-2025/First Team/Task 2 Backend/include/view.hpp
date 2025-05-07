#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <boost/config.hpp>
#include <iostream>
#include <json_interpreter.hpp>

namespace json_viewer {

class BOOST_SYMBOL_VISIBLE View {
public:
    View(const View&) = default;
    View(View&&) = default;
    View& operator=(const View&) = default;
    View& operator=(View&&) = default;
    virtual ~View() = default;
    View() = default;

    virtual void print_table(const std::vector<JsonRow>& row_array) const = 0;
};
}  // namespace json_viewer
#endif
