class Spawner extends Node {
    timer = 0.0;

    function Start() {

    }

    function Update(dt) {
        timer += dt;
        if (timer >= 2.0) {
            ::
            timer = 0.0;
        }
    }
}