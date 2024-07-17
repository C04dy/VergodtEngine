class bird extends Node {
    function Start() {

    }

    function Update(dt) {
        if (Input.IsMouseKeyJustPressed(1)) {
            local test = ::Vector2(500.0, 400.0);
            test.x += 500.0;
            ::print(test.x);
            Position.AddToVec2(::Vector2(500.0, 400.0));
            ::print(Position.x.tostring());
        }
    }
}