#ifndef VIEW_
#define VIEW_
#include <boost/config.hpp>
#include <game.hpp>

namespace tictactoe {

class BOOST_SYMBOL_VISIBLE View {
public:
    View(const View &) = default;
    View(View &&) = default;
    View &operator=(const View &) = default;
    View &operator=(View &&) = default;
    virtual ~View() = default;
    View() = default;
    virtual std::tuple<int, int, int> readCoord(std::string &str) const = 0;

    virtual void printField(Game &currGame, State &maybeWins, bool swift)
        const = 0;

    [[nodiscard]] virtual bool existExit() const = 0;
};
}  // namespace tictactoe

#endif
