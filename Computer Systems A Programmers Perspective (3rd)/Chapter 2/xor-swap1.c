#include<stdio.h>

void inplace_swap(int *x,int *y) {
    if (x == y) return; //just safty check

    *y = *x ^ *y; //internal binary calculas
    *x = *x ^ *y;
    *y = *x ^ *y;
}

void reverse_array(int a[], int cnt) {//cnt -> count
    int first, last;
    for (first = 0, last = cnt - 1;
        first < last;
        first++, last--) {
            inplace_swap(&a[first],&a[last]);
        }

}

void print_array(int a[], int cnt) {
    int i;
    for (i = 0; i < cnt; i++)
        printf("%d ",a[i]);
        printf("\n");
}

int main() {
    int a[] = {1,2,3,4,5};
    int n=5;

    printf("Before: ");
    print_array(a,n);

    reverse_array(a,n);

    printf("After: ");
    print_array(a,n);

    return 0;
}

/*
சூப்பர் 🔥 இப்போ நீ கேட்கிறதுதான் full flow (array reverse + XOR swap) —
👉 {1,2,3,4,5} → 5 4 3 2 1
👉 அதிலும் {2,4} swap step-யும் clearஆ காட்டணும்

நாம முழு flow simulate பண்ணலாம் 👇

🧠 🔥 INITIAL ARRAY
[1 2 3 4 5]
🔁 ITERATION 1 → swap(1,5)
x → 1
y → 5
STEP 1
y = 1 ^ 5 = 4
STEP 2
x = 1 ^ 4 = 5
STEP 3
y = 5 ^ 4 = 1
RESULT
[5 2 3 4 1]
🔁 ITERATION 2 → swap(2,4) 🔥 (நீ கேட்டது)
x → 2
y → 4
STEP 1
y = 2 ^ 4 = 6

👉 binary:

0010
0100
----
0110 = 6
STEP 2
x = 2 ^ 6 = 4

👉

0010
0110
----
0100 = 4
STEP 3
y = 4 ^ 6 = 2

👉

0100
0110
----
0010 = 2
RESULT
[5 4 3 2 1]
🔁 ITERATION 3
x → 3
y → 3

👉 skip (first < last condition ❌)

💥 FINAL OUTPUT
5 4 3 2 1
🧠 FULL FLOW SUMMARY
Initial:
[1 2 3 4 5]

After swap(1,5):
[5 2 3 4 1]

After swap(2,4):
[5 4 3 2 1]

Middle (3) unchanged
💣 CORE INSIGHT
ஒவ்வொரு iterationலும்:
outer elements swap ஆகுது
🔥 FINAL ONE-LINE
"XOR swap pair-by-pair apply பண்ணினா array reverse ஆகுது"
*/
//==============x=======x===========x======x=====

/*
அட இது தான் 🔥 முக்கியமான clarity — நீ ரொம்ப சரியான doubt கேட்டிருக்கிறாய்!

👉 நீ பார்த்தது:

x → 1
y → 5

👉 result:

x = 5
y = 1

👉 நீ கேள்வி:

"அப்போ 4,5,1 தான் கிடைச்சது — எப்படி முழு array [5 2 3 4 1] ஆகுது?"
🧠 🔥 CORE ANSWER

👉 XOR swap முழு array-யை change பண்ணாது
👉 அது 2 elements மட்டும் change பண்ணும்

⚙️ STEP-BY-STEP (REAL VIEW)
ஆரம்ப array
[1 2 3 4 5]
நாம் call பண்ணுறது
inplace_swap(&a[0], &a[4]);

👉 இதன் meaning:

a[0] மட்டும்
a[4] மட்டும்
🔥 INSIDE SWAP (only these change)
a[0] = 1 → 5
a[4] = 5 → 1

👉 middle elements:

a[1] = 2  (unchanged)
a[2] = 3  (unchanged)
a[3] = 4  (unchanged)
💥 FINAL ARRAY
[5 2 3 4 1]
🧠 IMPORTANT REALIZATION

👉 இந்த calculation:

y = 1 ^ 5 = 4
x = 1 ^ 4 = 5
y = 5 ^ 4 = 1

👉 இது temporary internal steps மட்டும்

👉 ஆனால் memory-ல:

final values மட்டும் save ஆகும்
🔥 VISUAL (VERY CLEAR)
Before:
[1 2 3 4 5]

Swap only positions 0 and 4:

[? 2 3 4 ?]
 ↑       ↑

👉 after swap:

[5 2 3 4 1]
💣 KEY POINT
XOR intermediate values (like 4) memoryல finalா இருக்காது ❗

👉 அது:

temporary calculation மட்டும்
⚡ ANALOGY

👉 imagine:

2 பேர் bag exchange பண்ணுறாங்க

middle people untouched
🔥 FINAL SUMMARY
swap → only 2 positions change
rest array untouched
💥 FINAL ONE-LINE
"XOR steps உள்ளே நடக்கும் — ஆனால் arrayல இறுதியில் இரண்டு value மட்டும் மாற்றப்படும்"
*/

//====x========x========x=======x=========x=========

/*
சூப்பர் 🔥 நீ almost சரியாகப் புரிஞ்சுட்ட — ஆனா ஒரு சிறிய correction இருக்குது. அதை clear பண்ணுறேன் 👇

🧠 🔥 நீ சொன்னது
y = 1 ^ 5 = 4 → temporary
x = 1 ^ 4 = 5
y = 5 ^ 4 = 1 → final

👉 நீ கேள்வி:

"முதல் line temporary தானே? கடைசி இரண்டு தான் actual?"
❌ இது முழுக்க சரி இல்லை

👉 உண்மையில்:

மூன்று steps-உம் actual memory update தான் ❗
⚙️ REAL FLOW (memory level)

Initial:

x = 1
y = 5
🔥 Step 1
y = 1 ^ 5 = 4

👉 இப்போ memory:

x = 1
y = 4   ✅ (real update)
🔥 Step 2
x = 1 ^ 4 = 5

👉 memory:

x = 5   ✅
y = 4
🔥 Step 3
y = 5 ^ 4 = 1

👉 memory:

x = 5
y = 1   ✅
💥 FINAL
swap complete
🧠 முக்கிய உண்மை

👉 Step 1:

temporary இல்லை ❌
actual update தான் ✅

👉 ஆனா:

அந்த value (4) intermediate மட்டும்
finalல் இருக்காது
🔥 CORRECT UNDERSTANDING
Step1 → mix values
Step2 → recover one value
Step3 → recover other value
⚡ WHY CONFUSION வந்தது?

👉 because:

4 final resultல இல்லை

👉 அதனால்:

temporary போல feel ஆகுது

👉 ஆனால் reality:

memoryல அது இருந்தது (Step1 & Step2ல)
💣 SUPER IMPORTANT
"temporary value ≠ temporary memory"

👉 இது:

Type	Meaning
temporary value	intermediate calculation
memory value	actually stored

👉 இங்கே:

4 → intermediate + stored (short time)
🔥 VISUAL TIMELINE
Step0: x=1, y=5
Step1: x=1, y=4
Step2: x=5, y=4
Step3: x=5, y=1
💥 FINAL ONE-LINE
"Step1 value temporary இல்லை — அது short time memoryல இருக்கும் intermediate value"
*/