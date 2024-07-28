`char`: single byte
`float`: single precision
`double`: double precision

Suffixes: `l` or `L` for long, `u` or `U` for unsigned, `ul` or `UL` for unsigned long, `f` or `F` for float.

Floating point constants are double by default.

Prefixes: `0` for octal, `0x` or `0X` for hex.

Character constant's value is the numeric value of the character in the machine's character set.

*constant expression*: `#define VAL 100` (not semicolon)

String constant / literal: `"A B"` surrounded by double quotes
`"hello," "world"` same as `"hello, world"`. Array of characters, has `\0` at the end.

enum: `enum boolean {NO, YES}`. Values 0, 1 etc unless specified. Values need not be distinct in the same enum.

All variables must be declared before use. A declaration specifies a type, and contains a list of variables of that type.

External and static variables are initialized to 0 be default.

`const` specifies value will not be changed.
`const char msg[] = "warning"`. Can be used with array to indicate function does not change the array `strlen(const char[])`

Value of a relational or logical expression is 1 if true, 0 if false.

`(type) var` for cast.

The order in which operands of an operators are evaluated is not specified (except for `&&`, `||`, `?:`, `,`). `f() + g()` `f` may be evaluated before or after `g`.

The order in which function arguments are evaluated is not specified.

| Operator                  | Associativity |
| ------------------        |---------------|
| () [] -> .                | left to right |
| ! ~ ++ -- unary+ unary- * & (type) sizeof   | right to left |
| * / %                     | left to right |
| + -                       | left to right |
| << >>                     | left to right |
| < <= > >=                 | left to right |
| == !=                     | left to right |
| &                         | left to right |
| ^                         | left to right |
| \|                        | left to right |
| &&                        | left to right |
| \|\|                      | left to right |
| ?:                        | right to left |
| = += -= *= /= %= &= \|= ^=  <<= >>=    | right to left |
| ,                         | left to right|