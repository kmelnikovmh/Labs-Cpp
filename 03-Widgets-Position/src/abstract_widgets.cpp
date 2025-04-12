#include "../include/abstract_widgets.hpp"

namespace widgets {
widget *widget::child_at(int x, int y) {
    if (0 <= x && x < width() && 0 <= y && y < height()) {
        return this;
    }
    return nullptr;
}

container *widget::parent() const {
    return m_parent;
}

void container::set_parent(widget *child, widget *parent) {
    child->m_parent = dynamic_cast<container *>(parent);
}
}  // namespace widgets
