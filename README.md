# Revision
## License
This project is licensed under the terms of the [MIT](LICENSE.md) license.



## Traits
### Paradigm's
* Mostly Imperative
    >Very intuitive and productive way of writing code.
* OOP without normal inheritance
    >Similar to C++ but all private variables will be const &.
* functional programming (optional functional purity)
    >Ensure cleaner/saver code. Make the Programmer feel better about the code.
* inline assembly
    >Grant low-level control.


### Fast runtime.
First compiling to c and taking advantage of the gcc's optimizer.
Later compiling to llvm with custom optimizations.


### Low-level support.


### Decent compilation speed. The language shouldn't be too complex to parse and not context sensitive.
>If its hard to parse it's often hard to read and write. A left-to-right readable approach is desired.


### Zero overhead abstractions.
* pretty sure:
    * A macro preprocessor (hopefully better thant in C/C++)
    * Const correctness
    * optional functional purity
    * Con-/Destructor instead of manual init or destruct member functions. Should be callable (RAII).
    * Use statement -> reduces the scoop of the next code block to a capture list. Similar to the effect of an immediately invoked lambda.
    * No exceptions, but panic semantic to safely handel crashes or save exits.

* Not quite sure jet:
    * Compile-time-contracts
    * interfaces/concepts checked at compile time

### Runtime abstractions/checks, that are conditional compile-time.
* optional runtime-contracts


### No cmake style build dependencies, java style packages.



## Syntax-Sketching
### Comments
```
// single line comments
/* 
    multi line comments
*/
/*
    /*  nested multi line comments */
    This is still commented
*/
```


### Statements/Code-blocks
1. C: 
    ```c
    if(cond) { true_statement_block } else { false_statement_block } // space/newline independent
    if(cond) true_statement; else false_statement; // space/newline independent
    ```
2. Lisp: ```(if (cond) (true_statement_block) (false_statement_block))```
3. Python: indenting + specific keywords 
    ```python
    if cond:
        true_statement_block
    else:
        false_statement_block # space/newline dependent
    ```
4. other:
    ```cpp
    cond ?
        true_statement_block
    |
        false_statement_block // space/newline dependent
    ```


### Entry Point
1. ```label:```
2. Start from the top of the file (from top to bottom).
3. Use a standard function e.g.: c-style ```int main() { }``` or ```int main(int argc, char **argv) { }``` 
4. Cyclic dependency graph.

Number 3 will be used, with maybe a different syntax (maybe the instead of main the enter keyword, but for easy use we'll go with main for now).


### Function Declaration
* Free functions
* Function local to another function

1. C: ```return_type name ( arguments ) { code }```
2. Java: ```public class A { public static return_type name ( arguments ) throws exception_list { code } }```
3. Haskell: ```name :: type -> type \n name arguments = code```
4. Lisp: ```(defun name (arguments) "Optional documentation string." (code)```
5. Matlab: ```function [ return_names ] = name (arguments) code end```
6. Python: ```def name ( arguments ) \n\t code \n```
7. Javascript: ```function name ( arguments ) { code }```

The desired style must feel safe to use and be intuitive.
See Variable/Types Declaration for the choice of type.\

We don't want multiple return values because a struct can be used, or maybe at a later day a anonymous struct. We'll go for the C-style number one.

Special cases:
```csharp
 void name (arguments)
 {
     code
 }
 int name ()
 {
     code
 }
 var name(arguments) // may be allowed
 {
     code
 }
```

Nested functions:
```csharp
int foo(int x, y)
{
    // It should also be allowed to forward declare
    void baar(int &x) { x += 200; } // Not a lambda because the behavior cant be changed
   
    baar(x);
    return baar(x + y);
}
```

Lambdas may use the C++-style (Still not decided):
```[ capture_list ] ( arguments ) { code }```

Function Pointers should work like this:
```csharp
return_type name ( arguments ) = // The '=' indicates the function is a variable (Function Pointers)
{
    code
}
```


### Variable/Types Declaration
1. C: ```const int * const var;```
2. C (east const): ```int const * const var;```
3. ```var is const * const int;```
4. ```var: const * const int;```
5. ```const * const int var;```

3 and 5 seem like very good approach, but well go with 5 because it will make the Function Declaration's easier.


### Classes
```cpp
// can have functions and variables
// is generally a dum class, doesn't do much to the data and it's purpose is to hold data
struct name
{
    // only public
};
// can have functions and variables
class name
{    
    // Default private
private: // outside of the class nobody can access the functions and the variables can't be changed but read in an constant context
    // private
public: // the variables and functions of the class can be read and changed from outside the class
    // public
};
```