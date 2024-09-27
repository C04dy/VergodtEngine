#include "Viewport.h"
#include "StringFunctions.h"
#include <math.h>
#include <string>
#include <algorithm>
#include <iostream>
#include "imgui_internal.h"
#include "App.h"
#include "SDL3/SDL.h"

static bool IsMouseHoveringPolyline(const ImVec2 Points[], int PointCount, float LineThickness)
{
    int crossings = 0;

    for (int i = 0; i < PointCount; i++)
    {
        ImVec2 p1 = Points[i];
        ImVec2 p2 = Points[(i + 1) % PointCount];

        if (((p1.y > ImGui::GetMousePos().y) != (p2.y > ImGui::GetMousePos().y)) && (ImGui::GetMousePos().x < (p2.x - p1.x) * (ImGui::GetMousePos().y - p1.y) / (p2.y - p1.y) + p1.x))
            crossings++;
    }

    return (crossings % 2) != 0;
}

static float DistanceBetween(ImVec2 a, ImVec2 b)
{
    return std::sqrt(std::powf(a.x - b.x, 2.0f) + std::powf(a.y - b.y, 2.0f));
}

static bool IsMouseHoveringCircle(float Radius, const ImVec2& Position)
{
    return DistanceBetween(Position, ImGui::GetMousePos()) < Radius;
}

static void DeleteChildNodes(std::vector<Node>& Nodes, std::vector<int>& ChildNodes, std::vector<int>& IDsToDelete)
{
    for (int i = 0; i < static_cast<int>(ChildNodes.size()); i++)
    {
        IDsToDelete.push_back(ChildNodes[i]);
        for (int j = 0; j < static_cast<int>(Nodes.size()); j++)
            if (Nodes[j].ID == ChildNodes[i])
                DeleteChildNodes(Nodes, Nodes[j].ChildIDs, IDsToDelete);
    }
}

