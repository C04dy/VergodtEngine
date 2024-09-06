#include "Viewport.h"
#include <math.h>
#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "App.h"

Viewport::Viewport() {

}

Viewport::~Viewport() {

}

bool LoadTextureFromFile(const std::string& FilePath, SDL_Texture** texture_ptr, float& width, float& height, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(FilePath.c_str());

    if(surface == nullptr){
        std::cout << "Failed to load texture: " << FilePath.c_str() << '\n';
        return false;
    }

    *texture_ptr = SDL_CreateTextureFromSurface(renderer, surface);

    if(*texture_ptr == nullptr){
        std::cout << "Failed to create texture from surface\n";
        return false;
    }


    width = surface->w;
    height = surface->h;

    SDL_DestroySurface(surface);

    return true;
}

void Viewport::ViewportSpace(SDL_Renderer* renderer, const std::vector<Node>& nodes) {
    ImGuiIO& io = ImGui::GetIO();

    if (!ImGui::Begin("Viewport")) {
        ImGui::End();
        return;
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    bool show_grid = true;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PopStyleVar(); // WindowPadding
    ImGui::PushItemWidth(120.0f);

    ImVec2 offset;
    offset.x = ImGui::GetCursorScreenPos().x + m_scrolling.x;
    offset.y = ImGui::GetCursorScreenPos().y + m_scrolling.y;

    if (show_grid) {
        ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
        float GRID_SZ = 64.0f;
        ImVec2 win_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_sz = ImGui::GetWindowSize();
        for (float x = fmodf(m_scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            draw_list->AddLine(ImVec2(x + win_pos.x, win_pos.y), ImVec2(x + win_pos.x, canvas_sz.y + win_pos.y), GRID_COLOR);
        for (float y = fmodf(m_scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            draw_list->AddLine(ImVec2(win_pos.x, y + win_pos.y), ImVec2(canvas_sz.x + win_pos.x, y + win_pos.y), GRID_COLOR);
    }

    for (size_t i = 0; i < nodes.size(); i++) {
        switch (nodes[i].NodeType)
        {
        case Node::Type::SPRITE:
            SDL_Texture* my_tex_id;
            float my_tex_w, my_tex_h;
            if (LoadTextureFromFile(*(std::string*)nodes[i].NodeValues[0].Value, &my_tex_id, my_tex_w, my_tex_h, renderer)) {
                static bool use_text_color_for_tint = false;
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);
                ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
                ImGui::SetCursorPos(ImVec2(nodes[i].Position.x + offset.x, nodes[i].Position.y + offset.y));
                ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
            }
            break;
        case Node::Type::PHYSICSBODY:
            ImGui::SetCursorPos(ImVec2(nodes[i].Position.x + offset.x, nodes[i].Position.y + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("PHYSICSBODY", ImVec2(50, 50));
            ImGui::Text("PhysicsBody");
            ImGui::EndGroup();
            break;
        case Node::Type::NODE:
            ImGui::SetCursorPos(ImVec2(nodes[i].Position.x + offset.x, nodes[i].Position.y + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("NODE", ImVec2(50, 50));
            ImGui::Text("Node");
            ImGui::EndGroup();
            break;
        case Node::Type::CAM:
            ImGui::SetCursorPos(ImVec2(nodes[i].Position.x + offset.x, nodes[i].Position.y + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("CAM", ImVec2(50, 50));
            ImGui::Text("Cam");
            ImGui::EndGroup();
            break;
        }
    }
    draw_list->ChannelsMerge();


    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle, 0.0f)) {
        m_scrolling.x = m_scrolling.x + io.MouseDelta.x;
        m_scrolling.y = m_scrolling.y + io.MouseDelta.y;
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::End();
}
