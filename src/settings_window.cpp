#include "settings_window.hpp"

#include <imgui.h>

settings_window::settings_window()
:
    window(
        "Settings",
        ImVec2(520, 600),
        ImVec2(300, 200),
        ImVec2(FLT_MAX, FLT_MAX))
{
}

void settings_window::draw()
{
    window::draw();

    ImGuiIO& io = ImGui::GetIO();
    ImFont* font_current = ImGui::GetFont();
    if (ImGui::BeginCombo("Font", font_current->GetDebugName()))
    {
        for (ImFont* font : io.Fonts->Fonts)
        {
            ImGui::PushID(font);
            if (ImGui::Selectable(font->GetDebugName(), font == font_current))
                io.FontDefault = font;
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
}
