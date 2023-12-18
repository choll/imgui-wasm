#ifndef CHOLL_IMGUI_WASM_QUERY_WINDOW_HPP
#define CHOLL_IMGUI_WASM_QUERY_WINDOW_HPP

#include "websocket.hpp"
#include "window.hpp"

struct query_window : window
{
    query_window();

    void draw() final;

    char query_text_[256] = "";
    websocket ws_;
};

#endif
