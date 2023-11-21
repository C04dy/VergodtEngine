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

function SetNode(Nd)
    Nd.Transform.Position.x = This.Transform.Position.x
    Nd.Transform.Position.y = This.Transform.Position.y

    Nd.Transform.Size.x = This.Transform.Size.x
    Nd.Transform.Size.y = This.Transform.Size.y

    Nd.Transform.Angle = This.Transform.Angle

    Nd.Name = This.Name
end

function InitNode(Nd)
    This.Transform.Position.x = Nd.Transform.Position.x
    This.Transform.Position.y = Nd.Transform.Position.y

    This.Transform.Size.x = Nd.Transform.Size.x
    This.Transform.Size.y = Nd.Transform.Size.y

    This.Transform.Angle = Nd.Transform.Angle

    This.Name = Nd.Name
end