dofile("../Assets/VergodtEngine.lua")

This = Node

f = 0

function NODE_Start()
	print(This.Transform.Position.x)
end

function NODE_Update()
	f = f + 1
	This.Transform.Position.x = f
end