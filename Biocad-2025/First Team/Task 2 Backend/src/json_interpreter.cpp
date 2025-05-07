#include <fstream>
#include <json_interpreter.hpp>

namespace json_viewer {

unable_output_file_error::unable_output_file_error()
    : std::runtime_error("Unable output answer to file") {};

void JsonRow::process_json(const json& j,
                           std::vector<JsonRow>& row_array,
                           const std::string& path,
                           int deep) {
    JsonRow row;
    row.path = path;

    if (j.is_object()) {
        row.type = "obj";
        row.value = "";
        row_array.push_back(row);

        for (auto& [key, value] : j.items()) {
            std::string child_path = std::string(deep * 2, ' ') + "." + key;
            process_json(value, row_array, child_path, deep + 1);
        }
    } else if (j.is_array()) {
        row.type = "arr";
        row.value = "";
        row_array.push_back(row);

        int index = 0;
        for (auto& element : j) {
            std::string child_path = std::string(deep * 2, ' ') + "[" + std::to_string(index) + "]";
            process_json(element, row_array, child_path, deep + 1);
            index++;
        }
    } else {
        if (j.is_string()) {
            row.type = "str";
            row.value = j.get<std::string>();
        } else if (j.is_number()) {
            row.type = "num";
            row.value = j.dump();
        } else if (j.is_boolean()) {
            row.type = "bool";
            row.value = j.dump();
        } else if (j.is_null()) {
            row.type = "null";
            row.value = "null";
        }
        row_array.push_back(row);
    }
}

std::vector<JsonRow> JsonRow::json_read(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw unable_output_file_error();
    }

    json data;
    file >> data;

    std::vector<JsonRow> row_array;

    for (auto& [key, value] : data.items()) {
        process_json(value, row_array, key, 1);
    }

    return row_array;
}
}  // namespace json_viewer
