
# 🖥️ Computer System என்றால் என்ன?

ஒரு **computer system** இரண்டு முக்கிய பகுதிகள் கொண்டு இருக்கும்:

1️⃣ **Hardware**  
2️⃣ **System Software**

இரண்டும் சேர்ந்து:

👉 **Application programs run செய்யும்**

----------

## 🧱 Hardware examples

-   CPU
    
-   Memory (RAM)
    
-   Disk
    
-   Network card
    

----------

## 💻 System Software examples

-   Operating system
    
-   Compiler
    
-   Linker
    
-   Shell
    

----------

# ⚙️ Systems change but concepts same

Technology காலத்துடன் மாறும்:

-   CPU model மாறும்
    
-   OS version மாறும்
    
-   hardware design மாறும்
    

ஆனால்:

👉 **basic concepts மாறாது**

எல்லா computers-லும்:

-   CPU
    
-   Memory
    
-   Storage
    
-   OS
    

இவை இருக்கும்.

----------

# 👨‍💻 இந்த புத்தகம் யாருக்காக?

இந்த புத்தகம்:

👉 **programmers**

காக எழுதப்பட்டது.

Goal:

Programmers புரிந்து கொள்ள வேண்டும்:

-   computer system எப்படி வேலை செய்கிறது
    
-   அது program performance-ஐ எப்படி பாதிக்கும்
    

----------

# 🚀 Power Programmer என்றால் என்ன?

இந்த புத்தகம் படித்தால்:

நீங்கள் ஒரு **Power Programmer** ஆகலாம்.

Power Programmer என்றால்:

-   computer system உள்ளே என்ன நடக்கிறது தெரியும்
    
-   bugs easy-ஆ fix செய்ய முடியும்
    
-   performance improve செய்ய முடியும்
    

----------

# 📚 இந்த புத்தகம் கற்றுக்கொடுக்கும் skills

இந்த புத்தகம் சில **practical skills** கற்றுக்கொடுக்கும்.

----------

## 1️⃣ Strange numerical errors

Computer numbers represent செய்வது different.

Example:

floating point numbers.

இதனால்:

👉 unexpected errors வரும்.

இந்த புத்தகம்:

✔ இந்த errors avoid செய்ய கற்றுக்கொடுக்கும்.

----------

## 2️⃣ C code optimization

Modern processors:

-   pipelines
    
-   cache
    
-   branch prediction
    

இவை பயன்படுத்தி:

👉 **C code fast ஆக எழுதுவது**

கற்றுக்கொடுக்கப்படும்.

----------

## 3️⃣ Procedure calls

Compiler எப்படி:

👉 **function calls implement செய்கிறது**

அதை explain செய்கிறது.

Example:

-   stack usage
    
-   registers
    

----------

## 4️⃣ Security problems

Example:

**Buffer overflow**

இது:

👉 network software-ல் common vulnerability.

இந்த புத்தகம்:

✔ அதை புரிந்து கொள்ள  
✔ அதை avoid செய்ய

கற்றுக்கொடுக்கும்.

----------

## 5️⃣ Linking errors

Program compile ஆன பிறகு:

👉 **linking stage**

இங்கு சில errors வரும்.

Average programmers:

❌ confuse ஆகிவிடுவார்கள்.

இந்த புத்தகம்:

✔ linking எப்படி வேலை செய்கிறது explain செய்கிறது.

----------

## 6️⃣ Real systems programs எழுத கற்றுக்கொடுக்கும்

இந்த புத்தகத்தில் நீங்கள் எழுத கற்றுக்கொள்வீர்கள்:

-   Unix shell
    
-   dynamic memory allocator
    
-   web server
    

அதாவது:

👉 real systems software.

----------

## 7️⃣ Concurrency

Modern CPUs:

👉 **multi-core processors**

இதனால் programs:

👉 parallel run ஆகும்.

இந்த topic:

✔ advantages  
✔ problems

இரண்டையும் explain செய்யும்.

----------

# 👋 Hello Program example

Authors ஒரு famous example பயன்படுத்துகிறார்கள்.

Hello program.

#include <stdio.h>  
  
int  main()  
{  
  printf("hello, world\n");  
  return  0;  
}

இந்த program:

👉 screen-ல் print செய்யும்

hello, world

----------

# 🤔 இந்த simple program run ஆக என்ன வேண்டும்?

இந்த simple program run ஆக:

Computer system முழுவதும் work செய்ய வேண்டும்.

Example:

1️⃣ compiler  
2️⃣ OS  
3️⃣ CPU  
4️⃣ memory  
5️⃣ terminal

இவை அனைத்தும் சேர்ந்து வேலை செய்ய வேண்டும்.

----------

# 🎯 இந்த புத்தகத்தின் goal

Authors சொல்வது:

👉 Hello program run ஆகும்போது

**computer system-ல் என்ன நடக்கிறது**

அதை முழுவதும் புரிய வைப்பது தான் இந்த புத்தகத்தின் goal.

----------

# 🔄 Hello program life cycle

இந்த chapter-ல் authors explain செய்வது:

Hello program வாழ்க்கை.

1️⃣ programmer code எழுதுகிறார்  
2️⃣ compiler compile செய்கிறது  
3️⃣ OS program load செய்கிறது  
4️⃣ CPU instructions execute செய்கிறது  
5️⃣ output print ஆகிறது

----------

# 💾 Code computer-ல் எப்படி store ஆகும்?

Computer:

👉 text புரியாது.

அது புரிவது:

**bits**

அதாவது:

0  
1

----------

# 🔤 ASCII representation

Hello program text computer-ல்

👉 ASCII numbers ஆக store ஆகும்.

Example:

h = 104  
e = 101  
l = 108  
o = 111

அதனால்:

text → numbers.

Example:

hello

store ஆகும்:

104 101 108 108 111

----------

# 📄 hello.c file example

File:

code/intro/hello.c

இந்த file computer memory-ல்

ASCII numbers ஆக இருக்கும்.

Example:

#include <stdio.h>

convert ஆகும் numbers:

35 105 110 99 ...

----------

# 🧠 முக்கிய concept

இந்த section main idea:

👉 **Information = Bits + Context**

அதாவது:

Computer-க்கு எல்லாம்:

0  
1

ஆனால்:

Context இருந்தால் தான் meaning தெரியும்.

Example:

01000001

இது:

ASCII context-ல்

👉 'A'

ஆகும்.

----------

# ⭐ Chapter 1 என்ன செய்யும்?

இந்த chapter:

Hello program life cycle explain செய்யும்.

Topics:

1️⃣ Source code  
2️⃣ Compilation  
3️⃣ Linking  
4️⃣ Loading  
5️⃣ Execution  
6️⃣ Output

Later chapters:

இந்த concepts-ஐ **detail-ஆ explain செய்யும்**.

----------

✅ **Short summary**

இந்த chapter சொல்லுவது:

-   computer system = hardware + software
    
-   programs run ஆக system components work together
    
-   programmers system understanding இருந்தால் better programs எழுதலாம்
    
-   Hello program மூலம் system operation explain செய்யப்படும்
    
-   computer-ல் எல்லாம் **bits (0,1)** ஆக store செய்யப்படும்
