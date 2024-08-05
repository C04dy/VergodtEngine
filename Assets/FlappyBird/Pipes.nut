class Pipes extends PhysicsBody {
    function Start() {
        Body.SetFixedRotation(true);
        Body.SetGravity(0.0);


    }

    function Update(dt) {
        ::print("hi");
        Body.SetVelocity(::Vector2(-5.0, 0.0));
        //Position = ::Vector2(250.0, 500.0);
    }
}