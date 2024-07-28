Function declaration and definition. Declaration may skip argument names. A function taking no input should have argument `(void)`.

Functions by default return `int`. 

Function must be declared before use. If not declared, first use becomes declaration: `sum += atof(line)`. Nothing is assumed about argument of such a function.


Command
```bash
cc main.c getline.c strindex.c
```
compiled three files and places object codes in `main.o, getline.o, strindex.o`. Then loads them all into an executable called `a.out`.

To recompile only `main.c`, and load:
```bash
cc main.o getline.o strindex.o
```

### External variables

A C program consists of a set of external objects, which are either variables or functions. *Internal* describes the arguments and variables defined inside functions. External variables are defined outside functions. Functions are always external.

*External linkage* All references to external functions and variables by the same name, even from functions compiled separately, are references to the same thing.

### Scope

Scope of an external variable or a function lasts from the point it is declared to the end of the file. 

If an external variable has to be referred before it is defined, an `extern` declaration is required.

Declaration of an external variable announces the type of variable. Definition also sets aside storage.

```c
double val[MAXVAL]; // definition
extern double val[]; // declaration - array size optional in declaration
```

There must be only 1 definition among all files in the program.

```c
// file 1
extern int sp;
extern double val[];

void push(double f) { ... }

// file 2
int sp = 0;
double val[MAXVAL];
```

### Header files

```c
// calc.h
void push(double);
double pop(void);

// main.c
#include "calc.h"
main() { ... }

// stack.c
#include "calc.h"
void push(double) { ... }
double pop(void) { ... }
```

### Static variables

`static`, applied to an external variable or function, limits the scope of that object to the rest of the source file being compiled.

Normally function names are global, visible to any part of the entire program.

If a declaration contains the extern storage class specifier, or is the declaration of a function with no storage class specifier (or both), then:

1. If there is already a visible declaration of that identifier with file scope, the resulting linkage is the same as that of the visible declaration;
2. otherwise the result is external linkage.

If a function is declared static, however, its name is invisible outside of the file in which it is declared.

static declaration can be applied to internal variables. Such variables are local to a particular function. They remain in existence rather than coming and going each time the function is activated. A static variable is initialized only the first time the block is entered.

### Register variables

`register` flags compiler to store variable on register. Compiler may ignore. Not possible to take the address of a register variable.

### Initializations

In the absence of explicit initialization, external and static variables are guaranteed to be initialized to zero; automatic and register variables have undefined (i.e., garbage) initial values.

Scalar variables may be initialized when they are defined.
For external and static variables, the initializer must be a constant expression; the initialization is done once, conceptually before the program begins execution.

For array:
```c
// Compiler computes size
int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
```

If there are fewer initializers for an array than the number specified, the missing elements will be zero for external, static, and automatic variables. Error to have too many initializers.

Character arrays can be initialized with strings: `char pat[] = "ould"` same as `char pat[] = {'o', 'u', 'l', 'd', '\0'}`.

### Preprocessor

First step on compilation.

#### File inclusion

Replaced by contents of the file.
```c
#include "filename" // search begins where source program was found
#include <filename>
```

```bash
# To find where gcc is looking for headers
`gcc -print-prog-name=cpp` -v

# gcc also looks in directories specified after -I
```

#### Macro substitution

```c
#define name repalcement text
// () useful, careful on call with side effects max(i++, j++)
#define max(A, B) ((A) > (B) ? (A) : (B)) 

#undef name
```

```c
#define  dprint(expr)  printf(#expr " = %g\n", expr)
dprint(x/y); // expands to
printf("x/y" " = %g\n", x/y);

#define  paste(front, back)  front ## back
paste(name, 1) // creates name1
```

#### Conditional inclusion

`#if` evaluates a constant integer expression (which may not include `sizeof`, casts, or enum constants). If the expression is non-zero, subsequent lines until an `#endif` or `#elif`` or `#else` are included. The expression defined(name) in a #if is 1 if the name has been defined, and 0 otherwise.

```c
//hdr.h

#if !defined(HDR)
#define HDR

/* contents of hdr.h go here */

#endif
```

```c
// use #ifdef, #ifndef
#ifndef HDR
#define HDR

/* contents of hdr.h go here */

#endif
```