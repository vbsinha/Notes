A text stream consists of a sequence of lines; each line ends with a newline character. If the system doesn’t operate that way, the library does whatever is necessary to make it appear as if it does.

```c
#include<stdio.h>

int getchar(void) // returns EOF (-1) at the end of file
int putchar(int) // returns char written or EOF if error
```

`putchar` is often a macro in `<stdio.h>`.

In include if < > are included search is made at a standard set of places.
```bash
prog < infile > outfile # infile is not in argv
```

```c
int printf(char *format, ...) // returns no of char printed
// %d int
// %ld long int
// %u unsigned
// %f double
// %s char *

int sprintf(char *string, char *format, ...)
// same as printf but stores the formatted string into char *string
```

```c
int scanf(char *format, ...) // All arguments must be pointers
int sscanf(char *string, char *format, ...) // Scan char *string
```

### Storage management

```c
// n bytes of uninitialized storage, returns NULL if fails
void *malloc(size_t n)

// enough space for n objects of size
void *calloc(size_t n, size_t size)
```

`free(p)` p MUST be the original value obtained from malloc / calloc
OS will remember how much memory starting from p was allocated to in call to malloc / calloc so will know how much to free
[See](https://stackoverflow.com/questions/79923/what-and-where-are-the-stack-and-heap/13326916#13326916)

Heap is often maintained as linked list of free available space. Malloc traverses the list, picks a big enough node, allocates, fragments and returns address. May ask OS for more space is not enough is found.


```c++
// Statically allocated in the data segment when the program/DLL is first loaded
// Deallocated when the program/DLL exits
// scope - can be accessed from anywhere in the code
int someGlobalVariable;

// Statically allocated in the data segment when the program is first loaded
// Deallocated when the program/DLL exits
// scope - can be accessed from anywhere in this particular code file
static int someStaticVariable;

// "someArgument" is allocated on the stack each time MyFunction is called
// "someArgument" is deallocated when MyFunction returns
// scope - can be accessed only within MyFunction()
void MyFunction(int someArgument) {

    // Statically allocated in the data segment when the program is first loaded
    // Deallocated when the program/DLL exits
    // scope - can be accessed only within MyFunction()
    static int someLocalStaticVariable;

    // Allocated on the stack each time MyFunction is called
    // Deallocated when MyFunction returns
    // scope - can be accessed only within MyFunction()
    int someLocalVariable;

    // A *pointer* is allocated on the stack each time MyFunction is called
    // This pointer is deallocated when MyFunction returns
    // scope - the pointer can be accessed only within MyFunction()
    int* someDynamicVariable;

    // This line causes space for an integer to be allocated in the heap
    // when this line is executed. Note this is not at the beginning of
    // the call to MyFunction(), like the automatic variables
    // scope - only code within MyFunction() can access this space
    // *through this particular variable*.
    // However, if you pass the address somewhere else, that code
    // can access it too
    someDynamicVariable = new int;


    // This line deallocates the space for the integer in the heap.
    // If we did not write it, the memory would be "leaked".
    // Note a fundamental difference between the stack and heap
    // the heap must be managed. The stack is managed for us.
    delete someDynamicVariable;

    // In other cases, instead of deallocating this heap space you
    // might store the address somewhere more permanent to use later.
    // Some languages even take care of deallocation for you... but
    // always it needs to be taken care of at runtime by some mechanism.

    // When the function returns, someArgument, someLocalVariable
    // and the pointer someDynamicVariable are deallocated.
    // The space pointed to by someDynamicVariable was already
    // deallocated prior to returning.
    return;
}

// Note that someGlobalVariable, someStaticVariable and
// someLocalStaticVariable continue to exist, and are not
// deallocated until the program exits.
```