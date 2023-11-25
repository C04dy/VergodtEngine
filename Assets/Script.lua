dofile("../Assets/VergodtEngine.lua")

f = 0

function NODEStart()
	print("hi")
end

function NODEUpdate()
	f = f + 1
	This.Transform.Position.x = f 
	print(This.Transform.Position.x)
end