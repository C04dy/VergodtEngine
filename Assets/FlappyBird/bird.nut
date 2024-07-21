local b = true;
class bird extends PhysicsBody {
    function Start() {
        //::print(10);
    }

    function Update(dt) {
        if (::IsMouseKeyPressed(1)) {
            if (Name == "bird" && b == true) {
                ::InstantiateNodesFromFile("../Assets/FlappyBird/OtherBird.vscene");
                b = false;
            }
            Body.SetVelocity(::Vector2(0.0, -10.0));
        }
    }
}