class bird extends PhysicsBody {
    function Start() {

    }

    function Update(dt) {
        if (::IsMouseKeyJustPressed(1)) {
            SetPosition(Position.AddToVec2(::Vector2(5.0, 0.0)));
        }
    }
}