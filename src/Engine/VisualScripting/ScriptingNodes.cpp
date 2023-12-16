#include "ScriptingNodes.h"

// SCRIPTING NODE

ScriptingNode::~ScriptingNode(){
    for (int i = 0; i < (int)ConnectedNodes.size(); i++){
        delete ConnectedNodes[i];
        ConnectedNodes[i] = nullptr;
    }
}

void ScriptingNode::SendSignal(){
    for (int i = 0; i < (int)ConnectedNodes.size(); i++){
        ConnectedNodes[i]->ReciveSignal();
    }
}

void ScriptingNode::ReciveSignal(){
    NodesFunction();
    SendSignal();
}

// PRINT NODE

void PrintNode::NodesFunction(){
    std::cout << Message << '\n';
}

// KEYBOARD INPUT NODE

KeyboardInputNode::KeyboardInputNode(Input* Input, SDL_Keycode KeyCode, KeyboardInputType InputType){
    m_input = Input;
    m_input->SetInputNode(this);
    m_keycode = KeyCode;
    m_inputtype = InputType;
}

void KeyboardInputNode::SendSignal(){
    for (int i = 0; i < (int)ConnectedNodes.size(); i++){
        switch (m_inputtype)
        {
        case KeyboardInputType::NONE:
            break;
        case KeyboardInputType::IsKeyDown:
            if(m_input->IsKeyDown(m_keycode))
                ConnectedNodes[i]->ReciveSignal();
            break;
        case KeyboardInputType::IsKeyUp:
            if(m_input->IsKeyUp(m_keycode))
                ConnectedNodes[i]->ReciveSignal();
            break;
        case KeyboardInputType::IsKeyPressed:
            if(m_input->IsKeyPressed(m_keycode))
                ConnectedNodes[i]->ReciveSignal();
            break;
        case KeyboardInputType::IsKeyNotPressed:
            if(m_input->IsKeyNotPressed(m_keycode))
                ConnectedNodes[i]->ReciveSignal();
            break;
        }
    }
}

// MOUSE INPUT NODE

MouseInputNode::MouseInputNode(Input* Input, Uint8 MouseKey, MouseInputType InputType){
    m_input = Input;
    m_input->SetInputNode(this);
    m_mousekey = MouseKey;
    m_inputtype = InputType;
}

void MouseInputNode::SendSignal(){
    for (int i = 0; i < (int)ConnectedNodes.size(); i++){
        switch (m_inputtype)
        {
        case MouseInputType::NONE:
            break;
        case MouseInputType::IsMouseKeyDown:
            if(m_input->IsMouseKeyDown(m_mousekey))
                ConnectedNodes[i]->ReciveSignal();
            break;
        case MouseInputType::IsMouseKeyUp:
            if(m_input->IsMouseKeyUp(m_mousekey))
                ConnectedNodes[i]->ReciveSignal();
            break;
        case MouseInputType::IsMouseKeyPressed:
            if(m_input->IsMouseKeyPressed(m_mousekey))
                ConnectedNodes[i]->ReciveSignal();
            break;
        case MouseInputType::IsMouseKeyNotPressed:
            if(m_input->IsMouseKeyNotPressed(m_mousekey))
                ConnectedNodes[i]->ReciveSignal();
            break;
        }
    }
}

// VISUAL SCRIPT

VisualScript::VisualScript(StartNode* StartNode, UpdateNode* UpdateNode){
    m_startnode = StartNode;
    m_updatenode = UpdateNode;
}

VisualScript::~VisualScript(){
    delete m_startnode;
    m_startnode = nullptr;
    delete m_updatenode;
    m_updatenode = nullptr;
}

void VisualScript::StartScript(){
    m_startnode->ReciveSignal();
}

void VisualScript::UpdateScript(){
    m_updatenode->ReciveSignal();
}