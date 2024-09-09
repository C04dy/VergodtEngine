#include "ScriptingNodes.h"
#include "Objects/PhysicsBody.h"

SDL_Keycode GetKeyCode(const std::string& Type)
{
    if (Type == "A")
        return SDLK_A;
    return SDLK_UNKNOWN;
}

KeyboardInputNode::Type GetKeyboardInputType(const std::string& Type)
{
    if (Type == "ISKEYJUSTPRESSED")
        return KeyboardInputNode::Type::ISKEYJUSTPRESSED;
    else if(Type == "ISKEYJUSTRELEASED")
        return KeyboardInputNode::Type::ISKEYJUSTRELEASED;
    else if(Type == "ISKEYPRESSED")
        return KeyboardInputNode::Type::ISKEYPRESSED;
    else if(Type == "ISKEYNOTPRESSED")
        return KeyboardInputNode::Type::ISKEYNOTPRESSED;
    return KeyboardInputNode::Type::NONE;
}

MouseInputNode::Type GetMouseInputType(const std::string& Type)
{
    if (Type == "ISMOUSEKEYJUSTPRESSED")
        return MouseInputNode::Type::ISMOUSEKEYJUSTPRESSED;
    else if(Type == "ISMOUSEKEYJUSTRELEASED")
        return MouseInputNode::Type::ISMOUSEKEYJUSTRELEASED;
    else if(Type == "ISMOUSEKEYPRESSED")
        return MouseInputNode::Type::ISMOUSEKEYPRESSED;
    else if(Type == "ISMOUSEKEYNOTPRESSED")
        return MouseInputNode::Type::ISMOUSEKEYNOTPRESSED;
    return MouseInputNode::Type::NONE;
}

// SCRIPTING NODE

ScriptingNode::~ScriptingNode()
{
    ConnectedNodes.clear();
}

void ScriptingNode::SendSignal()
{
    for (ScriptingNode* scripting_node : ConnectedNodes)
    {
        scripting_node->ReciveSignal();
    }
}

void ScriptingNode::ReciveSignal()
{
    NodesFunction();
    SendSignal();
}

// PRINT NODE

void PrintNode::NodesFunction()
{
    std::cout << Message << '\n';
}

// KEYBOARD INPUT NODE

KeyboardInputNode::KeyboardInputNode(InputManager* Input, const std::string& KeyCode, const std::string& InputType)
{
    m_Input = Input;
    m_Input->SetInputNode(this);
    m_KeyCode = GetKeyCode(KeyCode);
    m_InputType = GetKeyboardInputType(InputType);
}

void KeyboardInputNode::SendSignal()
{
    for (ScriptingNode* scripting_node : ConnectedNodes)
    {
        switch (m_InputType)
        {
        case Type::NONE:
            std::cout << "ERROR: INVALID KEYBOARDINPUT TYPE" << '\n';
            break;
        case Type::ISKEYJUSTPRESSED:
            if(m_Input->IsKeyJustPressed(m_KeyCode))
                scripting_node->ReciveSignal();
            break;
        case Type::ISKEYJUSTRELEASED:
            if(m_Input->IsKeyJustReleased(m_KeyCode))
                scripting_node->ReciveSignal();
            break;
        case Type::ISKEYPRESSED:
            if(m_Input->IsKeyPressed(m_KeyCode))
                scripting_node->ReciveSignal();
            break;
        case Type::ISKEYNOTPRESSED:
            if(m_Input->IsKeyNotPressed(m_KeyCode))
                scripting_node->ReciveSignal();
            break;
        }
    }
}

// MOUSE INPUT NODE

MouseInputNode::MouseInputNode(InputManager* Input, Uint8 MouseKey, const std::string& InputType)
{
    m_Input = Input;
    m_Input->SetInputNode(this);
    m_MouseKey = MouseKey;
    m_InputType = GetMouseInputType(InputType);
}

void MouseInputNode::SendSignal()
{
    for (ScriptingNode* scripting_node : ConnectedNodes)
    {
        switch (m_InputType)
        {
        case Type::NONE:
            std::cout << "ERROR: INVALID MOUSEINPUT TYPE" << '\n';
            break;
        case Type::ISMOUSEKEYJUSTPRESSED:
            if (m_Input->IsMouseKeyJustPressed(m_MouseKey))
                scripting_node->ReciveSignal();
            break;
        case Type::ISMOUSEKEYJUSTRELEASED:
            if(m_Input->IsMouseKeyJustReleased(m_MouseKey))
                scripting_node->ReciveSignal();
            break;
        case Type::ISMOUSEKEYPRESSED:
            if(m_Input->IsMouseKeyPressed(m_MouseKey))
                scripting_node->ReciveSignal();
            break;
        case Type::ISMOUSEKEYNOTPRESSED:
            if(m_Input->IsMouseKeyNotPressed(m_MouseKey))
                scripting_node->ReciveSignal();
            break;
        }
    }
}

// CONDITION NODE

ConditionNode::ConditionNode(bool* Condition)
{
    m_Condition = Condition;
}

ConditionNode::~ConditionNode()
{
    ConnectedNodes.clear();
    ConnectedNodesToFalse.clear();
    ConnectedNodesToTrue.clear();
    m_Condition = nullptr;
}

void ConditionNode::SendSignal()
{
    if (m_Condition != nullptr)
    {
        if (*m_Condition == true)
        {
            for (ScriptingNode* scripting_node : ConnectedNodesToTrue)
            {
                scripting_node->ReciveSignal();
            }
        }
        else
        {
            for (ScriptingNode* scripting_node : ConnectedNodesToFalse)
            {
                scripting_node->ReciveSignal();
            }
        }
    }
    for (ScriptingNode* scripting_node : ConnectedNodes)
    {
        scripting_node->ReciveSignal();
    }
}

// ADDFORCE NODE 

ApplyForceNode::ApplyForceNode(PhysicsBody* Body, const Vector2& Force)
{
    m_Body = Body;
    m_Force = Force;
}

ApplyForceNode::~ApplyForceNode()
{
    m_Body = nullptr;
}

void ApplyForceNode::NodesFunction()
{
    m_Body->GetBody()->ApplyForceToCenter(m_Force, true);
}

// SETFORCE NODE 

SetVelocityNode::SetVelocityNode(PhysicsBody* Body, const Vector2& Velocity)
{
    m_Body = Body;
    m_Velocity = Velocity;
}

SetVelocityNode::~SetVelocityNode()
{
    m_Body = nullptr;
}

void SetVelocityNode::NodesFunction()
{
    m_Body->GetBody()->SetLinearVelocity(m_Velocity);
}

// VISUAL SCRIPT

void VisualScript::InitVisualScript(StartNode* StartNode, UpdateNode* UpdateNode)
{
    m_StartNode = StartNode;
    m_UpdateNode = UpdateNode;
}

VisualScript::~VisualScript()
{
    delete m_StartNode;
    m_StartNode = nullptr;
    delete m_UpdateNode;
    m_UpdateNode = nullptr;
}

void VisualScript::StartScript()
{
    m_StartNode->ReciveSignal();
}

void VisualScript::UpdateScript()
{
    m_UpdateNode->ReciveSignal();
}