#pragma once
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
        IsKeyDown,
        IsKeyUp,
        IsKeyPressed,
        IsKeyNotPressed
    };
public:
    KeyboardInputNode(Input* Input, SDL_Keycode KeyCode, KeyboardInputType InputType);

    void SendSignal() override;
private:
    SDL_Keycode m_keycode;

    Input* m_input = nullptr;

    KeyboardInputType m_inputtype = KeyboardInputType::NONE;
};

class MouseInputNode : public ScriptingNode
{
public:
    enum class MouseInputType{
        NONE,
        IsMouseKeyDown,
        IsMouseKeyUp,
        IsMouseKeyPressed,
        IsMouseKeyNotPressed
    };
public:
    MouseInputNode(Input* Input, Uint8 MouseKey, MouseInputType InputType);

    void SendSignal() override;

    int X, Y;
private:
    Uint8 m_mousekey;

    Input* m_input = nullptr;

    MouseInputType m_inputtype = MouseInputType::NONE;
};

class VisualScript
{
public:
    VisualScript(StartNode* StartNode, UpdateNode* UpdateNode);

    ~VisualScript();

    void StartScript();
    
    void UpdateScript();
private:
    StartNode* m_startnode = nullptr;

    UpdateNode* m_updatenode = nullptr;
};