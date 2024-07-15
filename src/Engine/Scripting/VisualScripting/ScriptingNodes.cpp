#include "ScriptingNodes.h"

/* old stuff

SDL_Keycode GetKeyCode(const std::string& Type){
    if(Type == "A")
        return SDLK_a;
}

KeyboardInputNode::KeyboardInputType GetKeyboardInputType(const std::string& Type){
    if(Type == "ISKEYDOWN")
        return KeyboardInputNode::KeyboardInputType::IsKeyDown;
    if(Type == "ISKEYNOTPRESSED")
        return KeyboardInputNode::KeyboardInputType::IsKeyNotPressed;
    if(Type == "ISKEYPRESSED")
        return KeyboardInputNode::KeyboardInputType::IsKeyPressed;
    if(Type == "ISKEYUP")
        return KeyboardInputNode::KeyboardInputType::IsKeyUp;
}

MouseInputNode::MouseInputType GetMouseInputType(const std::string& Type){
    if(Type == "ISMOUSEKEYDOWN")
        return MouseInputNode::MouseInputType::IsMouseKeyDown;
    if(Type == "ISMOUSEKEYNOTPRESSED")
        return MouseInputNode::MouseInputType::IsMouseKeyNotPressed;
    if(Type == "ISMOUSEKEYPRESSED")
        return MouseInputNode::MouseInputType::IsMouseKeyPressed;
    if(Type == "ISMOUSEKEYUP")
        return MouseInputNode::MouseInputType::IsMouseKeyUp;
}

void Scene::SetScript(Node* n, const std::string& Line){
    if(GetLineBetween(Line, "[SCRIPT=", "]") != "NULL"){
        std::string ln;
        std::ifstream ScriptFile(GetLineBetween(Line, "[SCRIPT=", "]"));

        StartNode* s = new StartNode;
        UpdateNode* u = new UpdateNode;
        n->Script = new VisualScript;

        n->Script->InitVisualScript(s, u);

        std::vector<ScriptingNode*> ALLSCRIPNODES;

        while (std::getline(ScriptFile, ln)){
            std::string NodeType = GetLineBetween(ln, "[TYPE=", "]");

            if(NodeType == "START"){
                ALLSCRIPNODES.push_back(s);
            }else if(NodeType == "UPDATE"){
                ALLSCRIPNODES.push_back(u);
            }else if(NodeType == "PRINT"){
                PrintNode* p = new PrintNode;

                p->Message = GetLineBetween(ln, "(", ")");

                ALLSCRIPNODES.push_back(p);

                if(GetLineBetween(ln, "[CONNECTEDID=", "]") != "NULL")
                    ALLSCRIPNODES[std::stoi(GetLineBetween(ln, "[CONNECTEDID=", "]"))]->ConnectedNodes.push_back(p); 
            }else if(NodeType == "KEYBOARDINPUT"){
                KeyboardInputNode* k = new KeyboardInputNode(Input, 
                                                            GetKeyCode(GetLineBetween(ln, "[KEY=", "]")),
                                                            GetKeyboardInputType(GetLineBetween(ln, "[INPUTTYPE=", "]")));

                ALLSCRIPNODES.push_back(k);
            }else if(NodeType == "MOUSEINPUT"){
                MouseInputNode* k = new MouseInputNode(Input, 
                                                        std::stoi(GetLineBetween(ln, "[KEY=", "]")),
                                                        GetMouseInputType(GetLineBetween(ln, "[INPUTTYPE=", "]")));

                ALLSCRIPNODES.push_back(k);
            }
        }

        ScriptFile.close();
    }
}

*/

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

KeyboardInputNode::KeyboardInputNode(InputManager* Input, SDL_Keycode KeyCode, KeyboardInputType InputType){
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
            std::cout << "ERROR: INVALID KEYBOARDINPUT TYPE" << '\n';
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

MouseInputNode::MouseInputNode(InputManager* Input, Uint8 MouseKey, MouseInputType InputType){
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
            std::cout << "ERROR: INVALID MOUSEINPUT TYPE" << '\n';
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