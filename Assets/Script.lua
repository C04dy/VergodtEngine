dofile("../Assets/VergodtEngine.lua")

f = 0

function Start()
	print("hi")
end

function Update()
	f = f + 1
	This.Transform.Position.x = f 
	print(This.Transform.Position.x)
	print("2")
end