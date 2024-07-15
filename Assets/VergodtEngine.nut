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

function SetNodeVal(node) {
    Ns[Ns.len() - 1].Position.x = node.Position.x;
    Ns[Ns.len() - 1].Position.y = node.Position.y;
}

function GetNodeVal(node, pos) {
    pos.x = Ns[Ns.len() - 1].Position.x;
    pos.y = Ns[Ns.len() - 1].Position.y;
}