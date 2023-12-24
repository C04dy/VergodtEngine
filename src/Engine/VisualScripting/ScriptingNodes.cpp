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

// CONDITION NODE

ConditionNode::ConditionNode(bool* Condition){
    m_condition = Condition;
}

ConditionNode::~ConditionNode(){
    for (int i = 0; i < (int)ConnectedNodes.size(); i++)
    {
        delete ConnectedNodes[i];
        ConnectedNodes[i] = nullptr;
    }
    for (int i = 0; i < (int)ConnectedNodesToFalse.size(); i++)
    {
        delete ConnectedNodesToFalse[i];
        ConnectedNodesToFalse[i] = nullptr;
    }
    for (int i = 0; i < (int)ConnectedNodesToTrue.size(); i++)
    {
        delete ConnectedNodesToTrue[i];
        ConnectedNodesToTrue[i] = nullptr;
    }
    m_condition = nullptr;
}

void ConditionNode::SendSignal(){
    if(m_condition != nullptr){
        if(*m_condition == true){
            for (int i = 0; i < (int)ConnectedNodesToTrue.size(); i++)
            {
                ConnectedNodesToTrue[i]->ReciveSignal();
            }
        }else{
            for (int i = 0; i < (int)ConnectedNodesToFalse.size(); i++)
            {
                ConnectedNodesToFalse[i]->ReciveSignal();
            }
        }
    }
    for (int i = 0; i < (int)ConnectedNodes.size(); i++)
    {
        ConnectedNodes[i]->ReciveSignal();
    }
}

// VISUAL SCRIPT

void VisualScript::InitVisualScript(StartNode* StartNode, UpdateNode* UpdateNode){
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