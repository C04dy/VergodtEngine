#include "squall/squall_vmstd.hpp"
#include "squall/squall_klass.hpp"
#include "Objects/Node.h"
#include "Util/SceneFileFunctions.h"
#include <fstream>
#include <iostream>

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
                n->ScriptClassName = GetLineBetween(ln, "class ", " extends Node");
                break;
            }
        }
        n->ScriptLocation = sc;
        return true;
    }
    return false;
}

void BindFunctions(squall::VMStd* vm, InputManager* im) {
    vm->defun("IsMouseKeyJustPressed", [=](int x)->bool {
                return im->IsMouseKeyJustPressed(x);
            });
    vm->defun("IsMouseKeyJustReleased", [=](int x)->bool {
                return im->IsMouseKeyJustReleased(x);
            });
}

void BindNodes(squall::VMStd* vm) {
    squall::Klass<Vector2>(*vm, "Vector2")
        .var("x", &Vector2::x)
        .var("y", &Vector2::y);

    squall::Klass<Node>(*vm, "Node")
        .var("Name", &Node::Name)
        .var("Position", &Node::Position)
        .var("Size", &Node::Size);
}

void EditEngineFile(squall::VMStd* vm, std::vector<Node*> nodes) {
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
        if (line.find("function SetNodes() {") != std::string::npos) {
            ChangeLine = true;
        }
    }
    stream.close();
    
    vm->dostring(s.c_str());
}

void SetNodes(squall::VMStd* vm) {
    vm->call<void>("SetNodes");
}

void LoadNodeScripts(squall::VMStd* vm, std::vector<Node*> nodes) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->dofile(nodes[i]->ScriptLocation.c_str());
    }
}

void StartFunction(squall::VMStd* vm, std::vector<Node*> nodes) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->call<void>("SetNodeVal", nodes[i]->ScriptIndex, nodes[i]);
    }
    vm->call<void>("StartFunc");
    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->call<void>("GetNodeVal", nodes[i]->ScriptIndex, nodes[i], &nodes[i]->Position, &nodes[i]->Size);
    }
}

void UpdateFunction(squall::VMStd* vm, std::vector<Node*> nodes, float dt) {
    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->call<void>("SetNodeVal", nodes[i]->ScriptIndex, nodes[i]);
    }
    vm->call<void>("UpdateFunc", dt);
    for (int i = 0; i < (int)nodes.size(); i++) {
        vm->call<void>("GetNodeVal", nodes[i]->ScriptIndex, nodes[i], &nodes[i]->Position, &nodes[i]->Size);
    }
}