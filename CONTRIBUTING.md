# CONTRIBUTING TO 2BOIS ENGINE

Welcome, brave soul.

If you're reading this, you've somehow decided to contribute to the world's first 3D ASCII Game Engine built on caffeine, cigarettes, and questionable life choices. Respect.

But before you touch any code, **read this**.

---

## 🔒 RULE ZERO: NO NAVEED

If your name is **Naveed**, please stop reading and go reflect on your actions.  
You're not welcome here. You know what you did.

---

## 🧠 C++ CONVENTIONS

We want clean, consistent, and understandable code — even if we’re clinically insane.

### 🧱 Code Style
- **Use C++17** (or higher, if discussed).
- Use `.hpp` for headers, `.cpp` for source files.
- **Namespaces > Classes > Files**  
  Keep things modular. No monolithic `engine.cpp`.

### 🧼 Naming
- **CamelCase** for classes and structs: `GameObject`, `AsciiRenderer`
- **snake_case** for variables and functions: `load_mesh()`, `project_3d_to_2d()`
- Constants in `SCREAMING_SNAKE_CASE`


### 🔄 Code Practices
- Prefer `std::vector`, `std::array`, and smart pointers over raw memory.
- **RAII everything.** If it allocates, it should also deallocate.
- **No macros unless you’re writing curses (and we’re not using curses).**

---

## ✅ HOW TO CONTRIBUTE

1. Fork the repo.
2. Create a new branch:  
   `git checkout -b feature/your-thing`
3. Write readable code. Add comments. Don’t be Naveed.
4. PRs should be **atomic** — one feature/fix per PR.
5. Write a **clear commit message**. No `update lol`.
6. Make us laugh. If your code works and your PR description is funny, it will be merged faster.

---

## 🚨 WARNING

This project is being built with no safety rails.  
We may refactor things on a whim. We may pivot mid-feature.  
We may replace a renderer with a potato.

That's the game.

---

With love,  
Mush1e & Danqzq  
💻☕🚬

