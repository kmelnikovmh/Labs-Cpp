#ifndef BOX_
#define BOX_
#include <memory>
#include <vector>
#include "abstract_widgets.hpp"

namespace widgets {
class box final : public container {
public:
    enum class kind { HORIZONTAL, VERTICAL };
    explicit box(box::kind);

    widget* add(std::unique_ptr<widget>);
    std::unique_ptr<widget> remove(int);

    [[nodiscard]] widget* get(int) const;
    [[nodiscard]] int size() const;
    [[nodiscard]] int width() const final;
    [[nodiscard]] int height() const final;

    void update_width();
    void update_height();
    void update_layout() final;

    widget* child_at(int, int) final;
    widget* child_atVERT(const int&, const int&);
    widget* child_atHORI(const int&, const int&);

private:
    box::kind m_orientation;
    int m_width;
    int m_height;
    std::vector<std::unique_ptr<widget>> m_children;
};

std::unique_ptr<box> make_box(box::kind);
}  // namespace widgets
#endif
