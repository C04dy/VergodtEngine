#include "Viewport.h"
#include <math.h>
#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "imgui_internal.h"
#include "App.h"

static bool IsMouseHoveringPolyline(ImVec2 Points[], int PointCount, float LineThickness)
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

static bool LoadTextureFromFile(const std::string& FilePath, SDL_Texture** Texture, float& Width, float& Height, SDL_Renderer* Renderer)
{
    SDL_Surface* surface = IMG_Load(FilePath.c_str());

    if (surface == nullptr)
    {
        std::cout << "Failed to load texture: " << FilePath.c_str() << '\n';
        return false;
    }

    *Texture = SDL_CreateTextureFromSurface(Renderer, surface);

    if (*Texture == nullptr)
    {
        std::cout << "Failed to create texture from surface\n";
        return false;
    }


    Width = surface->w;
    Height = surface->h;

    SDL_DestroySurface(surface);

    return true;
}

void Viewport::ViewportSpace(SDL_Renderer* Renderer, std::vector<Node>& Nodes, int& SelectedNode)
{
    ImGuiIO& io = ImGui::GetIO();

    if (!ImGui::Begin("Viewport"))
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
    offset.x = m_Scrolling.x;
    offset.y = m_Scrolling.y;
    ImVec2 draw_list_offset;
    draw_list_offset.x = ImGui::GetCursorScreenPos().x + m_Scrolling.x;
    draw_list_offset.y = ImGui::GetCursorScreenPos().y + m_Scrolling.y;

    if (show_grid)
    {
        ImU32 grid_color = IM_COL32(200, 200, 200, 40);
        float grid_size = 64.0f;
        ImVec2 window_position = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetWindowSize();
        for (float x = fmodf(m_Scrolling.x, grid_size); x < canvas_size.x; x += grid_size)
            draw_list->AddLine(ImVec2(x + window_position.x, window_position.y), ImVec2(x + window_position.x, canvas_size.y + window_position.y), grid_color);
        for (float y = fmodf(m_Scrolling.y, grid_size); y < canvas_size.y; y += grid_size)
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
            SDL_Texture* texture;
            float textrue_width, textrue_height;
            if (*(std::string*)Nodes[i].NodeValues[0]->Value == "None")
            {

                ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x, Nodes[i].Position.y + offset.y));
                ImGui::BeginGroup();
                ImGui::InvisibleButton("SPRITE", ImVec2(50, 50), ImGuiButtonFlags_MouseButtonLeft);
                if (ImGui::IsItemActivated())
                    SelectedNode = i;
                ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x, Nodes[i].Position.y + offset.y));
                ImGui::Text("Sprite");
                ImGui::EndGroup();

            }
            else if (LoadTextureFromFile(*(std::string*)Nodes[i].NodeValues[0]->Value, &texture, textrue_width, textrue_height, Renderer))
            {
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);
                ImVec4 tint_col = false ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
                if (i == SelectedNode) {
                    ImVec2 minrec(Nodes[i].Position.x + draw_list_offset.x, Nodes[i].Position.y + draw_list_offset.y);
                    draw_list->AddRect(ImVec2(minrec.x - ((textrue_width * Nodes[i].Size.x) / 2), minrec.y - ((textrue_height * Nodes[i].Size.y) / 2))
                                    ,  ImVec2(minrec.x + ((textrue_width * Nodes[i].Size.x) / 2) + 2.5f, minrec.y + ((textrue_height * Nodes[i].Size.y) / 2) + 2.5f), IM_COL32(255, 99, 71, 255), 0.0f, ImDrawFlags_None, 5);
                }
                ImGui::BeginGroup();
                ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x - ((textrue_width * Nodes[i].Size.x) / 2), Nodes[i].Position.y + offset.y - ((textrue_height * Nodes[i].Size.y) / 2)));
                ImGui::Image(texture, ImVec2(textrue_width * Nodes[i].Size.x, textrue_height * Nodes[i].Size.y), uv_min, uv_max, tint_col, border_col);
                ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x - ((textrue_width * Nodes[i].Size.x) / 2), Nodes[i].Position.y + offset.y - ((textrue_height * Nodes[i].Size.y) / 2)));
                ImGui::InvisibleButton("SPRITE", ImVec2(textrue_width * Nodes[i].Size.x, textrue_height * Nodes[i].Size.y), ImGuiButtonFlags_MouseButtonLeft);
                if (ImGui::IsItemActivated())
                    SelectedNode = i;
                ImGui::EndGroup();
            }
            break;
        case Node::Type::PHYSICSBODY:
            ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x, Nodes[i].Position.y + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("PHYSICSBODY", ImVec2(50, 50), ImGuiButtonFlags_MouseButtonLeft);
            if (ImGui::IsItemActivated())
                SelectedNode = i;
            ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x, Nodes[i].Position.y + offset.y));
            ImGui::Text("PhysicsBody");
            ImGui::EndGroup();
            break;
        case Node::Type::NODE:
            ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x, Nodes[i].Position.y + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("NODE", ImVec2(50, 50), ImGuiButtonFlags_MouseButtonLeft);
            if (ImGui::IsItemActivated())
                SelectedNode = i;
            ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x, Nodes[i].Position.y + offset.y));
            ImGui::Text("Node");
            ImGui::EndGroup();
            break;
        case Node::Type::CAM:
            ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x, Nodes[i].Position.y + offset.y));
            ImGui::BeginGroup();
            ImGui::InvisibleButton("CAM", ImVec2(50, 50), ImGuiButtonFlags_MouseButtonLeft);
            if (ImGui::IsItemActivated())
                SelectedNode = i;
            ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x, Nodes[i].Position.y + offset.y));
            ImGui::Text("Cam");
            ImGui::EndGroup();
            break;
        case Node::Type::COLLIDER: {
            ImGui::BeginGroup();

            switch (*(Node::ColliderType*)Nodes[i].NodeValues[0]->Value)
            {
            case Node::ColliderType::BOX: {
                ImVec2 minrec(Nodes[i].Position.x + draw_list_offset.x , Nodes[i].Position.y + draw_list_offset.y);

                draw_list->AddRect(ImVec2(minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2))
                                ,  ImVec2(minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2) + 2.5f, minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2) + 2.5f), IM_COL32(255, 99, 71, 255), 0.0f, ImDrawFlags_None, 5);

                ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), Nodes[i].Position.y + offset.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)));
                ImGui::InvisibleButton("COLLIDER", ImVec2(((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x), ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y)), ImGuiButtonFlags_MouseButtonLeft);
                if (ImGui::IsItemActivated())
                    SelectedNode = i;
            }   break;
            case Node::ColliderType::CIRCLE: {
                ImVec2 minrec(Nodes[i].Position.x + draw_list_offset.x, Nodes[i].Position.y + draw_list_offset.y);
                draw_list->AddEllipse(minrec, ImVec2((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.x, (*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y), IM_COL32(255, 99, 71, 255), 0.0f, 0, 5);

                ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x - ((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.x / 2), Nodes[i].Position.y + offset.y - ((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y / 2)));
                ImGui::InvisibleButton("COLLIDER", ImVec2((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.x, (*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y), ImGuiButtonFlags_MouseButtonLeft);
                if (ImGui::IsItemActivated())
                    SelectedNode = i;
            }   break;
            case Node::ColliderType::POLYGONS: {
                ImVec2 points[Nodes[i].NodeValues[1]->VectorValues.size() + 1];

                for (int j = 0; j < static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()); j++)
                {
                    points[j] = ImVec2((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x + draw_list_offset.x + Nodes[i].Position.x,
                                       -((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y) + draw_list_offset.y + Nodes[i].Position.y);
                }

                draw_list->AddPolyline(points, static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()), IM_COL32(255, 99, 71, 255), ImDrawFlags_Closed, 5);

                static int hovered_point = -1;
                bool hovering_any_point = false;
                for (int j = 0; j < static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()); j++)
                {
                    ImGui::PushID(j);

                    ImGui::SetCursorPos(ImVec2((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x + Nodes[i].Position.x + offset.x,
                                               -(*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y + Nodes[i].Position.y + offset.y));
                    ImGui::InvisibleButton("point", ImVec2(10.0f, 10.0f), ImGuiButtonFlags_MouseButtonLeft);
                    if (ImGui::IsItemActivated())
                    {
                        hovered_point = j;
                        hovering_any_point = true;
                    }
                    draw_list->AddCircleFilled(ImVec2((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x + Nodes[i].Position.x + draw_list_offset.x, -(*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y + Nodes[i].Position.y + draw_list_offset.y), 5.0f, IM_COL32(210, 215, 211, 255));
                    ImGui::PopID();
                }
                if (!hovering_any_point && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    hovered_point = -1;

                if (hovered_point != -1 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    ((ImVec2*)Nodes[i].NodeValues[1]->VectorValues[hovered_point])->x += io.MouseDelta.x;
                    ((ImVec2*)Nodes[i].NodeValues[1]->VectorValues[hovered_point])->y -= io.MouseDelta.y;
                }


                if (IsMouseHoveringPolyline(points, static_cast<int>(Nodes[i].NodeValues[1]->VectorValues.size()), 5.0f) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered())
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

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        ImGui::OpenPopup("Nodes");

    if (ImGui::BeginPopup("Nodes"))
    {
        ImGui::PushID(10);
        ImVec2 scene_pos;
        scene_pos.x = ImGui::GetMousePosOnOpeningCurrentPopup().x - draw_list_offset.x;
        scene_pos.y = ImGui::GetMousePosOnOpeningCurrentPopup().y - draw_list_offset.y;
        if (ImGui::MenuItem("Node")) { Nodes.push_back(Node(scene_pos)); }
        if (ImGui::MenuItem("Sprite")) { Nodes.push_back(Node(scene_pos, Node::Type::SPRITE)); }
        if (ImGui::MenuItem("Camera")) { Nodes.push_back(Node(scene_pos, Node::Type::CAM)); }
        if (ImGui::MenuItem("PhysicsBody")) { Nodes.push_back(Node(scene_pos, Node::Type::PHYSICSBODY)); }
        if (ImGui::MenuItem("Collider"))
        {
            Nodes.push_back(Node(scene_pos, Node::Type::COLLIDER));

            Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new Node::ColliderType(Node::ColliderType::BOX), Node::NodeValue::Type::INT, { "Box", "Circle", "Polygon" }));

            Nodes[Nodes.size() - 1].NodeValues.push_back(new Node::NodeValue(new ImVec2(0.0f, 0.0f), Node::NodeValue::Type::VECTOR2));
        }
        ImGui::PopID();
        ImGui::EndPopup();
    }


    draw_list->ChannelsMerge();

    if (ImGui::IsAnyItemActive() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        Nodes[SelectedNode].Position.x += io.MouseDelta.x;
        Nodes[SelectedNode].Position.y += io.MouseDelta.y;
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
