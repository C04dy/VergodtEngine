#include <simplesquirrel/simplesquirrel.hpp>
#include "Objects/Node.h"
#include "Util/SceneFileFunctions.h"
#include <fstream>
#include <iostream>

InputManager* im;

void AssignInput(InputManager* input) {
    im = input;
}

std::string GetScriptClassLine(const std::string& ClassName, const std::string& NodeName) {
    return "Ns.push(" + ClassName + "(" + '"' + NodeName + '"' + ")" + "); \n";
}

bool SetNodesScript(const std::string& Line, Node* n) {
    std::string sc = GetLineBetween(Line, "[SCRIPT=", "]");
    if (sc != "NULL") {
        std::fstream stream(sc);
        std::string ln;
        while (getline(stream, ln)) {
            if (ln.find("class") != std::string::npos) {
                n->ScriptClassName = GetLineBetween(ln, "class ", " extends");
                break;
            }
        }
        n->ScriptLocation = sc;
        return true;
    }
    return false;
}

void BindFunctions(ssq::VM* vm) {
    vm->addFunc("IsMouseKeyJustPressed", [=](int x)->bool {
                return im->IsMouseKeyJustPressed(x);
            });
    vm->addFunc("IsMouseKeyJustReleased", [=](int x)->bool {
                return im->IsMouseKeyJustReleased(x);
            });
    vm->addFunc("IsMouseKeyPressed", [=](int x)->bool {
                return im->IsMouseKeyPressed(x);
            });
    vm->addFunc("IsKeyJustPressed", [=](int x)->bool {
                return im->IsKeyJustPressed(x);
            });
    vm->addFunc("IsKeyJustReleased", [=](int x)->bool {
                return im->IsKeyJustReleased(x);
            });
    vm->addFunc("IsKeyPressed", [=](int x)->bool {
                return im->IsKeyPressed(x);
            });
}

void BindNodes(ssq::VM* vm) {
    ssq::Class vec2 = vm->addClass("Vector2", ssq::Class::Ctor<Vector2(float, float)>());
    vec2.addVar("x", &Vector2::x);
    vec2.addVar("y", &Vector2::y);
    vec2.addFunc("AddToVec2", &Vector2::AddToVec2);

    ssq::Class input = vm->addClass("Input", ssq::Class::Ctor<InputManager()>());
    input.addFunc("IsMouseKeyPressed", &InputManager::IsMouseKeyPressed);
    input.addFunc("IsMouseKeyJustPressed", &InputManager::IsMouseKeyJustPressed);

    ssq::Class node = vm->addClass("Node", ssq::Class::Ctor<Node(std::string)>());
    node.addVar("Name", &Node::Name);
    node.addVar("Position", &Node::Position);
    node.addFunc("SetPosition", &Node::SetPosition);
    node.addVar("Angle", &Node::Angle);
    node.addVar("Size", &Node::Size);

    ssq::Class texture = vm->addClass("Texture", ssq::Class::Ctor<Texture()>());
    texture.addFunc("ChangeTexture", &Texture::ChangeTexture);
    
    ssq::Class sprite = vm->addClass("Sprite", ssq::Class::Ctor<Sprite(std::string)>());
    sprite.addVar("Name", &Node::Name);
    sprite.addVar("Position", &Node::Position);
    sprite.addFunc("SetPosition", &Node::SetPosition);
    sprite.addVar("Angle", &Node::Angle);
    sprite.addVar("Size", &Node::Size);
    sprite.addVar("Texture", &Sprite::texture);

    //squall::Klass<PhysicsBody>(*vm, "PhysicsBody")
    //    .func("SetFixedRotation", &PhysicsBody::SetFixedRotation);
}

void EditEngineFile(ssq::VM* vm, std::vector<Node*> nodes) {
    std::fstream stream("../Assets/VergodtEngine.nut");
    std::string line;
    std::string s;
    bool ChangeLine = false;
    while (getline(stream, line)) {
        if (!ChangeLine) {
            s += line + "\n";
        } else {
            for (int i = 0; i < (int)nodes.size(); i++) {
                s += GetScriptClassLine(nodes[i]->ScriptClassName, nodes[i]->Name);
                nodes[i]->ScriptIndex = i;
            }
            ChangeLine = false;
        }
        if (line.find("function SetNodes") != std::string::npos) {
            ChangeLine = true;
        }
    }
    stream.close();
    
    vm->run(vm->compileSource(s.c_str()));
}

