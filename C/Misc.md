### Segmentation fault

Occurs due to accessing memory that does not belong to you.
1. Dereferencing null pointer
```c
int *p = NULL;
*p = 1;
```
2. Dereferencing or assigning to an uninitialized pointer
3. Dereferencing or assigning to an freed pointer
```c
char *p = NULL;
{
    char c;
    p = &c;
}
// p is dangling
```
4. Buffer overflow
```c
char c[] = "Hello";
char s = c[20];
```
5. Stack overflow: Compiler could optimize it away.
```c
int main() {
    return main();
}
```
6. Writing to read only memory
```c
char *s = "Hello";
// "Hello" is in read only memory (data segment)
*s = 'h';
// Seg Fault

// Use
const char *t = "Jello";
```

### [const](https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const)

1. `int *` pointer to int
2. `int const *` pointer to const int
3. `int * const` const pointer to int
4. `int const * const` const pointer to const int

`const` can be on either side of type

`const int * == int const *`
`const int * const  == int const * const` 

```c
int a = 5, b = 10, c = 15;

const int* foo;     // pointer to constant int.
foo = &a;           // assignment to where foo points to.

/* dummy statement*/
*foo = 6;           // the value of a can´t get changed through the pointer.

foo = &b;           // the pointer foo can be changed.



int *const bar = &c;  // constant pointer to int 
                      // note, you actually need to set the pointer 
                      // here because you can't change it later ;)

*bar = 16;            // the value of c can be changed through the pointer.    

/* dummy statement*/
bar = &a;             // not possible because bar is a constant pointer.           
```

Split the statement at `*`. If `const` appears in the left part (like in `const int * foo`) - it belongs to pointed data, if it's in the right part (`int * const bar`) - it's about the pointer.