#include "../include/box.hpp"
#include "../include/placeholder.hpp"
#include "doctest.h"

TEST_CASE("horizontal box works and mutates") {
    widgets::box bx(widgets::box::kind::HORIZONTAL);
    auto* pa = dynamic_cast<widgets::placeholder*>(bx.add(widgets::make_placeholder(10, 15)));

    CHECK(std::as_const(bx).get(0) == pa);

    pa->m_height = 50;
    static_cast<widgets::container&>(bx).update_layout();

    // ...
}

TEST_CASE("box inside box") {
    widgets::box bx(widgets::box::kind::VERTICAL);
    widgets::widget* bx2 = bx.add(widgets::make_box(widgets::box::kind::HORIZONTAL));

    CHECK(bx2->parent() == &bx);

    // ...
}
