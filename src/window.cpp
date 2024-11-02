#include "window.hpp"

#include <fmt/core.h>

window::window(std::string_view title, ImVec2 init_size, ImVec2 size_min, ImVec2 size_max)
:
    title_(fmt::format("{}###{}", title, ++n_)),
    init_size_(init_size),
    size_min_(size_min),
    size_max_(size_max)
{
}

void window::draw()
{
    ImGui::SetNextWindowSizeConstraints(size_min_, size_max_);
    ImGui::SetNextWindowSize(init_size_, ImGuiCond_FirstUseEver);
    ImGui::Begin(title_.c_str(), &open_);
}

void window::end_draw()
{
    init_ = false;
}
