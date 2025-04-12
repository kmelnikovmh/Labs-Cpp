#ifndef GAME_
#define GAME_
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace tictactoe {
enum class OptionalPlayer { X, O };

enum class State { exit, bad, nice, wins };

class Game {
    OptionalPlayer currentPlayer;
    std::vector<std::vector<int>> field;
    State Move(int y, int x);
    [[nodiscard]] int rightMove(int y, int x) const;
    [[nodiscard]] bool maybeWins(int y, int x) const;
    [[nodiscard]] bool maybeWinsHorizVertic(int y, int x) const;
    [[nodiscard]] bool maybeWinsDiag(int y, int x) const;
    void Counter(int &count, int i, int j) const;

public:
    Game();
    [[nodiscard]] int what_in_cell(int y, int x) const;
    [[nodiscard]] char whoPlayer_to_char() const;
    State oneCircle(const std::tuple<int, int, int> &tuple, bool existExit);
};
}  // namespace tictactoe
#endif
