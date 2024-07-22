class bird extends PhysicsBody {
    function Start() {

    }

    function Update(dt) {
        if (::IsMouseKeyPressed(1)) {
            Body.SetVelocity(::Vector2(0.0, -10.0));
        }
    }
}