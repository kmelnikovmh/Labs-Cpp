#include <view.hpp>

namespace tictactoe {

class view_pretty : View {
    std::pair<char, int> static processingString(std::string &str) {
        if (str.empty() || (str[1] != '-' && isdigit(str[1]) == 0)) {
            return {'-', -2};
        }
        std::stringstream sstream(str);
        char alpha = 0;
        int digit = 0;
        sstream >> alpha >> digit;
        return {alpha, digit};
    }

    [[nodiscard]] char static what_in_cell_PrettyVersion(int key) {
        if (key == -1) {
            return ' ';
        } else if (key == 0) {
            return '@';
        } else {
            return 'X';
        }
    }

    void static printCell(
        const Game &currGame,
        int i,
        int j,
        int row,
        int column
    ) {
        if (i == 0) {
            if (j == 4) {
                std::cout << "+";
            } else {
                std::cout << "-";
            }
            return;
        }
        const char filler =
            what_in_cell_PrettyVersion(currGame.what_in_cell(row, column));
        if (j == 0 || j == 3) {
            std::cout << " ";
        } else if ((filler == ' ' || filler == '@') && j < 4) {
            std::cout << filler;
        } else if (j == 4) {
            std::cout << "|";
        } else if (j == 1) {
            if (i == 1) {
                std::cout << static_cast<char>(92);
            } else {
                std::cout << "/";
            }
        } else if (j == 2) {
            if (i == 1) {
                std::cout << "/";
            } else {
                std::cout << static_cast<char>(92);
            }
        }
    }

    void static printRow(const Game &currGame, int i, int row) {
        if (i == 1) {
            std::cout << 9 - row;
        } else {
            std::cout << " ";
        }
        std::cout << " ";
        if (i == 0) {
            std::cout << "+";
        } else {
            std::cout << "|";
        }

        for (int column = 0; column < 10; ++column) {
            for (int j = 0; j < 5; ++j) {
                printCell(currGame, i, j, row, column);
            }
        }
        std::cout << "\n";
    }

public:
    std::tuple<int, int, int> readCoord(std::string &str) const final {
        std::stringstream sstream(str);
        std::string first;
        std::string second;
        sstream >> first >> second;
        auto [alpha, digit] = processingString(first);
        const int x = (alpha - 'a');
        int y = digit;
        if (digit > -1 && digit < 10) {
            y = 9 - digit;
        }
        return {y, x, static_cast<int>(second.size())};
    }

    void printField(Game &currGame, State &maybeWins, bool swift) const final {
        if (!swift) {
            std::cout << "\n";
        }
        for (int row = 0; row < 10; ++row) {
            for (int i = 0; i < 3; ++i) {
                printRow(currGame, i, row);
            }
        }

        std::cout << "  ";
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (col == 0) {
                    std::cout << "+";
                } else {
                    std::cout << "-";
                }
            }
        }
        std::cout << "+\n  ";
        for (int row = 0; row < 10; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (col == 2) {
                    std::cout << static_cast<char>(97 + row);
                } else {
                    std::cout << " ";
                }
            }
        }
        std::cout << " \n";

        if (maybeWins == State::wins) {
            std::cout << currGame.whoPlayer_to_char() << " wins!\n";
        } else {
            std::cout << currGame.whoPlayer_to_char() << " move: ";
        }
    }

    [[nodiscard]] bool existExit() const final {
        return false;
    }
};

}  // namespace tictactoe

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)
extern "C" BOOST_SYMBOL_EXPORT tictactoe::view_pretty Views;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors)
tictactoe::view_pretty Views;
