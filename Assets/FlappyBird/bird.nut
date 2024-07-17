class bird extends Node {
    function Start() {
        //Body.SetFixedRotation(true);
    }

    function Update(dt) {
        if (IsMouseKeyJustReleased(1)) {
            Velocity.y = -5.0;
        }
    }
}