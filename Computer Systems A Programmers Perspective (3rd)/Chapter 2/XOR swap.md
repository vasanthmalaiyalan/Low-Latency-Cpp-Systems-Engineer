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

-----------


சூப்பர் 🔥 இது தான் famous **XOR swap trick** — bookல சொல்லுறதை exact flowல, step-by-step புரியவைக்கிறேன்.

---

# 🧠 🔥 CORE IDEA

```text
a ^ a = 0
a ^ 0 = a
```

👉 இதை தான் முழு trick use பண்ணுது

---

# ⚙️ GIVEN

```c
*x = a
*y = b
```

👉 initially:

| Step    | *x | *y |
| ------- | -- | -- |
| Initial | a  | b  |

---

# 🔥 Step 1

```c
*y = *x ^ *y
```

👉 replace:

```text
*y = a ^ b
```

---

👉 table:

| Step  | *x | *y    |
| ----- | -- | ----- |
| Step1 | a  | a ^ b |

---

# 🔥 Step 2

```c
*x = *x ^ *y
```

👉 substitute:

```text
*x = a ^ (a ^ b)
```

---

👉 rearrange:

```text
= (a ^ a) ^ b
= 0 ^ b
= b
```

---

👉 table:

| Step  | *x | *y    |
| ----- | -- | ----- |
| Step2 | b  | a ^ b |

---

# 🔥 Step 3

```c
*y = *x ^ *y
```

👉 substitute:

```text
*y = b ^ (a ^ b)
```

---

👉 rearrange:

```text
= (b ^ b) ^ a
= 0 ^ a
= a
```

---

👉 final:

| Step  | *x | *y |
| ----- | -- | -- |
| Step3 | b  | a  |

---

# 💥 FINAL RESULT

```text
values swapped ✅
```

---

# 🧠 WHY WORKS?

👉 XOR properties:

```text
a ^ a = 0
a ^ b ^ a = b
```

---

# 🔥 VISUAL UNDERSTANDING

👉 Step-by-step flow:

```text
Initial:
x = a
y = b

Step1:
y = a ^ b

Step2:
x = b

Step3:
y = a
```

---

# 💣 IMPORTANT WARNING ⚠️

👉 இந்த trick:

```c
inplace_swap(&x, &x);
```

👉 same address இருந்தா:

```text
break ஆகும் ❌
```

---

👉 because:

```text
x ^ x = 0 → data loss
```

---

# ⚡ NORMAL SWAP (safe)

```c
temp = x;
x = y;
y = temp;
```

---

# 🔥 WHY THIS IMPORTANT?

👉 இது காட்டுது:

```text
Boolean algebra → real program trick
```

---

# 💥 FINAL TABLE (ANSWER)

```text
Initially:  x = a     y = b
Step1:      x = a     y = a ^ b
Step2:      x = b     y = a ^ b
Step3:      x = b     y = a
```

---

# 🧠 FINAL INSIGHT

```text
XOR allows "information storing without extra memory"
```

---

# 🔥 FINAL ONE-LINE

```text
"a ^ b ^ a = b → இதுவே XOR swap magic"
```

---

🔥 நீ இப்போ:

👉 Boolean algebra real use
👉 XOR power
👉 bit-level thinking

---

👉 next (🔥 killer topics):

* XOR encryption 😈
* bit masking tricks
* reversing bits

சொல்லு 😎
----------

சூப்பர் 🔥🔥 நீ கேட்ட இந்த doubt தான் **XOR algebra real புரிதல் point** — இங்க தான் எல்லாரும் சிக்குவாங்க.

நாம் calmஆ, step-by-step clear பண்ணலாம் 👇

---

# 🧠 🔥 PROBLEM

```text
a ^ (a ^ b)
→ எப்படி (a ^ a) ^ b ஆகுது?
```

👉 நீ கேள்வி:

```text
"ஏன் a, b உடன் சேரல? ஏன் a உடன் சேர்ந்தது?"
```

---

# 💣 KEY RULE (மிக முக்கியம்)

```text
XOR is associative + commutative
```

---

## 🔥 Meaning

### 🟢 Commutative:

```text
a ^ b = b ^ a
```

---

### 🔵 Associative:

```text
a ^ (b ^ c) = (a ^ b) ^ c
```

---

# ⚙️ APPLY THIS

```text
a ^ (a ^ b)
```

---

## Step 1 — remove brackets (associative)

```text
= a ^ a ^ b
```

👉 (இது legal because associative)

---

## Step 2 — reorder (commutative)

```text
= (a ^ a) ^ b
```

👉 இப்போ same values group பண்ணுறோம்

---

# 🔥 WHY group a with a?

👉 because:

```text
a ^ a = 0 (very powerful rule 🔥)
```

---

# ⚡ continue

```text
(a ^ a) ^ b
= 0 ^ b
= b
```

---

# 🧠 CORE INSIGHT

👉 XORல:

```text
order matter ஆகாது ❗
grouping matter ஆகாது ❗
```

---

👉 அதனால:

```text
a ^ b ^ a
→ rearrange → a ^ a ^ b
→ cancel → b
```

---

# 💥 VISUAL UNDERSTANDING

👉 imagine:

```text
a = 5
b = 3
```

---

## Step:

```text
5 ^ (5 ^ 3)
```

---

👉 inner:

```text
5 ^ 3 = 6
```

---

👉 then:

```text
5 ^ 6 = 3
```

---

👉 same result:

```text
= b
```

---

# 🔥 ANOTHER WAY (shortcut thinking)

```text
a ^ (a ^ b)
```

👉 think:

```text
"a twice வந்துருக்கு → cancel ஆகும்"
```

---

👉 left:

```text
b
```

---

# 💣 SUPER IMPORTANT RULE

```text
X ^ X = 0
X ^ 0 = X
```

---

👉 so:

```text
a ^ b ^ a
→ cancel a
→ b
```

---

# ⚡ FINAL SUMMARY

```text
a ^ (a ^ b)
= a ^ a ^ b
= 0 ^ b
= b
```

---

# 🔥 FINAL ONE-LINE

```text
"XORல same value இரண்டுமுறை வந்தா cancel ஆகும்"
```

---

🔥 நீ இப்போ புரிஞ்சுட்ட:

👉 associative + commutative power
👉 XOR cancellation trick
👉 why swap works

---

👉 next (🔥 insane level):

* XOR encryption (real hacking 😈)
* why XOR reversible
* bit-level puzzles

சொல்லு 😎
