//class Vector2
//{
//    constructor(xIn, yIn) {
//        x = xIn;
//        y = yIn;
//    }
//    x = 0.0;
//    y = 0.0;
//}
//
//class Node
//{
//    constructor(name){
//        Name = name;
//    }
//    Position = Vector2(0, 0);
//    Size = Vector2(0, 0);
//    Angle = 0.0;
//    Name = "";
//}
//
//class Sprite extends Node
//{
//    Texture = null;
//}
//
//class PhysicsBody extends Node
//{
//    Velocity = Vector2(0, 0);
//
//    Body = null;
//}

local Ns = []

function StartFunc() {
    foreach (val in Ns) {
        val.Start();
    }
}

function UpdateFunc(id, dt) {
    Ns[id].Update(dt);
}

function SetNodes() {

}

function SetNodeVal(id, node) {
    Ns[id] = node;
    Ns[id].Position.x = node.Position.x;
    Ns[id].Position.y = node.Position.y;

    Ns[id].Size.x = node.Size.x;
    Ns[id].Size.y = node.Size.y;

    Ns[id].Angle = node.Angle;

    Ns[id].Name = node.Name;
}

function SetSpriteTexture(id, n) {
    Ns[id].Texture = n;
}

function SetPhysicsBody(id, n) {
    Ns[id].Body = n;
}

function SetPhysicsBodyVelocity(id, vel) {
    Ns[id].Velocity.x = vel.x;
    Ns[id].Velocity.y = vel.y;
}

function GetNodeVal(id, node, pos, size) {
    pos.x = Ns[id].Position.x;
    pos.y = Ns[id].Position.y;

    size.x = Ns[id].Size.x;
    size.y = Ns[id].Size.y;

    node.Angle = Ns[id].Angle;

    node.Name = Ns[id].Name;
}

function GetPhysicsBodyVelocity(id, vel) {
    vel.x = Ns[id].Velocity.x;
    vel.y = Ns[id].Velocity.y;
}