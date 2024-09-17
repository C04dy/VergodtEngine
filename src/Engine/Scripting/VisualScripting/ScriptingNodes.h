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

class InputNode : public ScriptingNode
{
public:
    ~InputNode();
protected:
    InputManager* m_Input = nullptr;
};

class KeyboardInputNode : public InputNode
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

    Type m_InputType = Type::NONE;
};

class MouseInputNode : public InputNode
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
    VisualScript();
    ~VisualScript();

    void StartScript();
    
    void UpdateScript();

    void AddInputNode(InputNode* _InputNode);

    StartNode* Start = nullptr;

    UpdateNode* Update = nullptr;
private:

    std::vector<InputNode*> m_InputNodes;
};

#endif