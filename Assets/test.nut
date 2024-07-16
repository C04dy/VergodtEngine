class test extends PhysicsBody{
    function Start() {
        Position.x += 400;
    }

    function Update(dt) {
        Velocity.x += 10 * dt * (IsKeyPressed(100).tofloat() - IsKeyPressed(97).tofloat());
        if (IsKeyJustPressed(32)) {
            Velocity.y = -5.0;
        }
    }
}