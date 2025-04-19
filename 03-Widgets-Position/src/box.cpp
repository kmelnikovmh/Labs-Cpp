#include "../include/box.hpp"
#include <algorithm>
#include <memory>

namespace widgets {
box::box(box::kind orientation) : m_orientation(orientation), m_width(0), m_height(0) {
}

widget* box::add(std::unique_ptr<widget> ptr) {
    m_children.push_back(std::move(ptr));
    set_parent(m_children.back().get(), this);
    if (m_orientation == kind::VERTICAL) {
        m_width = std::max(m_width, m_children.back()->width());
        m_height += m_children.back()->height();
    } else {
        m_width += m_children.back()->width();
        m_height = std::max(m_height, m_children.back()->height());
    }
    return m_children.back().get();
}

std::unique_ptr<widget> box::remove(int index) {
    if (m_children[index].get() == nullptr) {
        return nullptr;
    }
    set_parent(m_children[index].get(), nullptr);
    std::unique_ptr<widget> ptr = std::move(m_children[index]);
    m_children.erase(m_children.begin() + index);
    if (m_orientation == kind::VERTICAL) {
        update_width();
        m_height -= ptr->height();
    } else {
        m_width -= ptr->width();
        update_height();
    }
    return ptr;
}

widget* box::get(int index) const {
    return m_children[index].get();
}

int box::size() const {
    return static_cast<int>(m_children.size());
}

int box::width() const {
    return m_width;
}

int box::height() const {
    return m_height;
}

void box::update_width() {
    int curr_width = 0;
    if (m_orientation == kind::HORIZONTAL) {
        for (const auto& ptr : m_children) {
            // cppcheck-suppress[useStlAlgorithm]
            curr_width += (*ptr).width();
        }
    } else {
        for (const auto& ptr : m_children) {
            curr_width = std::max((*ptr).width(), curr_width);
        }
    }
    m_width = curr_width;
}

void box::update_height() {
    int curr_height = 0;
    if (m_orientation == kind::VERTICAL) {
        for (const auto& ptr : m_children) {
            // cppcheck-suppress[useStlAlgorithm]
            curr_height += (*ptr).height();
        }
    } else {
        for (const auto& ptr : m_children) {
            curr_height = std::max((*ptr).height(), curr_height);
        }
    }
    m_height = curr_height;
}

void box::update_layout() {
    this->update_width();
    this->update_height();
}

widget* box::child_atVERT(const int& x, const int& y) {
    int currY = 0;
    for (const auto& ptr : m_children) {
        const int ptr_height = (*ptr).height();
        const int ptr_width = (*ptr).width();
        if (currY <= y && y < currY + ptr_height) {
            const int leftline = (m_width - ptr_width) / 2;
            const int rightline = (m_width + ptr_width) / 2;
            if (leftline <= x && x <= rightline) {
                return (*ptr).child_at(x - leftline, y - currY);
            }
            return nullptr;
        }
        currY += ptr_height;
    }
    return nullptr;
}

widget* box::child_atHORI(const int& x, const int& y) {
    int currX = 0;
    for (const auto& ptr : m_children) {
        const int ptr_height = (*ptr).height();
        const int ptr_width = (*ptr).width();
        if (currX <= x && x < currX + ptr_width) {
            const int upline = (m_height - ptr_height) / 2;
            const int downline = (m_height + ptr_height) / 2;
            if (upline <= y && y <= downline) {
                return (*ptr).child_at(x - currX, y - upline);
            }
            return nullptr;
        }
        currX += ptr_width;
    }
    return nullptr;
}

widget* box::child_at(int x, int y) {
    widget* curr_ptr = nullptr;
    if (this->widget::child_at(x, y) == nullptr) {
        return nullptr;
    }
    if (m_orientation == kind::VERTICAL) {
        curr_ptr = box::child_atVERT(x, y);
    } else {
        curr_ptr = box::child_atHORI(x, y);
    }
    return curr_ptr;
}

std::unique_ptr<box> make_box(box::kind curr_box) {
    return std::make_unique<box>(curr_box);
}
}  // namespace widgets
