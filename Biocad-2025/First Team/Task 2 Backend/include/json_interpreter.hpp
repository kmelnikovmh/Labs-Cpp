#ifndef JSON_INTERPRETER_HPP_
#define JSON_INTERPRETER_HPP_

#include <exception>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace json_viewer {

struct unable_output_file_error : std::runtime_error {
    explicit unable_output_file_error();
};

struct JsonRow {
    std::string path;
    std::string value;
    std::string type;

    static void process_json(const json& j,
                             std::vector<JsonRow>& row_array,
                             const std::string& path,
                             int deep);

    static std::vector<JsonRow> json_read(const std::string& filepath);
};
}  // namespace json_viewer
#endif
