#include <iostream>
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

    void InitNode(Node* node, lua_State* L){
        luabridge::LuaRef init = luabridge::getGlobal(L, "InitNode");
        init(node);
    }

    void SetNode(Node* node, lua_State* L){
        luabridge::LuaRef set = luabridge::getGlobal(L, "SetNode");
        set(node);
    }
}