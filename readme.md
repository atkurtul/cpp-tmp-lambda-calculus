## Lambda Calculus with C++ TMP


True is a function which selects the 1st operand
- `T(x, y) = x` 
---
Similarly False selects the 2nd
- `F(x, y) = y`

---
Which allows us to implement the logical operators like so:

- `Not(x) = x(F, T) = ¬x`
- `Or(x, y) = x(x, y) = x ∨ y`
- `And(x, y) = x(y, x) = x ∧ y`
- `Xor(x, y) = x(Not(y), y) = x ⊻ y`

- `Or(x, ...) = Or(x, Or(...))`
- `And(x, ...) = And(x, And(...))`
- `Xor(x, ...) = Xor(x, Xor(...))`

---

Integer is defined as a list of little endian bits which are either T or F

And integer addition is defined as well.

Take a look on [godbolt](https://godbolt.org/z/s844EWq8q).
```cpp
                         // rev(000001) + rev(101001) 
                         // 0b100000    + 0b100101 = 69
int this_integer_is_69 = Add<Int<F,F,F,F,F,T>, Int<T,F,T,F,F,T>>{};
```