#include <iostream>
#include <string>
#include <lua.hpp>
#include <LuaBridge.h>


namespace LuaScript{

    void Wrap(lua_State* L){

        luabridge::getGlobalNamespace(L)
            .beginClass<Vector2>("Vector2")
            .addConstructor<void (float, float)>()
            .addProperty("x", &Vector2::GetX, &Vector2::SetX)
            .addProperty("y", &Vector2::GetY, &Vector2::SetY)
            .endClass();

        luabridge::getGlobalNamespace(L)
            .beginClass<Transform>("Transform")
            .addProperty("Position", &Transform::Position)
            .addProperty("Size", &Transform::Size)
            .addProperty("Angle", &Transform::Angle)
            .endClass();

        luabridge::getGlobalNamespace(L)
            .beginClass<Node>("Node")
            .addProperty("Name", &Node::Name)
            .addProperty("Transform", &Node::Transform)
            .endClass();
        
        luabridge::getGlobalNamespace(L)
            .beginClass<Sprite>("Sprite")
            .endClass();

    }

    void InitNode(lua_State* L, Node* node){
        luabridge::LuaRef init = luabridge::getGlobal(L, "InitNode");
        init(node);

        std::string startfunc = node->Name + "_Start";

        luabridge::LuaRef start = luabridge::getGlobal(L, startfunc.c_str());
        start();
    }

    void UpdateNode(lua_State* L, Node* node){
        luabridge::LuaRef update = luabridge::getGlobal(L, (node->Name + "_Update").c_str());
        update();
    }
}