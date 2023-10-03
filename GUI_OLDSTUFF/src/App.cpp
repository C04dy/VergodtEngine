#include "App.h"
#include "nlohmann/json.hpp"

std::ifstream ifs("Assets/test.json");
nlohmann::json jf = nlohmann::json::parse(ifs);
auto objects = jf["Objects"];

std::vector<std::string> SelNodesName;
std::vector<std::string> SelNodesType;
std::vector<std::string> TextureLocation;
std::vector<float> PosX;
std::vector<float> PosY;
std::vector<float> SizeX;
std::vector<float> SizeY;
std::vector<float> Angle;
std::string NodesWithTextures[1] = { "Sprite" };

int SelectedNode;

void App::Init()
{
    for (auto object : objects) {
        auto name = object["name"];
        auto texture = object["texture"];
        auto type = object["type"];

        SelNodesName.push_back(name);
        SelNodesType.push_back(type);
        if (texture != nullptr) {
            TextureLocation.push_back(texture);
        }
        else {
            TextureLocation.push_back("NULL");
        }

        PosX.push_back(object["position"]["X"].get<float>());
        PosY.push_back(object["position"]["Y"].get<float>());
        SizeX.push_back(object["size"]["X"].get<float>());
        SizeY.push_back(object["size"]["Y"].get<float>());
        Angle.push_back(object["angle"].get<float>());
    }
}

void App::RenderUI() {

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Inspection");

    for (int i = 0; i < SelNodesName.size(); i++)
    {
        ImGui::Bullet(); 
        bool sel = ImGui::SmallButton(SelNodesName[i].c_str());
        if (sel == true) {
            SelectedNode = i;
        }
    }

    ImGui::End();

	ImGui::Begin("Settings");

    char* Name = new char[SelNodesName[SelectedNode].length() + 1];

    strcpy(Name, SelNodesName[SelectedNode].c_str());

    ImGui::InputText("Name", Name, sizeof(Name));

    int i = 1;

    if (ImGui::TreeNode("Position"))
    {
        i = 1;
        for (auto object : objects) {
            if (object["name"] == SelNodesName[SelectedNode]) {
                ImGui::InputFloat("X", &PosX[SelectedNode]);
                ImGui::InputFloat("Y", &PosY[SelectedNode]);

                object["position"]["X"] = PosX[SelectedNode];
                object["position"]["Y"] = PosY[SelectedNode];

                objects[std::to_string(i)] = object;

                ImGui::TreePop();
            }
            i++;
        }
    }

    if (ImGui::TreeNode("Size"))
    {
        i = 1;
        for (auto object : objects) {
            if (object["name"] == SelNodesName[SelectedNode]) {
                ImGui::InputFloat("X", &SizeX[SelectedNode]);
                ImGui::InputFloat("Y", &SizeY[SelectedNode]);

                object["size"]["X"] = SizeX[SelectedNode];
                object["size"]["Y"] = SizeY[SelectedNode];

                objects[std::to_string(i)] = object;

                ImGui::TreePop();
            }
            i++;
        }
    }


    i = 1;
    for (auto object : objects) {
        if (object["name"] == SelNodesName[SelectedNode]) {
            ImGui::InputFloat("Angle", &Angle[SelectedNode]);

            object["angle"] = Angle[SelectedNode];
            objects[std::to_string(i)] = object;
        }
        i++;
    }

    i = 1;
    for (auto object : objects) {
        for (int j = 0; j < NodesWithTextures->size(); j++){
            if (SelNodesType[SelectedNode] == NodesWithTextures[j] && object["type"] == NodesWithTextures[j]) {
                
                char* Texture = new char[object["texture"].get<std::string>().length() + 1];

                strcpy(Texture, object["texture"].get<std::string>().c_str());

                ImGui::InputText("Texture", Texture, object["texture"].get<std::string>().length() + 100);
                
                object["texture"] = Texture;

                objects[std::to_string(i)] = object;
            }
        }
        i++;
    }

    ImGui::End();

    ImGui::Begin("ViewPort");
    ImGui::End();

	ImGui::End();

    //ImGui::ShowDemoWindow();
}

void App::WriteValue() {
    jf["Objects"] = objects;

    std::ofstream output_file("Assets/test.json");

    std::string file_json_string = jf.dump();
    output_file.write(file_json_string.c_str(), file_json_string.size());
    output_file.close();
}