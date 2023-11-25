dofile("../Assets/VergodtEngine.lua")

f = 0

function NODEE_Start()
	print(This.Transform.Position.x)
end

function NODEE_Update()
	f = f + 1
	This.Transform.Position.x = f
end