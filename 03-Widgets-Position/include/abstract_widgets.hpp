#ifndef ABSTRACT_WIDGETS_HPP_
#define ABSTRACT_WIDGETS_HPP_
#include <memory>

namespace widgets {
class container;

class widget {
public:
    [[nodiscard]] virtual int width() const = 0;
    [[nodiscard]] virtual int height() const = 0;
    virtual widget *child_at(int, int);

    widget(const widget &) = delete;
    widget(widget &&) = delete;
    widget &operator=(const widget &) = delete;
    widget &operator=(widget &&) = delete;
    virtual ~widget() = default;

    friend class container;
    [[nodiscard]] container *parent() const;

protected:
    widget() = default;

private:
    container *m_parent = nullptr;
};

class container : public widget {
public:
    virtual void update_layout() = 0;
    static void set_parent(widget *child, widget *parent);
};
}  // namespace widgets
#endif  // ABSTRACT_WIDGETS_HPP_
