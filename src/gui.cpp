#include "gui.hpp"

#include "settings_window.hpp"
#include "query_window.hpp"
#include "window.hpp"

#include <imgui.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace
{
    std::vector<std::unique_ptr<window>> windows;
}

void draw()
{
    ImGui::ShowDemoWindow();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Query Window", "N"))
                windows.push_back(std::make_unique<query_window>());
            if (ImGui::MenuItem("Light mode"))
                ImGui::StyleColorsLight();
            if (ImGui::MenuItem("Dark mode"))
                ImGui::StyleColorsDark();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
            windows.push_back(std::make_unique<settings_window>());
        ImGui::SameLine(ImGui::GetWindowWidth() - 60);
        ImGui::Text("%.0f FPS", ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }

    std::erase_if(windows, [](auto& win) { return !win->open_; });

    for (auto& win : windows)
    {
        win->draw();
        win->end_draw();
    }
}
