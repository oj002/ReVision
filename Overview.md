# Control Flow
## *asm*
Inline assembly.
```CPP
asm
{
    // Assembly code
};
```

## *do*
1. Execute code-block.
2. If the condition is true jmp to 1.
3. Coninue execution.
```CPP
do
{

} while(cond);
```

## *switch*
Generates a jump table and inserts break's.
```CPP
switch(var)
{
case 1, 2, 3:
    // Some code...
case 4:
    // Some more code...
    @fallthrough // will not generate a break.
default:
    // Some more code...
}
```

## *while*
1. If the condition is false jmp to 4.
2. Execute code-block.
3. Jmp to 1.
4. Coninue execution.
```cpp
while (cond)
{
    // Some code...
}
```

## *if*
```cpp
if (cond1)
{
    // Some code...
}
else if(cond2)
{
    // Some more code...
}
else
{
    // Some more code...
}
```

## *for*
1. Execute init.
2. If condition is false jmp 6.
3. Execute code-block.
4. Execute iteration.
5. Jmp 2.
6. Coninue execution.
```cpp
for(init; cond; iteration)
{
    // Some code...
}
```

## *jmp*
```cpp
LABLE:
// Some code...
jmp LABLE;
```

## *jmpif*
Only jumps if the condition is true.
```cpp
LABLE:
// Some code...
jmpif (cond) LABLE;
```

# Scope


# Composite Types


# Primitive Types


# Abstract types


# Operators