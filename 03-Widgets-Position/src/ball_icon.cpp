#include "../include/ball_icon.hpp"
#include <memory>

namespace widgets {

ball_icon::ball_icon(int x) : m_radius(x) {
}

[[nodiscard]] std::unique_ptr<ball_icon> make_ball_icon(int x) {
    return std::make_unique<ball_icon>(x);
}

int ball_icon::width() const {
    return 2 * m_radius + 1;
}

int ball_icon::height() const {
    return 2 * m_radius + 1;
}

widget* ball_icon::child_at(int x, int y) {
    const int ox = x - m_radius;
    const int oy = y - m_radius;
    if (ox * ox + oy * oy <= m_radius * m_radius) {
        return this;
    } else {
        return nullptr;
    }
}

int ball_icon::radius() const {
    return m_radius;
}

void ball_icon::radius(int new_radius) {
    m_radius = new_radius;
}
}  // namespace widgets
