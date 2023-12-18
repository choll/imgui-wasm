#ifndef CHOLL_IMGUI_WASM_WINDOW_HPP
#define CHOLL_IMGUI_WASM_WINDOW_HPP

#include <imgui.h>

#include <cstddef>
#include <string>

struct window
{
    window(std::string_view title, ImVec2 init_size, ImVec2 size_min, ImVec2 size_max);

    virtual void draw();
    virtual void end_draw();

    virtual ~window() = default;

    std::string title_;
    ImVec2 init_size_;
    ImVec2 size_min_;
    ImVec2 size_max_;
    bool open_ = true;
    // init_ is only true for the first call to draw()
    bool init_ = true;
    static inline std::size_t n_;
};

#endif