void Viewport::ViewportSpace(SDL_Renderer* Renderer, std::vector<Node>& Nodes, int& SelectedNode, bool& Saved, Project& CurrentProject)
{
    ImGuiIO& io = ImGui::GetIO();

    ImGuiWindowFlags flags = (Saved ? ImGuiWindowFlags_None : ImGuiWindowFlags_UnsavedDocument) | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
    if (!ImGui::Begin("Viewport", NULL, flags))
    {
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
    offset.x = m_Scrolling.x * m_Zoom;
    offset.y = m_Scrolling.y * m_Zoom;
    ImVec2 draw_list_offset;
    draw_list_offset.x = ImGui::GetCursorScreenPos().x + m_Scrolling.x * m_Zoom;
    draw_list_offset.y = ImGui::GetCursorScreenPos().y + m_Scrolling.y * m_Zoom;

    ImGui::SetScrollY(0);
    if (show_grid)
    {
        ImU32 grid_color = IM_COL32(200, 200, 200, 40);
        float grid_size = 64.0f * m_Zoom;
        ImVec2 window_position = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetWindowSize();
        for (float x = fmodf(m_Scrolling.x * m_Zoom, grid_size); x < canvas_size.x; x += grid_size)
            draw_list->AddLine(ImVec2(x + window_position.x, window_position.y), ImVec2(x + window_position.x, canvas_size.y + window_position.y), grid_color);
        for (float y = fmodf(m_Scrolling.y * m_Zoom, grid_size); y < canvas_size.y; y += grid_size)
            draw_list->AddLine(ImVec2(window_position.x, y + window_position.y), ImVec2(canvas_size.x + window_position.x, y + window_position.y), grid_color);
    }
    
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
        SelectedNode = -1;

    for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
    {
        ImGui::PushID(i);
        switch (Nodes[i].NodeType)
        {
        case Node::Type::SPRITE:
            if (Nodes[i].Texture == nullptr)
            {
                ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x, (Nodes[i].Position.y * m_Zoom) + offset.y));
                ImGui::BeginGroup();
                ImGui::InvisibleButton("SPRITE", ImVec2(50, 50), ImGuiButtonFlags_MouseButtonLeft);
                if (ImGui::IsItemActivated())
                    SelectedNode = i;
                ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x, (Nodes[i].Position.y * m_Zoom) + offset.y));
                ImGui::Text("Sprite");
                ImGui::EndGroup();
            }
            else
            {
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);
                ImVec4 tint_col = false ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
                if (i == SelectedNode) {
                    ImVec2 minrec((Nodes[i].Position.x * m_Zoom) + draw_list_offset.x, (Nodes[i].Position.y * m_Zoom) + draw_list_offset.y);
                    draw_list->AddRect(ImVec2(minrec.x - ((Nodes[i].TextureWidth * Nodes[i].Size.x) / 2 * m_Zoom), minrec.y - ((Nodes[i].TextureHeight * Nodes[i].Size.y) / 2 * m_Zoom)),
                                       ImVec2(minrec.x + ((((Nodes[i].TextureWidth * Nodes[i].Size.x) / 2) + 2.5f) * m_Zoom), minrec.y + (((Nodes[i].TextureHeight * Nodes[i].Size.y) / 2) + 2.5f) * m_Zoom), IM_COL32(255, 99, 71, 255), 0.0f, ImDrawFlags_None, 5 * m_Zoom);
                }
                ImGui::BeginGroup();
                ImGui::SetCursorPos(ImVec2(((Nodes[i].Position.x  * m_Zoom) + offset.x - (((Nodes[i].TextureWidth * Nodes[i].Size.x) / 2)) * m_Zoom), ((Nodes[i].Position.y * m_Zoom) + offset.y - (((Nodes[i].TextureHeight * Nodes[i].Size.y) / 2)) * m_Zoom)));
                
                ImGui::Image(Nodes[i].Texture, ImVec2(Nodes[i].TextureWidth * Nodes[i].Size.x * m_Zoom, Nodes[i].TextureHeight * Nodes[i].Size.y * m_Zoom), uv_min, uv_max, tint_col, border_col);

                ImGui::SetCursorPos(ImVec2(ImVec2(((Nodes[i].Position.x * m_Zoom) + offset.x - (((Nodes[i].TextureWidth * Nodes[i].Size.x) / 2)) * m_Zoom), ((Nodes[i].Position.y * m_Zoom) + offset.y - (((Nodes[i].TextureHeight * Nodes[i].Size.y) / 2)) * m_Zoom))));
                ImGui::InvisibleButton("SPRITE", ImVec2(Nodes[i].TextureWidth * Nodes[i].Size.x * m_Zoom, Nodes[i].TextureHeight * Nodes[i].Size.y * m_Zoom), ImGuiButtonFlags_MouseButtonLeft);
                if (ImGui::IsItemActivated())
                    SelectedNode = i;
                ImGui::EndGroup();
            }
            break;
        case Node::Type::PHYSICSBODY:
            ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x, (Nodes[i].Position.y * m_Zoom) + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("PHYSICSBODY", ImVec2(50, 50), ImGuiButtonFlags_MouseButtonLeft);
            if (ImGui::IsItemActivated())
                SelectedNode = i;
            ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x, (Nodes[i].Position.y * m_Zoom) + offset.y));
            ImGui::Text("PhysicsBody");
            ImGui::EndGroup();
            break;
        case Node::Type::NODE:
            ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x, (Nodes[i].Position.y * m_Zoom) + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("NODE", ImVec2(50, 50), ImGuiButtonFlags_MouseButtonLeft);
            if (ImGui::IsItemActivated())
                SelectedNode = i;
            ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x, (Nodes[i].Position.y * m_Zoom) + offset.y));
            ImGui::Text("Node");
            ImGui::EndGroup();
            break;
        case Node::Type::CAM:
            ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x, (Nodes[i].Position.y * m_Zoom) + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("CAM", ImVec2(50, 50), ImGuiButtonFlags_MouseButtonLeft);
            if (ImGui::IsItemActivated())
                SelectedNode = i;
            ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x, (Nodes[i].Position.y * m_Zoom) + offset.y));
            ImGui::Text("Cam");
            ImGui::EndGroup();
            break;
        case Node::Type::COLLIDER: {
            ImGui::BeginGroup();

            switch (*(Node::ColliderType*)Nodes[i].NodeValues[0]->Value)
            {
            case Node::ColliderType::BOX: {
                ImVec2 minrec((Nodes[i].Position.x * m_Zoom) + draw_list_offset.x , (Nodes[i].Position.y * m_Zoom) + draw_list_offset.y);

                draw_list->AddRect(ImVec2(minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)),
                                   ImVec2(minrec.x + ((((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2) + 2.5f) * m_Zoom), minrec.y + ((((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2) + 2.5f) * m_Zoom)), IM_COL32(255, 99, 71, 255), 0.0f, ImDrawFlags_None, 5 * m_Zoom);


                static int selected_point = -1;
                static int selected_ID = -1;
                draw_list->AddCircleFilled(ImVec2(minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 1;
                    selected_ID = Nodes[i].ID;
                }
                if (selected_point == 1 && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && selected_ID == Nodes[i].ID)
                {
                    float new_x = ((minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x += new_x;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->x <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x = 0.1f;
                    else
                        Nodes[i].Position.x -= new_x / 2;
                    Saved = false;
                }

                draw_list->AddCircleFilled(ImVec2(minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 2;
                    selected_ID = Nodes[i].ID;
                }
                if (selected_point == 2 && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && selected_ID == Nodes[i].ID)
                {
                    float new_x = ((minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x += new_x;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->x <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x = 0.1f;
                    else
                        Nodes[i].Position.x -= new_x / 2;

                    float new_y = ((minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y += new_y;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->y <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y = 0.1f;
                    else
                        Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }
                
                draw_list->AddCircleFilled(ImVec2(minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 3;
                    selected_ID = Nodes[i].ID;
                }
                if (selected_point == 3 && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && selected_ID == Nodes[i].ID)
                {
                    float new_x = ((minrec.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x += new_x;
                    
                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->x <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x = 0.1f;
                    else
                        Nodes[i].Position.x -= new_x / 2;

                    float new_y = ((minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y -= new_y;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->y <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y = 0.1f;
                    else
                        Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }
                
                draw_list->AddCircleFilled(ImVec2(minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 4;
                    selected_ID = Nodes[i].ID;
                }
                if (selected_point == 4 && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && selected_ID == Nodes[i].ID)
                {
                    float new_x = ((minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x -= new_x;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->x <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x = 0.1f;
                    else
                        Nodes[i].Position.x -= new_x / 2;
                    Saved = false;
                }
                
                draw_list->AddCircleFilled(ImVec2(minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 5;
                    selected_ID = Nodes[i].ID;
                }
                if (selected_point == 5 && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && selected_ID == Nodes[i].ID)
                {
                    float new_x = ((minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x -= new_x;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->x <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x = 0.1f;
                    else
                        Nodes[i].Position.x -= new_x / 2;

                    float new_y = ((minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y += new_y;
                    
                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->y <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y = 0.1f;
                    else
                        Nodes[i].Position.y -= new_y / 2;
                    
                    Saved = false;
                }

                draw_list->AddCircleFilled(ImVec2(minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 6;
                    selected_ID = Nodes[i].ID;
                }
                if (selected_point == 6 && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && selected_ID == Nodes[i].ID)
                {
                    float new_x = ((minrec.x + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x -= new_x;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->x <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x = 0.1f;
                    else
                        Nodes[i].Position.x -= new_x / 2;

                    float new_y = ((minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y -= new_y;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->y <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y = 0.1f;
                    else
                        Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }
                
                draw_list->AddCircleFilled(ImVec2(minrec.x, minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(minrec.x, minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 7;
                    selected_ID = Nodes[i].ID;
                }
                if (selected_point == 7 && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && selected_ID == Nodes[i].ID)
                {
                    float new_y = ((minrec.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y += new_y;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->y <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y = 0.1f;
                    else
                        Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }

                draw_list->AddCircleFilled(ImVec2(minrec.x, minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(minrec.x, minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 8;
                    selected_ID = Nodes[i].ID;
                }
                if (selected_point == 8 && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && selected_ID == Nodes[i].ID)
                {
                    float new_y = ((minrec.y + ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y -= new_y;

                    if (((ImVec2*)Nodes[i].NodeValues[1]->Value)->y <= 0.0f)
                        ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y = 0.1f;
                    else
                        Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }

                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && selected_point != -1)
                {
                    selected_point = -1;
                    selected_ID = -1;
                }


                ImGui::SetCursorPos(ImVec2((Nodes[i].Position.x * m_Zoom) + offset.x - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x / 2 * m_Zoom), (Nodes[i].Position.y * m_Zoom) + offset.y - ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y / 2 * m_Zoom)));
                ImVec2 button_size = ImVec2(((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x * m_Zoom), ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y * m_Zoom));
                if (button_size.x == 0)
                    button_size.x = 1;
                if (button_size.y == 0)
                    button_size.y = 1;
                ImGui::InvisibleButton("COLLIDER", button_size, ImGuiButtonFlags_MouseButtonLeft);
                if (ImGui::IsItemActivated() && selected_point == -1)
                    SelectedNode = i;
            }   break;
            case Node::ColliderType::CIRCLE: {
                ImVec2 minrec((Nodes[i].Position.x * m_Zoom) + draw_list_offset.x, (Nodes[i].Position.y * m_Zoom) + draw_list_offset.y);
                draw_list->AddEllipse(minrec, ImVec2((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.x * m_Zoom, (*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y * m_Zoom), IM_COL32(255, 99, 71, 255), 0.0f, 0, 5 * m_Zoom);

                draw_list->AddCircleFilled(ImVec2(minrec.x , minrec.y - ((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y * m_Zoom) - 2.5f), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));

                static int resizing = -1;
                if (IsMouseHoveringCircle(5.0f * m_Zoom, ImVec2(minrec.x , minrec.y - ((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y * m_Zoom) - 2.5f)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    resizing = i;
                if (resizing == i && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float* radius = (float*)Nodes[i].NodeValues[1]->Value;

                    float new_y = ((minrec.y - ((((*radius * Nodes[i].Size.y)) - 2.5f) * m_Zoom)) - ImGui::GetMousePos().y) / 2;
                    *radius += new_y;

                    if (*radius <= 0.0f)
                        *radius = 0.1f;
                    else
                        Nodes[i].Position.y -= new_y;
                    Saved = false;
                }
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && resizing != -1)
                    resizing = -1;

                if (IsMouseHoveringCircle((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.x * m_Zoom, minrec) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && resizing == -1)
                    SelectedNode = i;
            }   break;
            case Node::ColliderType::POLYGONS: {
                ImVec2 points[Nodes[i].NodeValues[1]->VectorValues.size() + 1];

                for (int j = 0; j < static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()); j++)
                {
                    points[j] = ImVec2(((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x * m_Zoom) + draw_list_offset.x + (Nodes[i].Position.x * m_Zoom),
                                       (-((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y) * m_Zoom) + draw_list_offset.y + (Nodes[i].Position.y * m_Zoom));
                }

                draw_list->AddPolyline(points, static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()), IM_COL32(255, 99, 71, 255), ImDrawFlags_Closed, 5 * m_Zoom);

                static int hovered_point = -1;
                static int hovered_ID = -1;
                bool hovering_any_point = false;
                for (int j = 0; j < static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()); j++)
                {
                    if (IsMouseHoveringCircle(10 * m_Zoom, ImVec2(((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x * m_Zoom) + (Nodes[i].Position.x * m_Zoom) + draw_list_offset.x, (-(*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y * m_Zoom) + (Nodes[i].Position.y * m_Zoom) + draw_list_offset.y)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        hovered_point = j;
                        hovered_ID = Nodes[i].ID;
                        hovering_any_point = true;
                    }
                    draw_list->AddCircleFilled(ImVec2(((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x * m_Zoom) + (Nodes[i].Position.x * m_Zoom) + draw_list_offset.x, (-(*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y * m_Zoom) + (Nodes[i].Position.y * m_Zoom) + draw_list_offset.y), 5.0f * m_Zoom, IM_COL32(210, 215, 211, 255));
                }
                if (!hovering_any_point && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
                    hovered_point = -1;
                    hovered_ID = -1;
                }

                if (hovered_point != -1 && hovered_ID == Nodes[i].ID && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    ((ImVec2*)Nodes[i].NodeValues[1]->VectorValues[hovered_point])->x += io.MouseDelta.x * m_Zoom;
                    ((ImVec2*)Nodes[i].NodeValues[1]->VectorValues[hovered_point])->y -= io.MouseDelta.y * m_Zoom;
                    Saved = false;
                }


                if (IsMouseHoveringPolyline(points, static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()), 5.0f * m_Zoom) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && hovered_point == -1)
                    SelectedNode = i;
            }   break;
            }

            ImGui::EndGroup();
        }   break;
        default:
            break;
        }
        ImGui::PopID();
    }

    ImGui::SetCursorPos(ImGui::GetWindowViewport()->Pos);
    ImGui::InvisibleButton("VIEWPORT_TARGET", ImGui::GetWindowViewport()->Size);
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("FILE"))
        {
            std::string file_path = *(std::string*)p->Data;

            if (IsLineExist(file_path, ".vscene"))
            {
                CurrentProject.LoadSceneFile(file_path);
            } 
            else if (IsLineExist(file_path, ".png") || IsLineExist(file_path, ".jpeg") || IsLineExist(file_path, ".jpg") || IsLineExist(file_path, ".webp"))
            {
                ImVec2 scene_pos;
                scene_pos.x = ImGui::GetMousePosOnOpeningCurrentPopup().x - draw_list_offset.x;
                scene_pos.y = ImGui::GetMousePosOnOpeningCurrentPopup().y - draw_list_offset.y;

                Nodes.push_back(Node(scene_pos));

                Nodes[Nodes.size() - 1].NodeType = Node::Type::SPRITE;
            
                Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new std::string(file_path), Node::NodeValue::Type::STRING));

                CurrentProject.LoadTextureFromFile(CurrentProject.GetProjectLocation() + file_path, Nodes[Nodes.size() - 1]);

                Saved = false;
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
    {
        if (SelectedNode != -1)
            ImGui::OpenPopup("NodeMenu");
        else
            ImGui::OpenPopup("Nodes");
    }

    if (ImGui::BeginPopup("Nodes"))
    {
        ImGui::PushID(10);
        ImVec2 scene_pos;
        scene_pos.x = ImGui::GetMousePosOnOpeningCurrentPopup().x - draw_list_offset.x;
        scene_pos.y = ImGui::GetMousePosOnOpeningCurrentPopup().y - draw_list_offset.y;
        if (ImGui::MenuItem("Node")) { Nodes.push_back(Node(scene_pos)); Saved = false; }
        if (ImGui::MenuItem("Sprite")) { Nodes.push_back(Node(scene_pos, Node::Type::SPRITE)); Saved = false; }
        if (ImGui::MenuItem("Camera")) { Nodes.push_back(Node(scene_pos, Node::Type::CAM)); Saved = false; }
        if (ImGui::MenuItem("PhysicsBody")) { Nodes.push_back(Node(scene_pos, Node::Type::PHYSICSBODY)); Saved = false; }
        if (ImGui::MenuItem("Collider")) { Nodes.push_back(Node(scene_pos, Node::Type::COLLIDER)); Saved = false; }
        ImGui::PopID();
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("NodeMenu"))
    {
        ImGui::PushID(10);
        if (ImGui::MenuItem("Delete Node"))
        {
            std::vector<int> IDs_to_delete;
            IDs_to_delete.push_back(Nodes[SelectedNode].ID);
            DeleteChildNodes(Nodes, Nodes[SelectedNode].ChildIDs, IDs_to_delete);
            while (IDs_to_delete.size() > 0)
            {
                for (int i = 0; i < static_cast<int>(Nodes.size()); i++)
                {
                    for (int j = 0; j < static_cast<int>(IDs_to_delete.size()); j++)
                    {
                        if (Nodes[i].ID == IDs_to_delete[j])
                        {
                            for (int x = 0; x < static_cast<int>(Nodes[i].NodeValues.size()); x++)
                            {
                                delete Nodes[i].NodeValues[x];
                                Nodes[i].NodeValues[x] = nullptr;
                            }
                            if (Nodes[i].Texture != nullptr)
                                SDL_DestroyTexture(Nodes[i].Texture);

                            if (Nodes[SelectedNode].ParentID != -1)
                            {
                                for (int x = 0; x < static_cast<int>(Nodes.size()); x++)
                                {
                                    if (Nodes[SelectedNode].ParentID == Nodes[x].ID)
                                    {
                                        Nodes[x].ChildIDs.erase(std::find(Nodes[x].ChildIDs.begin(), Nodes[x].ChildIDs.end(), Nodes[SelectedNode].ID));
                                        break;
                                    }
                                }
                            }

                            Nodes.erase(Nodes.begin() + i);
                            IDs_to_delete.erase(IDs_to_delete.begin() + j);
                        }
                    }
                }
            }

            SelectedNode = -1;

            Saved = false;
        }
        ImGui::PopID();
        ImGui::EndPopup();
    }

    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        m_Zoom += io.MouseWheel / 10.0f;

    if (m_Zoom >= 2.5f)
        m_Zoom = 2.5f;
    else if (m_Zoom <= 0.1f)
        m_Zoom = 0.1f;

    draw_list->ChannelsMerge();

    if (ImGui::IsAnyItemActive() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsWindowFocused() && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && SelectedNode != -1)
    {
        Nodes[SelectedNode].Position.x += io.MouseDelta.x;
        Nodes[SelectedNode].Position.y += io.MouseDelta.y;
        Saved = false;
    }

    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
    {
        m_Scrolling.x = m_Scrolling.x + io.MouseDelta.x;
        m_Scrolling.y = m_Scrolling.y + io.MouseDelta.y;
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::End();
}