local Ns = []

function StartFunc(id) {
    Ns[id].Start();
    ::print("Start" + id.tostring());
}

function UpdateFunc(id, dt) {
    Ns[id].Update(dt);
}

function RemoveNodeFromArray(n) {
    for (local i = 0; i < Ns.len(); i++) {
        if (n == Ns[i]) {
            Ns.remove(i);
            break;
        }
    }
}

function GetNodesArray() {
    return Ns;
}

function GetNodeFromList(id) {
    return Ns[id];
}

function SetSpriteTexture(id, texture) {
    Ns[id].Texture = texture;
}

function SetPhysicsbodyBody(id, body) {
    Ns[id].Body = body;
}

function SetChildren(id, n) {
    Ns[id].Childs = n.Childs;
}

function SetNodeVal(id, node) {
    Ns[id].Position = node.Position;
    Ns[id].LocalPosition = node.LocalPosition;
    Ns[id].Size = node.Size;
    Ns[id].Angle = node.Angle;
    Ns[id].Name = node.Name;
}

function GetNodeVal(id, node) {
    node.Position = Ns[id].Position;
    node.LocalPosition = Ns[id].LocalPosition;
    node.Size = Ns[id].Size;
    node.Angle = Ns[id].Angle;
    node.Name = Ns[id].Name;
}