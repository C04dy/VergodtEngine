#include <simplesquirrel/simplesquirrel.hpp>
#include "Objects/Node.h"
#include "Util/SceneFileFunctions.h"
#include <fstream>
#include <iostream>
#include <random>

Scene* root;

int RandomRangeInt(int Min, int Max) {
    int n = Max - Min + 1;
    int i = rand() % n;
    if (i < 0) i = -i;
    return Min + i;
}

float RandomRangeFloat(float Min, float Max) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = Max - Min;
    float r = random * diff;
    return (float)(Min + r);
}


void FreeNode(Node* n) {
    root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("RemoveNodeFromArray"), *root->GetSquirrelVM(), n);
    root->RemoveNodeFromScene(n);
}

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
    }
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
    node->addVar("LocalPosition", &Node::LocalPosition);
    node->addVar("Angle", &Node::Angle);
    node->addVar("Size", &Node::Size);
    node->addVar("Childs", &Node::ChildNodes);
    node->addVar("ScriptIndex", &Node::ScriptIndex);
}

void BindNodes() {
    ssq::Class vec2 = root->GetSquirrelVM()->addClass("Vector2", ssq::Class::Ctor<Vector2(float, float)>());
    vec2.addVar("x", &Vector2::x);
    vec2.addVar("y", &Vector2::y);
    vec2.addFunc("AddToVec2", &Vector2::AddToVec2);

    ssq::Class chn = root->GetSquirrelVM()->addClass("Children", ssq::Class::Ctor<Node::Children()>());
    chn.addFunc("GetChild", &Node::Children::GetChild);
    chn.addFunc("GetChilds", &Node::Children::GetChilds);
    chn.addFunc("AddChild", &Node::Children::AddChildToList);
    chn.addFunc("GetChildCount", &Node::Children::GetChildCount);

    ssq::Class node = root->GetSquirrelVM()->addClass("Node", ssq::Class::Ctor<Node()>());
    DefaultNodeBindings(&node);

    ssq::Class texture = root->GetSquirrelVM()->addClass("Texture", ssq::Class::Ctor<Sprite::Texture()>());
    texture.addFunc("ChangeTexture", &Sprite::Texture::ChangeTexture);
    
    ssq::Class sprite = root->GetSquirrelVM()->addClass("Sprite", ssq::Class::Ctor<Sprite()>());
    DefaultNodeBindings(&sprite);
    sprite.addVar("Texture", &Sprite::texture);

    ssq::Class body = root->GetSquirrelVM()->addClass("Body", ssq::Class::Ctor<Body()>());
    body.addFunc("SetFixedRotation", &Body::SetFixedRotation);
    body.addFunc("SetVelocity", &Body::SetVelocity);
    body.addFunc("GetVelocity", &Body::GetVelocity);
    body.addFunc("SetGravity", &Body::SetGravity);

    ssq::Class physicsbody = root->GetSquirrelVM()->addClass("PhysicsBody", ssq::Class::Ctor<PhysicsBody()>());
    DefaultNodeBindings(&physicsbody);
    physicsbody.addVar("Body", &PhysicsBody::body);
}

void RunEngineFile() {
    root->GetSquirrelVM()->run(root->GetSquirrelVM()->compileFile("../Assets/VergodtEngine.nut"));
}

void StartFunction(std::vector<Node*> nodes) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("SetNodeVal"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, nodes[i]);
        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("SetChildren"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, nodes[i]);
        switch (nodes[i]->Type) {
            case NodeType::NODE:if (nodes[i]->Type == NodeType::PHYSICSBODY) {
            ((PhysicsBody*)nodes[i])->GetBody()->SetTransform(nodes[i]->Position / 100, ((PhysicsBody *)nodes[i])->GetBody()->GetAngle());
        }
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

void UpdateFunction(std::vector<Node*> nodes, float dt) {
    for (std::size_t i = 0; i < nodes.size(); i++) {
        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("SetNodeVal"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, nodes[i]);

        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("UpdateFunc"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, dt);

        root->GetSquirrelVM()->callFunc(root->GetSquirrelVM()->findFunc("GetNodeVal"), *root->GetSquirrelVM(), nodes[i]->ScriptIndex, nodes[i]);

        if (nodes[i]->Type == NodeType::PHYSICSBODY) {
            ((PhysicsBody*)nodes[i])->GetBody()->SetTransform(nodes[i]->Position / 100, ((PhysicsBody*)nodes[i])->GetBody()->GetAngle());
        }
    }
}

void InstantiateNodesFromFile(const std::string& NodeFilePath) {
    std::vector<Node*> NewNodes = root->AddNodesToScene(NodeFilePath);

    root->UpdateChilds();

    LoadNodeScripts(NewNodes);
    SetNodes(NewNodes);
    StartFunction(NewNodes);
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
    root->GetSquirrelVM()->addFunc("QueueFree", FreeNode);
    root->GetSquirrelVM()->addFunc("RandomRangeInt", RandomRangeInt);
    root->GetSquirrelVM()->addFunc("RandomRangeFloat", RandomRangeFloat);
}