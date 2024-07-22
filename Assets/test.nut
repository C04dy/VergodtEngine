class test extends PhysicsBody {
    function Start() {
        Body.SetFixedRotation(true);

        Angle = 0.0;
    }

    function Update(dt) {
        Body.SetVelocity(::Vector2(10000.0 * dt * (::IsKeyPressed(100).tofloat() - ::IsKeyPressed(97).tofloat()), Body.GetVelocity().y));

        if (::IsKeyJustPressed(32)) {
            Body.SetVelocity(::Vector2(Body.GetVelocity().x, -5.0));
        }
    }
}