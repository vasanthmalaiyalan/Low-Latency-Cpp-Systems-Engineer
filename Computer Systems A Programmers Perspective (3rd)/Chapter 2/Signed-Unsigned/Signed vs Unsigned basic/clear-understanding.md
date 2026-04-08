Great! Let me show you how to **reverse** two's complement - going from **decimal -12345 back to its binary representation**.

## Method 1: Direct Two's Complement Conversion

### Step 1: Find absolute value
```
|-12345| = 12345
```

### Step 2: Convert 12345 to binary
```
12345 in binary (16 bits):

12345 ÷ 2 = 6172 remainder 1
6172 ÷ 2 = 3086 remainder 0
3086 ÷ 2 = 1543 remainder 0
1543 ÷ 2 = 771 remainder 1
771 ÷ 2 = 385 remainder 1
385 ÷ 2 = 192 remainder 1
192 ÷ 2 = 96 remainder 0
96 ÷ 2 = 48 remainder 0
48 ÷ 2 = 24 remainder 0
24 ÷ 2 = 12 remainder 0
12 ÷ 2 = 6 remainder 0
6 ÷ 2 = 3 remainder 0
3 ÷ 2 = 1 remainder 1
1 ÷ 2 = 0 remainder 1

Reading remainders bottom to top:
12345 = 0011 0000 0011 1001₂
```

### Step 3: Apply two's complement (invert bits + add 1)
```
Original: 0011 0000 0011 1001
Invert:   1100 1111 1100 0110
Add 1:    1100 1111 1100 0111 ✓
```

## Method 2: Using the Formula `2^n - |negative|`

For 16-bit numbers (n=16):
```
-12345 in two's complement = 2^16 - 12345
= 65536 - 12345
= 53191
```

Now convert 53191 to binary:
```
53191 in binary:
53191 = 32768 + 20423
32768 = 2^15 (bit 15 = 1)
20423 = 16384 + 4039
16384 = 2^14 (bit 14 = 1)
4039 = 2048 + 1991
2048 = 2^11 (bit 11 = 1)
1991 = 1024 + 967
1024 = 2^10 (bit 10 = 1)
967 = 512 + 455
512 = 2^9 (bit 9 = 1)
455 = 256 + 199
256 = 2^8 (bit 8 = 1)
199 = 128 + 71
128 = 2^7 (bit 7 = 1)
71 = 64 + 7
64 = 2^6 (bit 6 = 1)
7 = 4 + 2 + 1
4 = 2^2 (bit 2 = 1)
2 = 2^1 (bit 1 = 1)
1 = 2^0 (bit 0 = 1)

Bits set: 15,14,11,10,9,8,7,6,2,1,0
Result: 1100 1111 1100 0111 ✓
```

## Quick Formula Reference

For n-bit two's complement of negative number **-x**:

```
Two's complement representation = 2^n - x

Example with -12345 (n=16):
= 65536 - 12345
= 53191
= 0xCFC7
= 1100 1111 1100 0111₂

Same as unsigned like,
if unsigned Example with 53191 (n=16):
= 65536 - 53191
= 12345
= 1100 1111 1100 0111₂
```

## Visual Summary

```
-12345 (decimal)
    ↓
Step 1: 12345 = 0011 0000 0011 1001₂
    ↓
Step 2: Invert = 1100 1111 1100 0110₂
    ↓
Step 3: Add 1  = 1100 1111 1100 0111₂
    ↓
-12345 in two's complement = 1100 1111 1100 0111₂
```

So **-12345 in two's complement** = `1100 1111 1100 0111` (which matches our original binary!) ✓

--