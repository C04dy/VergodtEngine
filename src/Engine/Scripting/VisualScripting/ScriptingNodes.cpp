#include "ScriptingNodes.h"
#include "Objects/PhysicsBody.h"

SDL_Keycode GetKeyCode(const std::string& Type){
    if(Type == "A")
        return SDLK_A;
}

KeyboardInputNode::KeyboardInputType GetKeyboardInputType(const std::string& Type){
    if(Type == "ISKEYJUSTPRESSED")
        return KeyboardInputNode::KeyboardInputType::IsKeyJustPressed;
    if(Type == "ISKEYJUSTRELEASED")
        return KeyboardInputNode::KeyboardInputType::IsKeyJustReleased;
    if(Type == "ISKEYPRESSED")
        return KeyboardInputNode::KeyboardInputType::IsKeyPressed;
    if(Type == "ISKEYNOTPRESSED")
        return KeyboardInputNode::KeyboardInputType::IsKeyNotPressed;
}

MouseInputNode::MouseInputType GetMouseInputType(const std::string& Type){
    if(Type == "ISMOUSEKEYJUSTPRESSED")
        return MouseInputNode::MouseInputType::IsMouseKeyJustPressed;
    if(Type == "ISMOUSEKEYJUSTRELEASED")
        return MouseInputNode::MouseInputType::IsMouseKeyJustReleased;
    if(Type == "ISMOUSEKEYPRESSED")
        return MouseInputNode::MouseInputType::IsMouseKeyPressed;
    if(Type == "ISMOUSEKEYNOTPRESSED")
        return MouseInputNode::MouseInputType::IsMouseKeyNotPressed;
}

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

KeyboardInputNode::KeyboardInputNode(InputManager* Input, const std::string& KeyCode, const std::string& InputType){
    m_input = Input;
    m_input->SetInputNode(this);
    m_keycode = GetKeyCode(KeyCode);
    m_inputtype = GetKeyboardInputType(InputType);
}

void KeyboardInputNode::SendSignal(){
    for (int i = 0; i < (int)ConnectedNodes.size(); i++){
        switch (m_inputtype)
        {
        case KeyboardInputType::NONE:
            std::cout << "ERROR: INVALID KEYBOARDINPUT TYPE" << '\n';
            break;
        case KeyboardInputType::IsKeyJustPressed:
            if(m_input->IsKeyJustPressed(m_keycode))
                ConnectedNodes[i]->ReciveSignal();
            break;
        case KeyboardInputType::IsKeyJustReleased:
            if(m_input->IsKeyJustReleased(m_keycode))
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

MouseInputNode::MouseInputNode(InputManager* Input, Uint8 MouseKey, const std::string& InputType){
    m_input = Input;
    m_input->SetInputNode(this);
    m_mousekey = MouseKey;
    m_inputtype = GetMouseInputType(InputType);
}

void MouseInputNode::SendSignal() {
    for (int i = 0; i < (int)ConnectedNodes.size(); i++) {
        switch (m_inputtype)
        {
        case MouseInputType::NONE:
            std::cout << "ERROR: INVALID MOUSEINPUT TYPE" << '\n';
            break;
        case MouseInputType::IsMouseKeyJustPressed:
            if (m_input->IsMouseKeyJustPressed(m_mousekey))
                ConnectedNodes[i]->ReciveSignal();
            break;
        case MouseInputType::IsMouseKeyJustReleased:
            if(m_input->IsMouseKeyJustReleased(m_mousekey))
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

// ADDFORCE NODE 

ApplyForceNode::ApplyForceNode(PhysicsBody* Body, const Vector2& Force) {
    m_body = Body;
    m_force = Force;
}

ApplyForceNode::~ApplyForceNode() {
    m_body = nullptr;
}

void ApplyForceNode::NodesFunction() {
    m_body->GetBody()->ApplyForceToCenter(m_force, true);
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