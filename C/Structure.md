```c
struct point {
    int x;
    int y;
} pt1, p2;

struct point p3 = { 50, 100 };
p3.x

struct point *pp;
(*pp).x
pp->x

struct point pts[10];
```

Only legal operations on a structure:
1. copying it or assigning to it as a unit
2. Taking its address with `&`
3. Accessing its members

compile time op: `sizeof`

There may be holes when storing structs. Struct with int and char may take 8 bytes.

`(struct node *) malloc (sizeof(struct node))`

### Typedef

```c
typedef int Length;
typedef char *String;
typedef struct tnode {   /* the tree node: */
    char *word;              /* points to the text */
    int count;               /* number of occurrences */
    Treeptr left;            /* left child */
    Treeptr right;           /* right child */
} Treenode;
typedef int (*PFI)(char *, char *);
```

Uses:
1. Use typedef for machine dependent types. Only typedef's type needs to change when machine changes. `size_t`, `ptrdiff_t` etc.
2. Easier to read

### Union

```c
union u_tag {
    int ival;
    float fval;
    char *sval;
} u;
```

It is the programmer’s responsibility to keep track of which type is currently stored in a union; the results are implementation-dependent if something is stored as one type and extracted as another.

A union may only be initialized with a value of the type of its first member.

### Bit fields

```c
// use 3 bits
struct {
    unsigned int is_keyword : 1;
    unsigned int is_extern  : 1;
    unsigned int is_static  : 1;
} flags;
flags.is_static
```

Fields can only be unsigned or signed int.