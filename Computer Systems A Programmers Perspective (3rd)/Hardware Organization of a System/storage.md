சரி Jarvis! Book content-க்கு stick பண்றேன் — simple, clean explanation.

---

# 1.6 Storage Devices Form a Hierarchy

இந்த section-ரோட core idea ஒண்ணே ஒண்ணுதான்:

**"Smaller + faster device = cache for the next bigger + slower device"** — இந்த pattern every level-லயும் repeat ஆகுது.

## Memory Hierarchy — L0 to L6## Book-ரோட main points

**ஒவ்வொரு level-உம் கீழே இருக்கற level-க்கு cache:**

- Register file → L1 cache-க்கு cache
- L1, L2 → L2, L3-க்கு cache (respectively)
- L3 → Main memory-க்கு cache
- Main memory → Disk-க்கு cache
- Local disk → Remote storage-க்கு cache (networked systems-ல)

**மேலே போக போக:** smaller, faster, costlier per byte

**கீழே போக போக:** larger, slower, cheaper per byte

## Key takeaway (book directly சொல்றது)

Programmers cache memory knowledge exploit பண்ணி performance improve பண்ணலாம் — அது மட்டும் இல்லாம, **entire memory hierarchy-ஐ** புரிஞ்சுக்கணும். Chapter 6-ல இதை deep-ஆ cover பண்ணும்.

---

அடுத்து 1.7 போகலாமா?