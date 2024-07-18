class bird extends Sprite {
    function Start() {

    }

    function Update(dt) {
        if (::IsMouseKeyJustPressed(1)) {
            Texture.ChangeTexture("../Assets/Test2.png");
        }
    }
}