#include <boost/dll/import.hpp>
#include <boost/version.hpp>

#include <json_interpreter.hpp>
#include <view.hpp>

#include <memory>
#include <sstream>

#if BOOST_VERSION >= 107600
#define boost_dll_import_symbol ::boost::dll::import_symbol
#else
#define boost_dll_import_symbol ::boost::dll::import
#endif

namespace json_viewer {

struct unknown_cmd_error : std::runtime_error {
    explicit unknown_cmd_error(const std::string& comment)
        : std::runtime_error("Unknown command: " + comment + "\n") {
    }
};

struct invalid_cmd_format_error : std::runtime_error {
    explicit invalid_cmd_format_error() : std::runtime_error("Ivalid command format\n") {
    }
};

enum class COMMANDS { OUTPUT_FORM_FILE, SWITCH_VIEW, EXIT, UNKNOWN };

COMMANDS get_cmd(const std::string& str_cmd) {
    if (str_cmd == "exit") {
        return COMMANDS::EXIT;
    } else if (str_cmd == "file") {
        return COMMANDS::OUTPUT_FORM_FILE;
    } else if (str_cmd == "switch") {
        return COMMANDS::SWITCH_VIEW;
    }
    return COMMANDS::UNKNOWN;
}

std::vector<std::string> read_command() {
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
        throw unknown_cmd_error("");
    }

    const int count_tokens = static_cast<int>(tokens.size());
    COMMANDS command = get_cmd(tokens[0]);

    if (command == COMMANDS::UNKNOWN) {
        throw unknown_cmd_error(tokens[0]);
    } else if ((command == COMMANDS::OUTPUT_FORM_FILE && count_tokens != 2) ||
               (command == COMMANDS::SWITCH_VIEW && count_tokens != 2) ||
               (command == COMMANDS::EXIT && count_tokens != 1)) {
        throw invalid_cmd_format_error();
    }

    return tokens;
}
}  // namespace json_viewer

int main() {
    std::shared_ptr<json_viewer::View> currentView;
    currentView = boost_dll_import_symbol<json_viewer::View>(
        "view-compact", "Views", boost::dll::load_mode::append_decorations);

    while (std::cin) {
        try {
            std::vector<std::string> cmd_tokens = json_viewer::read_command();

            switch (json_viewer::get_cmd(cmd_tokens[0])) {
                case json_viewer::COMMANDS::OUTPUT_FORM_FILE: {
                    std::vector<json_viewer::JsonRow> json_table =
                        json_viewer::JsonRow::json_read(cmd_tokens[1]);
                    currentView->print_table(json_table);
                    break;
                }
                case json_viewer::COMMANDS::SWITCH_VIEW:
                    currentView = boost_dll_import_symbol<json_viewer::View>(
                        cmd_tokens[1], "Views", boost::dll::load_mode::append_decorations);
                    break;
                case json_viewer::COMMANDS::EXIT:
                    break;
                case json_viewer::COMMANDS::UNKNOWN:
                    break;
            }
        } catch (json_viewer::unknown_cmd_error& e) {
            std::cout << e.what() << "\n";
        } catch (json_viewer::invalid_cmd_format_error& e) {
            std::cout << e.what() << "\n";
        } catch (json_viewer::unable_output_file_error& e) {
            std::cout << e.what() << "\n";
        } catch (std::exception& e) {
            std::cout << e.what() << "\n";
        }
    }
}
