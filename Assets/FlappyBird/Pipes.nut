class Pipes extends PhysicsBody {
    function Start() {
        Body.SetFixedRotation(true);
        Body.SetGravity(0.0);
    }

    function Update(dt) {
        Body.SetVelocity(::Vector2(-5.0, 0.0));
        ::print(LocalPosition.y);
    }
}