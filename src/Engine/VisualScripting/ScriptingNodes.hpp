#pragma once

#include <iostream>
#include <string>
#include <vector>

class ScriptingNode
{
public:
    virtual void SendSignal() {}
    virtual void ReciveSignal() {}
};

class StartNode : public ScriptingNode
{
public:
    ~StartNode(){
        ConnectedNode = nullptr;
    }

    void SendSignal() override {
        ConnectedNode->ReciveSignal();
    }

    void ReciveSignal() override {
        SendSignal();
    }

    ScriptingNode* ConnectedNode = nullptr;
};

class UpdateNode : public ScriptingNode
{
public:
    ~UpdateNode(){
        ConnectedNode = nullptr;
    }

    void SendSignal() override {
        ConnectedNode->ReciveSignal();
    }

    void ReciveSignal() override {
        SendSignal();
    }

    ScriptingNode* ConnectedNode = nullptr;
};

class PrintNode : public ScriptingNode
{
public:
    void ReciveSignal() override {
        std::cout << Message << '\n';
    }

    std::string Message = "";
};

class VisualScript
{
public:
    VisualScript(StartNode* StartNode, UpdateNode* UpdateNode){
        m_startnode = StartNode;
        m_updatenode = UpdateNode;
    }

    ~VisualScript(){
        delete m_startnode;
        m_startnode = nullptr;
        delete m_updatenode;
        m_updatenode = nullptr;
    }

    void StartScript(){
        m_startnode->ReciveSignal();
    }
    
    void UpdateScript(){
        m_updatenode->ReciveSignal();
    }
private:
    StartNode* m_startnode = nullptr;

    UpdateNode* m_updatenode = nullptr;
};