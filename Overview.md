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
2. If the *cond*ition is true jmp to 1.
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

## *if*

## *for*

## *jmp*

## *jmpif*


# Scope


# Composite Types


# Primitive Types


# Abstract types


# Operators