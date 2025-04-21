#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#include <boost/dll/import.hpp>
#include <boost/version.hpp>
#include <game.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <view.hpp>

#if BOOST_VERSION >= 107600
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import_symbol
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import
#endif

std::string primaryProcessingString(const std::string& str) {
    std::stringstream sstream(str);
    std::string out;
    sstream >> out;
    return out;
}

int main(int argc, char* argv[]) {
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

    if (argc > 2) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic,-warnings-as-errors)
        std::string key = argv[2];
        if (key[0] == 'D' || key[1] == 'D') {
#define VIEW_COMPACT_USE_ZERO
        }
    }
    std::shared_ptr<tictactoe::View> currentView;
    currentView = boost_dll_import_symbol<tictactoe::View>(
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic,-warnings-as-errors)
        argv[1], "Views", boost::dll::load_mode::append_decorations);

    tictactoe::Game currGame = tictactoe::Game();
    std::string str;
    tictactoe::State rightCircle = tictactoe::State::nice;
    (*currentView).printField(currGame, rightCircle, false);
    while (std::getline(std::cin, str)) {
        std::string primaryString = primaryProcessingString(str);
        if (primaryString.starts_with("view-")) {
            currentView = boost_dll_import_symbol<tictactoe::View>(
                primaryString, "Views", boost::dll::load_mode::append_decorations);
            currentView->printField(currGame, rightCircle, true);
            continue;
        }

        rightCircle = currGame.oneCircle(currentView->readCoord(str), currentView->existExit());
        if (rightCircle == tictactoe::State::exit) {
            break;
        }
        if (rightCircle != tictactoe::State::bad) {
            currentView->printField(currGame, rightCircle, false);
        }
        if (rightCircle == tictactoe::State::wins) {
            break;
        }
    }
}
