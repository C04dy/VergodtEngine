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
    node = Ns[id];
    //Ns[id].Position = node.Position;
    //Ns[id].Size = node.Size;
    //Ns[id].Angle = node.Angle;
    //Ns[id].Name = node.Name;
}

function SetPhysicsBodyVelocity(id, vel) {
    Ns[id].Velocity = vel;
}

function SetSpriteTexture(id, texture) {
    Ns[id].Texture = texture;
}

function GetNodeVal(id, node) {
    //node.Position = Ns[id].Position;
    //node.Size = Ns[id].Size;
    //node.Angle = Ns[id].Angle;
    //node.Name = Ns[id].Name;
}

function GetPhysicsBodyVelocity(id, vel) {
    vel = Ns[id].Velocity;
}