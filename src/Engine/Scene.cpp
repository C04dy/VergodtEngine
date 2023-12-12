#include "Scene.h"
#include <string>
#include "VisualScripting/ScriptingNodes.hpp"

//#include <pybind11/embed.h>

//namespace py = pybind11;

template <typename T>
void Log(T log)
{
    std::cout << log << "\n";
}

/* Scene file stuff
enum NodeType{
    Null,
    NODE,
    SPRITE,
    CAM,
    PHYSICSBODY
};

NodeType GetNodeType(std::string const& inString){
    if (inString == "NODE") return NODE;
    if (inString == "SPRITE") return SPRITE;
    if (inString == "CAMERA") return CAM;
    if (inString == "PHYSICSBODY") return PHYSICSBODY;
    return Null;
}*/

void Scene::Start(){
    //py::scoped_interpreter guard{};
    //py::print("Hello, World!");
    
    /*std::ifstream script("../Assets/test.py");
    std::string line;
    
    while (std::getline(script, line))
    {
        py::exec(line.c_str());
    }
    script.close();*/
    

    /* TODO: Implement the scene file Load
    std::string Line;

    std::ifstream SceneFile("../Assets/test.vscene");

    SceneFile.close();*/
    Cam = new Camera;

    Test1s = new Sprite("../Assets/Test1.png", Cam, Renderer);
    Test1p = new PhysicsBody;
    Test1p->Transform.Position = Vector2(250, 400);
    Test1p->Transform.Angle = 44;
    Test1p->InitPhysicsBodyBox(PhysicsWorld, b2BodyType::b2_dynamicBody, Vector2( 100, 100 ));
    Test1p->AddChild(Test1s);

    Test2s = new Sprite("../Assets/Test2.png", Cam, Renderer);
    Test2p = new PhysicsBody;
    Test2p->Transform.Position = Vector2(250, 250);
    Test2p->InitPhysicsBodyCircle(PhysicsWorld, b2BodyType::b2_dynamicBody, 50);
    Test2p->AddChild(Test2s);

    Test3s = new Sprite("../Assets/Test1.png", Cam, Renderer);
    Test3p = new PhysicsBody;
    Test3p->Transform.Position = Vector2(250, 550);
    Test3p->Transform.Size = Vector2(3, 1);
    Test3p->InitPhysicsBodyBox(PhysicsWorld, b2BodyType::b2_staticBody, Vector2( 100, 100 ));
    Test3p->AddChild(Test3s);

    Test4s = new Sprite("../Assets/Test1.png", Cam, Renderer);
    Test4p = new PhysicsBody;
    Test4p->Transform.Position = Vector2(550, 650);
    Test4p->Transform.Size = Vector2(3, 1);
    Test4p->InitPhysicsBodyBox(PhysicsWorld, b2BodyType::b2_staticBody, Vector2( 100, 100 ));
    Test4p->AddChild(Test4s);

    Test5s = new Sprite("../Assets/Test3.png", Cam, Renderer);
    Test5p = new PhysicsBody;
    Test5p->Transform.Position = Vector2(250, 100);
    Test5p->Transform.Angle = 20;

    Vector2 polygons[3];
    polygons[0] = Vector2(0, -50);
    polygons[1] = Vector2(50, 50);
    polygons[2] = Vector2(-50, 50);

    Test5p->InitPhysicsBodyPolygon(PhysicsWorld, b2BodyType::b2_dynamicBody, polygons, 3);
    Test5p->AddChild(Test5s);

    StartNode* s = new StartNode;

    UpdateNode* u = new UpdateNode;

    PrintNode* printn1 = new PrintNode;
    printn1->Message = "Start";

    PrintNode* printn2 = new PrintNode;
    printn2->Message = "Update";

    s->ConnectedNode = printn1;
    u->ConnectedNode = printn2;

    script = new VisualScript(s, u);
    
    script->StartScript();
}

void Scene::Update(double dt){
    Test1p->UpdatePhysicsNode();
    Test2p->UpdatePhysicsNode();
    Test3p->UpdatePhysicsNode();
    Test4p->UpdatePhysicsNode();
    Test5p->UpdatePhysicsNode();
    
    Test1p->UpdateChild();
    Test2p->UpdateChild();
    Test3p->UpdateChild();
    Test4p->UpdateChild();
    Test5p->UpdateChild();

    script->UpdateScript();
}

void Scene::Draw(){
    Test1s->DrawImage();
    Test2s->DrawImage();
    Test3s->DrawImage();
    Test4s->DrawImage();
    Test5s->DrawImage();
}

void Scene::Clean(){
    delete Test1s;
    Test1s = nullptr;
    delete Test1p;
    Test1p = nullptr;
    
    delete Test2s;
    Test2s = nullptr;
    delete Test2p;
    Test2p = nullptr;

    delete Test3s;
    Test3s = nullptr;
    delete Test3p;
    Test3p = nullptr;

    delete Test4s;
    Test4s = nullptr;
    delete Test4p;
    Test4p = nullptr;

    delete Test5s;
    Test5s = nullptr;
    delete Test5p;
    Test5p = nullptr;

    delete Cam;
    Cam = nullptr;

    delete script;
    script = nullptr;
}