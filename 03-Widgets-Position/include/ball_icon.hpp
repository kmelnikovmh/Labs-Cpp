#ifndef BALL_ICON_
#define BALL_ICON_
#include <memory>
#include "abstract_widgets.hpp"

namespace widgets {
class ball_icon final : public widget {
public:
    explicit ball_icon(int);

    [[nodiscard]] int width() const final;
    [[nodiscard]] int height() const final;
    widget* child_at(int x, int y) final;

    [[nodiscard]] int radius() const;
    void radius(int new_radius);

private:
    int m_radius;
};

[[nodiscard]] std::unique_ptr<ball_icon> make_ball_icon(int);
}  // namespace widgets
#endif
