dofile("../Assets/Node.lua")

Sprite = {
	This = Node 
}

function Start()
	print("Start")

	Sprite.This.Transform.Position.x = 10
	Sprite.This.Name = "gi"

	x = 5
end