class Vector2
{
    constructor(xIn, yIn) {
        x = xIn;
        y = yIn;
    }
    x = 0.0;
    y = 0.0;
}

class Node
{
    constructor(name){
        Name = name;
    }
    Position = Vector2(0, 0);
    Size = Vector2(0, 0);
    Name = "";
}

local Ns = []

function StartFunc() {
    for (local i = 0; i < Ns.len(); i++) {
        Ns[i].Start();
    }
}

function UpdateFunc(dt) {
    for (local i = 0; i < Ns.len(); i++) {
        Ns[i].Update(dt);
    }
}

function SetNodes() {

}

function SetNodeVal(id, node) {
    Ns[id].Position.x = node.Position.x;
    Ns[id].Position.y = node.Position.y;

    Ns[id].Size.x = node.Size.x;
    Ns[id].Size.y = node.Size.y;

    Ns[id].Name = node.Name;
}

function GetNodeVal(id, node, pos, size) {
    pos.x = Ns[id].Position.x;
    pos.y = Ns[id].Position.y;

    size.x = Ns[id].Size.x;
    size.y = Ns[id].Size.y;

    node.Name = Ns[id].Name;
}