void SetNodes(ssq::VM* vm) {
    vm->callFunc(vm->findFunc("SetNodes"), *vm);
}

void LoadNodeScripts(ssq::VM* vm, std::vector<Node*> nodes) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->run(vm->compileFile(nodes[i]->ScriptLocation.c_str()));
    }
}

void StartFunction(ssq::VM* vm, std::vector<Node*> nodes) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->callFunc(vm->findFunc("SetNodeVal"), *vm, nodes[i]->ScriptIndex, nodes[i]);
        switch (nodes[i]->Type) {
            case NodeType::SPRITE:
            vm->callFunc(vm->findFunc("SetSpriteTexture"), *vm, nodes[i]->ScriptIndex, ((Sprite*)nodes[i])->texture);
                break;
            case NodeType::PHYSICSBODY:
            Vector2 v;
            v.x = ((PhysicsBody *)nodes[i])->GetBody()->GetLinearVelocity().x;
            v.y = ((PhysicsBody *)nodes[i])->GetBody()->GetLinearVelocity().y;
            vm->callFunc(vm->findFunc("SetPhysicsBodyVelocity"), *vm, nodes[i]->ScriptIndex, v);
            vm->callFunc(vm->findFunc("SetPhysicsBody"), *vm, nodes[i]->ScriptIndex, (PhysicsBody*)nodes[i]);  
                break;
        }
    }

    vm->callFunc(vm->findFunc("StartFunc"), *vm);

    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->callFunc(vm->findFunc("GetNodeVal"), *vm, nodes[i]->ScriptIndex, nodes[i]);
    
        switch (nodes[i]->Type) {
            case NodeType::PHYSICSBODY:
            Vector2 v;
            v.x = ((PhysicsBody *)nodes[i])->GetBody()->GetLinearVelocity().x;
            v.y = ((PhysicsBody *)nodes[i])->GetBody()->GetLinearVelocity().y;
            vm->callFunc(vm->findFunc("GetPhysicsBodyVelocity"), *vm, nodes[i]->ScriptIndex, v);
                break;
        }
    }
}

void UpdateFunction(ssq::VM* vm, std::vector<Node*> nodes, float dt) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->callFunc(vm->findFunc("SetNodeVal"), *vm, nodes[i]->ScriptIndex, nodes[i]);
        
        switch (nodes[i]->Type) {
            case NodeType::PHYSICSBODY:
            Vector2 v;
            v.x = ((PhysicsBody *)nodes[i])->GetBody()->GetLinearVelocity().x;
            v.y = ((PhysicsBody *)nodes[i])->GetBody()->GetLinearVelocity().y;
            vm->callFunc(vm->findFunc("SetPhysicsBodyVelocity"), *vm, nodes[i]->ScriptIndex, v);
            vm->callFunc(vm->findFunc("SetPhysicsBody"), *vm, nodes[i]->ScriptIndex, (PhysicsBody*)nodes[i]);                 
                break;
        }
        
        vm->callFunc(vm->findFunc("UpdateFunc"), *vm, nodes[i]->ScriptIndex, dt);

        vm->callFunc(vm->findFunc("GetNodeVal"), *vm, nodes[i]->ScriptIndex, nodes[i]);
        switch (nodes[i]->Type) {
            case NodeType::PHYSICSBODY:
            Vector2 v;
            v.x = ((PhysicsBody *)nodes[i])->GetBody()->GetLinearVelocity().x;
            v.y = ((PhysicsBody *)nodes[i])->GetBody()->GetLinearVelocity().y;
            vm->callFunc(vm->findFunc("GetPhysicsBodyVelocity"), *vm, nodes[i]->ScriptIndex, v);
                break;
        }
    }
}