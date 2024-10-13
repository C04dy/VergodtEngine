#include "ScriptingNodes.h"
#include "Objects/PhysicsBody.h"

SDL_Keycode GetKeyCode(const std::string& Type)
{
    if (Type == "A")
        return SDLK_A;
    else if (Type == "B")
        return SDLK_B;
    else if (Type == "C")
        return SDLK_C;
    else if (Type == "D")
        return SDLK_D;
    else if (Type == "E")
        return SDLK_E;
    else if (Type == "F")
        return SDLK_F;
    else if (Type == "G")
        return SDLK_G;
    else if (Type == "H")
        return SDLK_I;
    else if (Type == "J")
        return SDLK_J;
    else if (Type == "K")
        return SDLK_K;
    else if (Type == "L")
        return SDLK_L;
    else if (Type == "M")
        return SDLK_M;
    else if (Type == "N")
        return SDLK_N;
    else if (Type == "O")
        return SDLK_O;
    else if (Type == "P")
        return SDLK_P;
    else if (Type == "Q")
        return SDLK_Q;
    else if (Type == "R")
        return SDLK_R;
    else if (Type == "S")
        return SDLK_S;
    else if (Type == "T")
        return SDLK_T;
    else if (Type == "U")
        return SDLK_U;
    else if (Type == "V")
        return SDLK_V;
    else if (Type == "W")
        return SDLK_W;
    else if (Type == "X")
        return SDLK_X;
    else if (Type == "Y")
        return SDLK_Y;
    else if (Type == "Z")
        return SDLK_Z;
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
    for (ScriptingNode* s : ConnectedNodes)
    {
        delete s;
        s = nullptr;
    }
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
    std::cout << Message << std::endl;
}

// INPUT NODE

InputNode::~InputNode()
{
    m_Input = nullptr;
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
    for (ScriptingNode* s : ConnectedNodesToFalse)
    {
        delete s;
        s = nullptr;
    }
    for (ScriptingNode* s : ConnectedNodesToTrue)
    {
        delete s;
        s = nullptr;
    }

    ConnectedNodesToFalse.clear();
    ConnectedNodesToTrue.clear();
    delete m_Condition;
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

ApplyForceNode::~ApplyForceNode()
{
    m_BodyID = nullptr;
}

ApplyForceNode::ApplyForceNode(b2BodyId& BodyID, const Vector2& Force)
{
    m_BodyID = &BodyID;
    m_Force = Force;
}

void ApplyForceNode::NodesFunction()
{
    b2Body_ApplyForceToCenter(*m_BodyID, m_Force, true);
}

// SETFORCE NODE 

SetVelocityNode::~SetVelocityNode()
{
    m_BodyID = nullptr;
}

SetVelocityNode::SetVelocityNode(b2BodyId& BodyID, const Vector2& Velocity)
{
    m_BodyID = &BodyID;
    m_Velocity = Velocity;
}

void SetVelocityNode::NodesFunction()
{
    b2Body_SetLinearVelocity(*m_BodyID, m_Velocity);
}

// VISUAL SCRIPT

VisualScript::VisualScript()
{
    Start = new StartNode;
    Update = new UpdateNode;
}

VisualScript::~VisualScript()
{
    delete Start;
    Start = nullptr;
    delete Update;
    Update = nullptr;

    for (InputNode* i : m_InputNodes)
    {
        delete i;
        i = nullptr;
    }
    m_InputNodes.clear();
}

void VisualScript::StartScript()
{
    Start->ReciveSignal();
}

void VisualScript::UpdateScript()
{
    Update->ReciveSignal();
}

void VisualScript::AddInputNode(InputNode* _InputNode)
{
    m_InputNodes.push_back(_InputNode);
}