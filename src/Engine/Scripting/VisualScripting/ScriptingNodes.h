#pragma once
#ifndef SCRIPTINGNODES_H
#define SCRIPTINGNODES_H

#include <iostream>
#include <string>
#include <vector>
#include "../Input.h"

class ScriptingNode
{
public:
    virtual ~ScriptingNode();

    virtual void SendSignal();

    virtual void ReciveSignal();

    virtual void NodesFunction() {}

    std::vector<ScriptingNode*> ConnectedNodes;
};

class StartNode : public ScriptingNode { };

class UpdateNode : public ScriptingNode { };

class PrintNode : public ScriptingNode
{
public:
    void NodesFunction() override;

    std::string Message = "";
};

class KeyboardInputNode : public ScriptingNode
{
public:
    enum class Type
    {
        NONE = 0,
        ISKEYJUSTPRESSED = 1,
        ISKEYJUSTRELEASED = 2,
        ISKEYPRESSED = 3,
        ISKEYNOTPRESSED = 4
    };
public:
    KeyboardInputNode(InputManager* Input, const std::string& KeyCode, const std::string& InputType);

    void SendSignal() override;
private:
    SDL_Keycode m_KeyCode;

    InputManager* m_Input = nullptr;

    Type m_InputType = Type::NONE;
};

class MouseInputNode : public ScriptingNode
{
public:
    enum class Type{
        NONE = 0,
        ISMOUSEKEYJUSTPRESSED = 1,
        ISMOUSEKEYJUSTRELEASED = 2,
        ISMOUSEKEYPRESSED = 3,
        ISMOUSEKEYNOTPRESSED = 4
    };
public:
    MouseInputNode(InputManager* Input, Uint8 MouseKey, const std::string& InputType);

    void SendSignal() override;

    int X, Y;
private:
    Uint8 m_MouseKey;

    InputManager* m_Input = nullptr;

    Type m_InputType = Type::NONE;
};

class ConditionNode : public ScriptingNode
{
public:
    ~ConditionNode();

    ConditionNode(bool* Condition);

    void SendSignal() override;

    std::vector<ScriptingNode*> ConnectedNodesToTrue;
    std::vector<ScriptingNode*> ConnectedNodesToFalse;
private:
    bool* m_Condition = nullptr;
};

class PhysicsBody;

class ApplyForceNode : public ScriptingNode
{
public:
    ApplyForceNode(PhysicsBody* Body, const Vector2& Force);

    ~ApplyForceNode();

    void NodesFunction() override;
private:
    PhysicsBody* m_Body = nullptr;

    Vector2 m_Force;
};

class SetVelocityNode : public ScriptingNode
{
public:
    SetVelocityNode(PhysicsBody* Body, const Vector2& Force);

    ~SetVelocityNode();

    void NodesFunction() override;
private:
    PhysicsBody* m_Body = nullptr;

    Vector2 m_Velocity;
};

class VisualScript
{
public:
    void InitVisualScript(StartNode* StartNode = nullptr, UpdateNode* UpdateNode = nullptr);

    ~VisualScript();

    void StartScript();
    
    void UpdateScript();
private:
    StartNode* m_StartNode = nullptr;

    UpdateNode* m_UpdateNode = nullptr;
};

#endif