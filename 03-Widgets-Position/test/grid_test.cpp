#include "../include/grid.hpp"
#include "../include/placeholder.hpp"
#include "doctest.h"

// ...

TEST_CASE("grid works and mutates") {
    widgets::grid gr(2, 3);
    auto* pa = dynamic_cast<widgets::placeholder*>(gr.add(widgets::make_placeholder(10, 15), 0, 0));

    CHECK(std::as_const(gr).get(0, 0) == pa);
    CHECK(std::as_const(*pa).parent() == &gr);
    CHECK(gr.child_at(4, 5) == pa);

    pa->m_height = 30;
    static_cast<widgets::container&>(gr).update_layout();

    // ...
}
