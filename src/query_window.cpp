#include "query_window.hpp"

#include <imgui.h>

//#include <iostream>

query_window::query_window()
:
    window(
        "Query",
        ImVec2(520, 600),
        ImVec2(300, 200),
        ImVec2(FLT_MAX, FLT_MAX))
{
    ws_.connect("ws://localhost:12345");
}

void query_window::draw()
{
    window::draw();

    //ImGui::Text("Hello from another window!");
    //if (ImGui::Button("Close Me"))
    //    open_ = false;
    //ImGui::Text("Query:");
    //ImGui::SameLine();

    ImGui::InputTextWithHint(
        nullptr,
        "Enter query terms: field1=value1 field2=value2 ...",
        query_text_,
        sizeof(query_text_));

    //std::cout << query_text_ << "\n";

    //ws_.send(query_text_);

    constexpr ImGuiTableFlags flags =
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_BordersOuter |
        ImGuiTableFlags_BordersV |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_Reorderable |
        ImGuiTableFlags_Hideable;

    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

    // When using ScrollX or ScrollY we need to specify a size for our table container!
    // Otherwise by default the table will fit all available space, like a BeginChild() call.
    //ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
    ImVec2 outer_size = ImGui::GetContentRegionAvail();
    outer_size.y -= TEXT_BASE_HEIGHT;
    if (ImGui::BeginTable("table_scrolly", 4, flags, outer_size))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("One", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Two", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Three", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Four", ImGuiTableColumnFlags_None);
        if (init_)
            ImGui::TableSetColumnEnabled(3, false);
        ImGui::TableHeadersRow();

        // Demonstrate using clipper for large vertical lists
        ImGuiListClipper clipper;
        clipper.Begin(500000);
        while (clipper.Step())
        {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd;
                 row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("Hello %d,%d", column, row);
                }
            }
        }
        ImGui::EndTable();
    }

    ImGui::End();
}
