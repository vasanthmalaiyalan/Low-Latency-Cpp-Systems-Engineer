# Practice Problem 2.10 — Inplace Swap

## Key property used

```
a ^ a = 0     (any value XOR itself = 0)
a ^ 0 = a     (any value XOR 0 = itself)
a ^ b ^ b = a (XOR is its own inverse)
XOR is commutative and associative
```

## Trace — step by step

Initially: `*x = a`, `*y = b`

---

### Step 1: `*y = *x ^ *y`

```
*y = a ^ b

*x = a        (unchanged)
*y = a ^ b    (new value)
```

---

### Step 2: `*x = *x ^ *y`

```
*x = a ^ (a ^ b)
   = (a ^ a) ^ b    (associative)
   = 0 ^ b
   = b

*x = b        (now holds b!)
*y = a ^ b    (unchanged)
```

---

### Step 3: `*y = *x ^ *y`

```
*y = b ^ (a ^ b)
   = b ^ (b ^ a)    (commutative)
   = (b ^ b) ^ a    (associative)
   = 0 ^ a
   = a

*x = b        (unchanged)
*y = a        (now holds a!)
```

---

## Completed table

```
Step    *x          *y
──────────────────────────
Init    a           b
Step 1  a           a ^ b
Step 2  b           a ^ b
Step 3  b           a
```

Swap complete — `*x = b`, `*y = a`. No temporary variable needed!

---

## Why this works — intuition

```
XOR = "difference recorder"

Step 1: *y stores the DIFFERENCE between a and b
Step 2: *x = a XOR (difference) = b  (original b recovered)
Step 3: *y = b XOR (difference) = a  (original a recovered)
```

## Important note — book சொல்றது

```c
// WARNING: x == y ஆனா (same pointer) — broken!
inplace_swap(&a, &a);

Step 1: *y = a ^ a = 0   (both x and y point to same location!)
Step 2: *x = 0 ^ 0 = 0
Step 3: *y = 0 ^ 0 = 0
Result: value destroyed!

// Always ensure x != y before calling
```

Book: "There is no performance advantage to this way of swapping; it is merely an intellectual amusement." — Practical code-ல temp variable use பண்றதே better.