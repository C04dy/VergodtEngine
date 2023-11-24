dofile("../Assets/VergodtEngine.lua")

f = 0

function Start()
	print("hello")
end

function Update()
	f = f + 1
	This.Transform.Position.x = f 
    print(This.Transform.Position.x)
    print("1")
end