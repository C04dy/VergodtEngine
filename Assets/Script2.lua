dofile("../Assets/VergodtEngine.lua")

f = 0

function NODEEStart()
	print("hello")
end

function NODEEUpdate()
	f = f + 1
	This.Transform.Position.x = f 
    print(This.Transform.Position.x)
end