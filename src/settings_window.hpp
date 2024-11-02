#ifndef CHOLL_IMGUI_WASM_SETTINGS_WINDOW_HPP
#define CHOLL_IMGUI_WASM_SETTINGS_WINDOW_HPP

#include "window.hpp"

struct settings_window : window
{
    settings_window();

    void draw() final;
};

#endif
