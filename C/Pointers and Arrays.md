`int *ip`: `*ip` is an `int`.
`double *fun(char *c)`: `*fun(char *c)` is a `double`.

#### Arrays

```c
int a[10];
int *pa = &a[0];
*(pa+i) // same as a[i]
```

The value of a variable or expression of type array is the address of zeroth element of array. `pa = &a[0]` is same as `pa = a`. In evaluating, C immediately converts `a[i]` to `*(a+i)`. `&a[i]` is same as `(a+i)`.

A pointer is a variable, so `pa=a` and `pa++` are legal.
Array name is not a variable, `a=pa` and `a++` are illegal.

When an array name is passed to a function, what is passed is the location of the initial element. 
As formal parameters in a function definition, `char s[]` and `char *s` are equivalent, `char *s` is preferred.

If elements are known to exist, `p[-1]` etc. is legal (where `p` was assigned `&a[1]`)

In general a pointer can be initialized just as any other variable can, though normally the only meaningful values are zero or an expression involving the addresses of previously defined data of appropriate type.

C guarantees that zero is never a valid address for data. `NULL` is used in place of 0. 

Pointers may be compared under certain circumstances. If p and q point to members of the same array, then relations like ==, !=, <, >=, etc., work properly. For example,

p < q

is true if p points to an earlier member of the array than q does. Any pointer can be meaningfully compared for equality or inequality with zero. But the behavior is undefined for arithmetic or comparisons with pointers that do not point to members of the same array. (There is one exception: the address of the first element past the end of an array can be used in pointer arithmetic.)

`size_t` is the unsigned integer type returned by the `sizeof` operator.

The valid pointer operations are assignment of pointers of the same type, adding or subtracting a pointer and an integer, subtracting or comparing two pointers to members of the same array, and assigning or comparing to zero. All other pointer arithmetic is illegal. It is not legal to add two pointers, or to multiply or divide or shift or mask them, or to add float or double to them, or even, except for void *, to assign a pointer of one type to a pointer of another type without a cast.

### Character Pointers and Functions

```C
char amessage[] = "now";
char *pmessage = "now";
```

`amessage` is an array, and will always refer to the same storage.
`pmessage` is a pointer and can be modified to point elsewhere.

`*s++` is `*(s++)` since unary ops are evaluated right to left.

```c
void strcpy(char *s, char *t) { while (*s++ = *t++); }
```

### Pointer Arrays; Pointers to Pointers

```c
char *lines[500]; // array of pointers
```

### Multidimensional arrays

```c
char days[2][13] = {
    {0, 31, 28, 31, ...},
    {0, 31, 29, 32, ...}
}
```

To pass a 2d array to function, include the number of columns in parameter declaration. This is required for the compiler to compute the offset for `array[i][j]`.

```c
// function declaration to accept 2d array
f(int a[2][13]) { ... }
f(int a[][13]) { ... }
f(int (*a)[13]) { ... }
```

`int (*a)[13]` pointer to an array of 13 ints.
`int *a[13]` array of 13 pointers to int.

```c
static char *name[] = {
        "Illegal month",
        "January", "February", "March",
        "April", "May", "June",
        "July", "August", "September",
        "October", "November", "December"
    };
name[0] // valid
name[1] // valid
```

### Command line arguments

`main` takes `argc` (argument count) and `argv` (argument values). First value of `argv` is the program name so the `argc` is at least 1. `argv[argc]` is always a null pointer.

```c
int main(int argc, char *argv[])
```

### Function pointers

```c
void qsort(void *lineptr[], int left, int right,
           int (*comp)(void *, void *));

// call
qsort((void **) lineptr, 0, nlines−1,
          (int (*)(void*,void*))(numeric ? numcmp : strcmp));

int (*comp)(void *, void *) // ptr to a fn that has two void* arguments
(*comp)(v[i], v[left]) // Call comp
int *comp(void *, void *) // fn returning ptr to int
```

### Examples

```c
char **argv
//    argv:  pointer to pointer to char
int (*daytab)[13]
//    daytab:  pointer to array[13] of int
int *daytab[13]
//    daytab:  array[13] of pointer to int
void *comp()
//    comp:  function returning pointer to void
void (*comp)()
//    comp:  pointer to function returning void
char (*(*x())[])()
//    x: function returning pointer to array[] of
//    pointer to function returning char
char (*(*x[3])())[5]
//    x: array[3] of pointer to function returning
//    pointer to array[5] of char
```