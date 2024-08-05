class Pipe extends Node {

    function Start() {
        Position = ::Vector2(Position.x, ::RandomRangeFloat(0.0, 720.0));

        ::print("hi");
        Childs.GetChild(0);
        ::print("bye");
    }

    function Update(dt) {

    }
}