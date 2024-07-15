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
    Position = Vector2(0, 0);
}

local Ns = []

function StartFunc() {
    for (local i = 0; i < Ns.len(); i++) {
        Ns[i].Start();
    }
}

function SetNodes() {

}

function SetNodeVal(id, node) {
    Ns[id].Position.x = node.Position.x;
    Ns[id].Position.y = node.Position.y;
}

function GetNodeVal(id, node, pos) {
    pos.x = Ns[id].Position.x;
    pos.y = Ns[id].Position.y;
}