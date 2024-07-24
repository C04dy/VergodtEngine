class Spawner extends Node {
    timer = 0.0;

    function Start() {

    }

    function Update(dt) {
        timer += dt;
        if (timer >= 2) {
            ::InstantiateNodesFromFile("../Assets/FlappyBird/Pipes.vscene");
            timer = 0.0;
        }
    }
}