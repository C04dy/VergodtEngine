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

local file_env = {}
local file_chunk = {}

function AddScript(ScriptLocation, index, Nd)
    file_env[index] = setmetatable({}, {__index = _G})
    file_chunk[index] = loadfile(ScriptLocation)

    _ENV = file_env[index]

    file_chunk[index]()

    file_env[index].InitNode(Nd)
    file_env[index].Start()
end

function UpdateScript(index)
    print(file_env[index])
    file_env[index].Update()
end

function InitNode(Nd)
    This = Nd
end