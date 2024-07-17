class test extends PhysicsBody{
    function Start() {
        //Position.x = 100.0;

        Body.SetFixedRotation(true);

        Angle = 0.0;
    }

    function Update(dt) {
        Velocity.x += 10 * dt * (IsKeyPressed(100).tofloat() - IsKeyPressed(97).tofloat());
        if (IsMouseKeyJustReleased(1)) {
            Velocity.y = -5.0;
        }
    }
}