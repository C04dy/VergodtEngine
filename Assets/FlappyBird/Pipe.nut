class Pipe extends Node {

    function Start() {
        Position = ::Vector2(Position.x, ::RandomRangeFloat(0.0, 720.0));
    }

    function Update(dt) {

    }
}