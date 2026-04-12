சூப்பர் 🔥 இது real-world question — interview level 💯
நான் straightஆ சொல்லுறேன் 👇

---

# 🏢 பெரிய கம்பெனிகள் எப்படி எழுதுவாங்க?

👉 Examples:

* Google
* Microsoft
* Meta

---

## 🎯 அவர்கள் follow பண்ணுற rule

👉
❌ `using namespace std;` use பண்ண மாட்டாங்க

👉
✅ `std::cout` மாதிரி explicitஆ எழுதுவாங்க

---

# 🧠 ஏன் `using namespace std;` avoid பண்ணுறாங்க?

---

## ⚠️ 1. Name conflict problem

👉 பெரிய projectல:

* thousands of files
* multiple libraries

---

### ❗ Example:

```cpp
int count;   // your variable
std::count;  // algorithm
```

👉 conflict 😬

---

## ⚠️ 2. Debugging problem

👉 error வந்தா:

* எந்த function use ஆகுது தெரியாது

---

## ⚠️ 3. Readability

👉 `std::cout` பார்த்தவுடன்:

👉
**“standard libraryல இருந்து வந்தது”ன்னு தெரியும்**

---

# 🧩 Real industry style

---

## ✅ Correct style

```cpp
#include <iostream>

int main() {
    std::cout << "Hello";
}
```

---

## ❌ Avoid this

```cpp
using namespace std;
```

---

# 🧠 4. Exception (சில cases)

👉 சில நேரம்:

```cpp
using std::cout;
using std::cin;
```

👉 allowed ✔️

---

# 📜 Real guideline

👉 Google C++ Style Guide

👉 clearly சொல்லுது:

👉
**“Don’t use using namespace std;”**

---

# 🎯 Summary table

| Practice               | Industry   |
| ---------------------- | ---------- |
| `std::cout`            | ✅ YES      |
| `using namespace std;` | ❌ NO       |
| `using std::cout;`     | ⚠️ limited |

---

# 💥 One line takeaway

👉 **“Professional codeல clarity முக்கியம்… அதனால் std:: always use பண்ணுவாங்க”**

---

🔥 நீங்க இதை இப்போவே follow பண்ணினா
👉 futureல interview + job easy ஆகும் 💪

---

👉 Next level போகலாமா?
👉 நான் உங்களுக்கு **real company-level C++ coding style (clean code rules)** சொல்லி தரலாமா 😄
