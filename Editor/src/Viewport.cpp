#include "Viewport.h"
#include <math.h>
#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "imgui_internal.h"
#include "App.h"

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

    
    ImGuiWindowFlags flags = Saved ? ImGuiWindowFlags_None : ImGuiWindowFlags_UnsavedDocument;
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
            else if (LoadTextureFromFile(CurrentProject.GetProjectLocation() + *(std::string*)Nodes[i].NodeValues[0]->Value, &texture, textrue_width, textrue_height, Renderer))
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

                draw_list->AddRect(ImVec2(minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)),
                                   ImVec2(minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2) + 2.5f, minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2) + 2.5f), IM_COL32(255, 99, 71, 255), 0.0f, ImDrawFlags_None, 5);


                static int selected_point = -1;
                draw_list->AddCircleFilled(ImVec2(minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y), 5.0f, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10, ImVec2(minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 1;
                }
                if (selected_point == 1 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float new_x = ((minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x += new_x;
                    Nodes[i].Position.x -= new_x / 2;
                    Saved = false;
                }

                draw_list->AddCircleFilled(ImVec2(minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)), 5.0f, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10, ImVec2(minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 2;
                }
                if (selected_point == 2 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float new_x = ((minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x += new_x;
                    Nodes[i].Position.x -= new_x / 2;

                    float new_y = ((minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y += new_y;
                    Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }
                
                draw_list->AddCircleFilled(ImVec2(minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)), 5.0f, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10, ImVec2(minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 3;
                }
                if (selected_point == 3 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float new_x = ((minrec.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x += new_x;
                    Nodes[i].Position.x -= new_x / 2;

                    float new_y = ((minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y -= new_y;
                    Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }
                
                draw_list->AddCircleFilled(ImVec2(minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y), 5.0f, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10, ImVec2(minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 4;
                }
                if (selected_point == 4 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float new_x = ((minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x -= new_x;
                    Nodes[i].Position.x -= new_x / 2;
                    Saved = false;
                }
                
                draw_list->AddCircleFilled(ImVec2(minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)), 5.0f, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10, ImVec2(minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 5;
                }
                if (selected_point == 5 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float new_x = ((minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x -= new_x;
                    Nodes[i].Position.x -= new_x / 2;

                    float new_y = ((minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y += new_y;
                    Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }

                draw_list->AddCircleFilled(ImVec2(minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)), 5.0f, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10, ImVec2(minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 6;
                }
                if (selected_point == 6 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float new_x = ((minrec.x + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2)) - ImGui::GetMousePos().x) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->x -= new_x;
                    Nodes[i].Position.x -= new_x / 2;

                    float new_y = ((minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y -= new_y;
                    Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }
                
                draw_list->AddCircleFilled(ImVec2(minrec.x, minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)), 5.0f, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10, ImVec2(minrec.x, minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 7;
                }
                if (selected_point == 7 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float new_y = ((minrec.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y += new_y;
                    Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }

                draw_list->AddCircleFilled(ImVec2(minrec.x, minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)), 5.0f, IM_COL32(210, 215, 211, 255));
                if (IsMouseHoveringCircle(10, ImVec2(minrec.x, minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2))) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    selected_point = 8;
                }
                if (selected_point == 8 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float new_y = ((minrec.y + (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)) - ImGui::GetMousePos().y) / 2;
                    ((ImVec2*)Nodes[i].NodeValues[1]->Value)->y -= new_y;
                    Nodes[i].Position.y -= new_y / 2;
                    Saved = false;
                }

                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && selected_point != -1)
                    selected_point = -1;


                ImGui::SetCursorPos(ImVec2(Nodes[i].Position.x + offset.x - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x) / 2), Nodes[i].Position.y + offset.y - (((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y) / 2)));
                ImVec2 button_size = ImVec2(((*(ImVec2*)Nodes[i].NodeValues[1]->Value).x * Nodes[i].Size.x), ((*(ImVec2*)Nodes[i].NodeValues[1]->Value).y * Nodes[i].Size.y));
                if (button_size.x == 0)
                    button_size.x = 1;
                if (button_size.y == 0)
                    button_size.y = 1;
                ImGui::InvisibleButton("COLLIDER", button_size, ImGuiButtonFlags_MouseButtonLeft);
                if (ImGui::IsItemActivated() && selected_point == -1)
                    SelectedNode = i;
            }   break;
            case Node::ColliderType::CIRCLE: {
                ImVec2 minrec(Nodes[i].Position.x + draw_list_offset.x, Nodes[i].Position.y + draw_list_offset.y);
                draw_list->AddEllipse(minrec, ImVec2((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.x, (*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y), IM_COL32(255, 99, 71, 255), 0.0f, 0, 5);

                draw_list->AddCircleFilled(ImVec2(minrec.x , minrec.y - ((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y) - 2.5f), 5.0f, IM_COL32(210, 215, 211, 255));

                static bool resizing = false;
                if (IsMouseHoveringCircle(5.0f, ImVec2(minrec.x , minrec.y - ((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.y) - 2.5f)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    resizing = true;
                if (resizing && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    float* radius = (float*)Nodes[i].NodeValues[1]->Value;

                    float new_y = ((minrec.y - ((*radius * Nodes[i].Size.y)) - 2.5f) - ImGui::GetMousePos().y) / 2;
                    *radius += new_y;
                    Nodes[i].Position.y -= new_y;
                    Saved = false;
                }
                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && resizing)
                    resizing = false;

                if (IsMouseHoveringCircle((*(float*)Nodes[i].NodeValues[1]->Value) * Nodes[i].Size.x, minrec) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !resizing)
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
                    if (IsMouseHoveringCircle(10, ImVec2((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x + Nodes[i].Position.x + draw_list_offset.x, -(*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y + Nodes[i].Position.y + draw_list_offset.y)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        hovered_point = j;
                        hovering_any_point = true;
                    }
                    draw_list->AddCircleFilled(ImVec2((*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).x + Nodes[i].Position.x + draw_list_offset.x, -(*(ImVec2*)Nodes[i].NodeValues[1]->VectorValues[j]).y + Nodes[i].Position.y + draw_list_offset.y), 5.0f, IM_COL32(210, 215, 211, 255));
                }
                if (!hovering_any_point && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    hovered_point = -1;

                if (hovered_point != -1 && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
                {
                    ((ImVec2*)Nodes[i].NodeValues[1]->VectorValues[hovered_point])->x += io.MouseDelta.x;
                    ((ImVec2*)Nodes[i].NodeValues[1]->VectorValues[hovered_point])->y -= io.MouseDelta.y;
                    Saved = false;
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
    {
        std::cout << SelectedNode;
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
                            Nodes.erase(Nodes.begin() + i);
                            IDs_to_delete.erase(IDs_to_delete.begin() + j);
                        }
                    }
                }
            }

            SelectedNode = -1;
        }
        ImGui::PopID();
        ImGui::EndPopup();
    }


    draw_list->ChannelsMerge();

    if (ImGui::IsAnyItemActive() && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
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