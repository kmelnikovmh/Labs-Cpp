#include <view.hpp>

namespace tictactoe {
class view_compact final : View {
    int static processingString(const std::string& str) {
        if (str.empty()) {
            return -2;
        }
        unsigned int i = 0;
        if (str[0] == '-') {
            i = 1;
        }
        for (; i < str.size(); ++i) {
            if (isdigit(str[i]) == 0) {
                return -1;
            }
        }
        return std::stoi(str);
    }

public:
    std::tuple<int, int, int> readCoord(std::string& str) const final {
        std::stringstream sstream(str);
        std::string first;
        std::string second;
        std::string third;
        sstream >> first >> second >> third;
        const int y = processingString(first);
        const int x = processingString(second);
        return {y, x, static_cast<int>(third.size())};
    }

    void printField(Game& currGame, State& maybeWins, bool swift) const final {
        if (!swift) {
            std::cout << "\n";
        }
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                const int cell = currGame.what_in_cell(i, j);
                if (cell == -1) {
                    std::cout << '.';
                } else if (cell == 0) {
#ifdef VIEW_COMPACT_USE_ZERO
                    std::cout << '0';
#else
                    std::cout << 'O';
#endif
                } else {
                    std::cout << 'X';
                }
            }
            std::cout << "\n";
        }
        if (maybeWins == State::wins) {
            std::cout << currGame.whoPlayer_to_char() << " wins!\n";
        } else {
            std::cout << currGame.whoPlayer_to_char() << " move: ";
        }
    }

    [[nodiscard]] bool existExit() const final {
        return true;
    }
};
}  // namespace tictactoe

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)
extern "C" BOOST_SYMBOL_EXPORT tictactoe::view_compact Views;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)
tictactoe::view_compact Views;
