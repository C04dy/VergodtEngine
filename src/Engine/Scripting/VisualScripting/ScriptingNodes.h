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
    void NodesFunction() override ;

    std::string Message = "";
};

class KeyboardInputNode : public ScriptingNode
{
public:
    enum class KeyboardInputType{
        NONE,
        IsKeyJustPressed,
        IsKeyJustReleased,
        IsKeyPressed,
        IsKeyNotPressed
    };
public:
    KeyboardInputNode(InputManager* Input, const std::string& KeyCode, const std::string& InputType);

    void SendSignal() override;
private:
    SDL_Keycode m_keycode;

    InputManager* m_input = nullptr;

    KeyboardInputType m_inputtype = KeyboardInputType::NONE;
};

class MouseInputNode : public ScriptingNode
{
public:
    enum class MouseInputType{
        NONE,
        IsMouseKeyJustPressed,
        IsMouseKeyJustReleased,
        IsMouseKeyPressed,
        IsMouseKeyNotPressed
    };
public:
    MouseInputNode(InputManager* Input, Uint8 MouseKey, const std::string& InputType);

    void SendSignal() override;

    int X, Y;
private:
    Uint8 m_mousekey;

    InputManager* m_input = nullptr;

    MouseInputType m_inputtype = MouseInputType::NONE;
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
    bool* m_condition = nullptr;
};

class VisualScript
{
public:
    void InitVisualScript(StartNode* StartNode = nullptr, UpdateNode* UpdateNode = nullptr);

    ~VisualScript();

    void StartScript();
    
    void UpdateScript();
private:
    StartNode* m_startnode = nullptr;

    UpdateNode* m_updatenode = nullptr;
};

#endif