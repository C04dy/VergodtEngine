Vector2 = {
	x = 0.0,
	y = 0.0
}

Transform = {
    Position = Vector2,
    Size = Vector2,
    Angle = 0.0
}

Node = {
	Transform = Transform,
	Name = "Node"
}

This = Node

function InitNode(Nd)
    This = Nd
end