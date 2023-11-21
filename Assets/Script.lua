dofile("../Assets/Node.lua")

f = 0

function Start()
	
	This.Transform.Position.x = 10
end

function Update()
	f = f + 1
	This.Transform.Position.x = f 
end