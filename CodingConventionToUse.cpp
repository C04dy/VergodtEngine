// Coding conventions i use in this project is in this cpp file
// i think archiving this is a good idea because people can forgot stuff


// If statements

if (true)
{
    //code goes here
}
else
{
    //code goes here
}
else if (false)
{
    //code goes here
}

// Sometimes if statements can be used like this if the statement
// only use one line of code
if (true)
    //code goes here

// Switch statement

switch (1)
{
case 1:
    //code goes here
    break;
// if all values is not handled by statement add this for no warnings
default:
    break;
}

// For loop
// the size of a vector can be casted to a int with static_cast<int>
// asuming that the engine will never have an object array that 
// have 99.999.999 objects
for (int i = 0; i < 10; i++)
{
    //code goes here
}

// while loop

while (true)
{
    //code goes here
}

do
{
    //code goes here
} while (true);

// Functions

void Func(int ValueNameWillBeLikeThis/*If the type and the value name is the same put "_" at the start of the value name*/)
{
    // Variables declared inside functions
    int variable_name;

    //code goes here
}

// These two will be defined in header files if the function have more than 1 line do not define on the header
// Getter functions
bool GetterFunc() { return true; }

// Setter functions
void SetterFunc(int Value) { /*m_value = Value;*/ }

// Class Variables

// Public
int VariableName;
// Private
int m_VariableName;

// Pointers and references because some people write different

int* p;

int& r;

// Enums should be a enum class for better typing and readability

enum class EnumName
{
    NONE = 0,
    VALUE = 1
};

// If you read some of my code you can see i do not
// explicitly say Node::NodeType::NODE with types of objects
// i rather say Node::Type::NODE Because calling it Type rather than
// NodeType makes the code so much readable because if you use that 
// namespace you know that you are using that namespace for nodes
// not something other than nodes so i prefer writing this way 
// because it is cleaner in my opinion