#include "../include/abstract_widgets.hpp"
#include "../include/ball_icon.hpp"
#include "../include/box.hpp"
#include "../include/button.hpp"
#include "../include/grid.hpp"
#include "../include/placeholder.hpp"
#include "doctest.h"

// ...

TEST_CASE("Combine inside a grid inside a box") {
    auto bx1 = widgets::make_box(widgets::box::kind::HORIZONTAL);
    auto* ball = bx1->add(widgets::make_ball_icon(4));
    bx1->add(widgets::make_placeholder(0, 101));

    widgets::grid gr(2, 2);
    gr.add(std::move(bx1), 0, 0);

    CHECK(gr.child_at(0, 50) == ball);

    // ...
}
