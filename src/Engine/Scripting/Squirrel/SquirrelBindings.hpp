#include <simplesquirrel/simplesquirrel.hpp>
#include "Objects/Node.h"
#include "Util/SceneFileFunctions.h"
#include <fstream>
#include <iostream>

Scene* root;

std::vector<std::string> InstantiatedNodeFilePaths;

void LoadNodeScripts(std::vector<Node*> nodes) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        root->GetSquirrelVM()->run(root->GetSquirrelVM()->compileSource(nodes[i]->Script.c_str()));
        nodes[i]->Script = "";
    }
}

void SetNodes(std::vector<Node*> nodes) {
    ssq::Array nds = (ssq::Array)root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("GetNodesArray"), *root->GetSquirrelVM());
    for (int i = 0; i < (int)nodes.size(); i++) {
        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc(("add" + nodes[i]->Name + "class").c_str()), *root->GetSquirrelVM(), nds);
        //nodes[i]->ScriptIndex = i;
    }
}

void StartFunction(std::vector<Node*> nodes) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("SetNodeVal"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, nodes[i]);
        switch (nodes[i]->Type) {
            case NodeType::NODE:
                break;
            case NodeType::CAM:
                break;
            case NodeType::SPRITE:
            root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("SetSpriteTexture"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, ((Sprite*)nodes[i])->texture);
                break;
            case NodeType::PHYSICSBODY:
            root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("SetPhysicsbodyBody"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, ((PhysicsBody*)nodes[i])->body);
                break;
        }

        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("StartFunc"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex);

        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("GetNodeVal"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, nodes[i]);
    }
}

void InstantiateNodesFromFile(const std::string& NodeFilePath) {
    InstantiatedNodeFilePaths.push_back(NodeFilePath); 
}

void AssignRoot(Scene* r) {
    root = r;
}

bool SetNodesScript(const std::string& Line, Node* n) {
    std::string sc = GetLineBetween(Line, "[SCRIPT=", "]");
    if (sc != "NULL") {
        std::fstream stream(sc);
        std::string ln;
        n->ScriptLocation = sc;
        while (getline(stream, ln)) {
            n->Script += ln + '\n';
            if (ln.find("class") != std::string::npos) {
                n->ScriptClassName = GetLineBetween(ln, "class ", " extends");
            }
        }
        n->Script += "\nfunction add" + n->Name + "class(n) { n.push(" + n->ScriptClassName + "()); }";
        return true;
    }
    return false;
}

void DefaultNodeBindings(ssq::Class* node) {
    node->addVar("Name", &Node::Name);
    node->addVar("Position", &Node::Position);
    node->addFunc("SetPosition", &Node::SetPosition);
    node->addVar("Angle", &Node::Angle);
    node->addVar("Size", &Node::Size);
}

void BindNodes() {
    ssq::Class vec2 = root->GetSquirrelVM()->addClass("Vector2", ssq::Class::Ctor<Vector2(float, float)>());
    vec2.addVar("x", &Vector2::x);
    vec2.addVar("y", &Vector2::y);
    vec2.addFunc("AddToVec2", &Vector2::AddToVec2);

    ssq::Class node = root->GetSquirrelVM()->addClass("Node", ssq::Class::Ctor<Node()>());
    DefaultNodeBindings(&node);

    ssq::Class texture = root->GetSquirrelVM()->addClass("Texture", ssq::Class::Ctor<Texture()>());
    texture.addFunc("ChangeTexture", &Texture::ChangeTexture);
    
    ssq::Class sprite = root->GetSquirrelVM()->addClass("Sprite", ssq::Class::Ctor<Sprite()>());
    DefaultNodeBindings(&sprite);
    sprite.addVar("Texture", &Sprite::texture);

    ssq::Class body = root->GetSquirrelVM()->addClass("Body", ssq::Class::Ctor<Body()>());
    body.addFunc("SetFixedRotation", &Body::SetFixedRotation);
    body.addFunc("SetVelocity", &Body::SetVelocity);
    body.addFunc("GetVelocity", &Body::GetVelocity);

    ssq::Class physicsbody = root->GetSquirrelVM()->addClass("PhysicsBody", ssq::Class::Ctor<PhysicsBody()>());
    DefaultNodeBindings(&physicsbody);
    physicsbody.addVar("Body", &PhysicsBody::body);
}

void BindFunctions() {
    root->GetSquirrelVM()->addFunc("IsMouseKeyJustPressed", [=](int x)->bool {
                return root->GetInput()->IsMouseKeyJustPressed(x);
            });
    root->GetSquirrelVM()->addFunc("IsMouseKeyJustReleased", [=](int x)->bool {
                return root->GetInput()->IsMouseKeyJustReleased(x);
            });
    root->GetSquirrelVM()->addFunc("IsMouseKeyPressed", [=](int x)->bool {
                return root->GetInput()->IsMouseKeyPressed(x);
            });
    root->GetSquirrelVM()->addFunc("IsKeyJustPressed", [=](int x)->bool {
                return root->GetInput()->IsKeyJustPressed(x);
            });
    root->GetSquirrelVM()->addFunc("IsKeyJustReleased", [=](int x)->bool {
                return root->GetInput()->IsKeyJustReleased(x);
            });
    root->GetSquirrelVM()->addFunc("IsKeyPressed", [=](int x)->bool {
                return root->GetInput()->IsKeyPressed(x);
            });
    root->GetSquirrelVM()->addFunc("InstantiateNodesFromFile", InstantiateNodesFromFile);
}

void RunEngineFile() {
    root->GetSquirrelVM()->run(root->GetSquirrelVM()->compileFile("../Assets/VergodtEngine.nut"));
}

void UpdateFunction(std::vector<Node*> nodes, float dt) {
    for (std::size_t i = 0; i < nodes.size(); i++) {
        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("SetNodeVal"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, nodes[i]);
        
        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("UpdateFunc"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, dt);

        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("GetNodeVal"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, nodes[i]);
    }
    for (std::size_t i = 0; i < InstantiatedNodeFilePaths.size(); i++)
    {
        std::vector<Node*> NewNodes = root->AddNodesToScene(InstantiatedNodeFilePaths[i]);

        LoadNodeScripts(NewNodes);
        SetNodes(NewNodes);
        StartFunction(NewNodes);
    }
    InstantiatedNodeFilePaths.clear();
}