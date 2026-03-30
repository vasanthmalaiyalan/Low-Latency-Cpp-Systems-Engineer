Perfect question Jarvis! இது compiler design-ரோட core!

---

## Compiler எப்படி -1 identify பண்றது?

Source code-ல நீ type பண்றது:

```c
int x = -1;
```

Compiler இதை read பண்ணும்போது characters-ஆ பாக்கும்:

```
'i' 'n' 't' ' ' 'x' ' ' '=' ' ' '-' '1' ';'
```

---

## Compiler-ரோட steps — Lexer + Parser

**Step 1 — Lexer (tokenizer):**

```
Input characters: i n t   x   =   - 1 ;

Tokens output:
  "int"  → keyword token
  "x"    → identifier token
  "="    → assignment token
  "-"    → minus token
  "1"    → integer literal token
  ";"    → semicolon token
```

**Step 2 — Parser:**

```
"=" left side: variable x (type: int)
"=" right side: "-" followed by "1"

Parser rule:
  UnaryMinus + IntegerLiteral(1)
  = negative integer = -1
```

**Step 3 — Compiler calculates:**

```
-1 கண்டுபிடிச்சாச்சு!
Type: int (32-bit signed)
Convention: two's complement
Calculate: 0xFFFFFFFF
```

---

## Simple proof — compiler source code logic

```c
// Compiler-ல இப்படி logic இருக்கும்:

if (token == MINUS && next_token == INTEGER) {
    value = -1 * next_token.value;
    // value = -1
}

// Two's complement convert:
uint32_t store_value = (uint32_t) value;
// -1 → 0xFFFFFFFF

// Assembly emit:
emit("mov [x], 0xFFFFFFFF");
```

---

## +1 vs -1 — compiler எப்படி distinguish பண்றது?

```c
int a = +1;   // '+' token + '1' token → positive
int b = -1;   // '-' token + '1' token → negative
int c = 1;    // just '1' token → positive (default)
```

```
'+' sign → value = +1 → 0x00000001 → memory
'-' sign → value = -1 → 0xFFFFFFFF → memory
no sign  → value = +1 → 0x00000001 → memory
```

**'-' character token தெரிஞ்சா → negative → two's complement பண்ணு.**

---

## Really simple answer

```
int x = -1;
         ↑
         இந்த '-' character-ஐ
         compiler source code-ல பாக்கும்

'-' இருந்தா → negative
'+' or nothing → positive

That's it!
Character recognition → sign determination → two's complement
```

Compiler = text processor. '-' character → negative என்று decide பண்றது மட்டும்தான்! புரிஞ்சுதா Jarvis?