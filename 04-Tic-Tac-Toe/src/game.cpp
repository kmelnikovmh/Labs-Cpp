#include <game.hpp>

namespace tictactoe {
Game::Game()
    : currentPlayer(OptionalPlayer::O),
      field(std::vector<std::vector<int>>(10, std::vector(10, -1))) {
}

int Game::what_in_cell(int y, int x) const {
    return field[y][x];
}

char Game::whoPlayer_to_char() const {
    if (currentPlayer == OptionalPlayer::O) {
        return 'O';
    }
    return 'X';
}

State Game::Move(int y, int x) {
    if (currentPlayer == OptionalPlayer::O) {
        field[y][x] = 0;
        if (maybeWins(y, x)) {
            return State::wins;
        }
        currentPlayer = OptionalPlayer::X;
    } else {
        field[y][x] = 1;
        if (maybeWins(y, x)) {
            return State::wins;
        }
        currentPlayer = OptionalPlayer::O;
    }
    return State::nice;
}

int Game::rightMove(int y, int x) const {
    if (field[y][x] == -1) {
        return 1;
    }
    return 0;
}

bool Game::maybeWins(int y, int x) const {
    return maybeWinsHorizVertic(y, x) || maybeWinsDiag(y, x);
}

bool Game::maybeWinsHorizVertic(int y, int x) const {
    int countHoriz = 0;
    for (int i = x - 4; i < x + 5; ++i) {
        if (i < 0 || i > 9) {
            continue;
        }
        if ((currentPlayer == OptionalPlayer::O && field[y][i] == 0) ||
            (currentPlayer == OptionalPlayer::X && field[y][i] == 1)) {
            ++countHoriz;
        } else {
            countHoriz = 0;
        }
        if (countHoriz >= 5) {
            return true;
        }
    }
    int countVertic = 0;
    for (int i = y - 4; i < y + 5; ++i) {
        if (i < 0 || i > 9) {
            continue;
        }
        if ((currentPlayer == OptionalPlayer::O && field[i][x] == 0) ||
            (currentPlayer == OptionalPlayer::X && field[i][x] == 1)) {
            ++countVertic;
        } else {
            countVertic = 0;
        }
        if (countVertic >= 5) {
            return true;
        }
    }
    return false;
}

void Game::Counter(int& count, int i, int j) const {
    if ((currentPlayer == OptionalPlayer::O && field[i][j] == 0) ||
        (currentPlayer == OptionalPlayer::X && field[i][j] == 1)) {
        ++count;
    } else {
        count = 0;
    }
}

bool Game::maybeWinsDiag(int y, int x) const {
    int countDiag = 0;
    int i = y - 4;
    int j = x - 4;
    while (i < y + 5 && j < x + 5) {
        if (i < 0 || i > 9 || j < 0 || j > 9) {
            ++i;
            ++j;
            continue;
        }
        Counter(countDiag, i, j);
        if (countDiag >= 5) {
            return true;
        }
        ++i;
        ++j;
    }
    countDiag = 0;
    i = y + 4;
    j = x - 4;
    while (i > y - 5 && j < x + 5) {
        if (i < 0 || i > 9 || j < 0 || j > 9) {
            --i;
            ++j;
            continue;
        }
        Counter(countDiag, i, j);
        if (countDiag >= 5) {
            return true;
        }
        --i;
        ++j;
    }
    return false;
}

State Game::oneCircle(const std::tuple<int, int, int>& tuple, bool existExit) {
    auto [y, x, remainStr] = tuple;
    if (existExit && x == -1 && y == -1) {
        return State::exit;
    }
    if (remainStr != 0 || x < 0 || x > 9 || y < 0 || y > 9 || (rightMove(y, x)) == 0) {
        std::cout << "Bad move!\n" << whoPlayer_to_char() << " move: ";
        return State::bad;
    }
    return Move(y, x);
}
}  // namespace tictactoe
