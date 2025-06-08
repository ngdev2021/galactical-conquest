# 🚀 Galactical Conquest - Agent Guidelines & Collaboration Charter

This document defines the roles, responsibilities, do's and don'ts for each Agent (or human developer) collaborating on the Galactical Conquest project.  
It also includes a **detailed approach to every project** to ensure smooth, conflict-free workflows.

---

## 🧩 1️⃣ General Principles

✅ Every Agent works in an **assigned branch + worktree**.  
✅ **No direct commits to `main` or `dev`**; always use feature branches.  
✅ All changes must go through **Pull Requests** and **PM Agent review**.  
✅ Keep **communication open** if you need to modify files owned by another Agent.

---

## 🎭 2️⃣ Agent Roles & Responsibilities

| Agent               | Responsibilities                                                                                      | Allowed Files / Areas               | Forbidden Actions                                           |
|----------------------|-------------------------------------------------------------------------------------------------------|--------------------------------------|------------------------------------------------------------|
| **Project Manager Agent** (PM) | Oversees roadmap, task assignments, and final merges. Reviews PRs, resolves conflicts, final integration. | All files (for oversight, not editing) | Editing features directly without feature branch |
| **C++ Expert Agent**    | Develops core game logic, physics, performance optimizations, and SFML integration.                     | `/src/*.cpp`, `/src/*.h` (core logic)  | Modifying UI assets or design elements |
| **UI/UX Agent**        | Designs game menus, HUD elements, animations, and visual feedback.                                       | `/assets/`, UI-related code in `/src/` | Changing core game mechanics or physics |
| **CloudOps Agent**     | Handles build systems, CI/CD pipelines, cross-platform compatibility, and deployment.                    | `/config/`, `Makefile`, CI scripts   | Modifying game logic or assets without approval |
| **QA Agent**           | Tests gameplay, identifies bugs, validates features, and ensures game balance.                          | `/tests/`, test documentation        | Changing production code without PM approval |

---

## 🛠️ 3️⃣ Project Workflow (End-to-End)

### a) Project Initialization
1. PM Agent creates the project structure and initializes **`main`** and **`dev`** branches.
2. PM defines initial tasks in **`PROJECT_PLAN.md`** or issue tracker (GitHub Projects, Jira, etc.).
3. PM creates feature branches for each Agent:
    - `feature/cpp-expert`
    - `feature/uiux-agent`
    - `feature/cloudops-agent`
    - `feature/qa-agent`
    - `feature/pm-agent`

### b) Development Process
✅ Each Agent clones their branch in a dedicated worktree:  
```bash
# Example
git worktree add worktrees/cpp feature/cpp-expert
```

✅ Each Agent:  
- Works only on **their designated areas**.  
- Uses **clean commits** with clear messages (`feat:`, `fix:`, etc.).  
- Keeps commits **atomic** (small, focused).

✅ If you must work in a file owned by another Agent:  
- **Communicate first!**  
- Create a separate branch if significant changes are needed (e.g., `feature/cpp-with-uiux-adjustments`).

### c) Pull Requests & Reviews
✅ When ready, **open a PR** to `dev`.  
✅ Include:  
- Summary of changes  
- Screenshots (for UI)  
- Testing evidence

✅ PM Agent reviews, suggests edits, and merges.

✅ If conflicts arise, PM Agent resolves with the involved Agents.

### d) Final Integration
✅ After all feature PRs merged to `dev`:  
- PM validates full feature integration.  
- Final testing (manual & automated).  
- PM merges to `main` for production.

---

## 🔄 4️⃣ Collaboration Rules

✅ **Branch Naming Conventions**:  
- Features: `feature/<agent-role>-<feature-name>`  
- Fixes: `fix/<agent-role>-<issue>`  
- Experiments: `exp/<agent-role>-<topic>`

✅ **Commit Message Format**:  
- `feat: added pause menu to Game class`  
- `fix: resolved collision detection in Player module`

✅ **File Ownership**:  
- Respect the areas assigned to each Agent.  
- Don't overwrite or remove another Agent's files without explicit agreement.

✅ **Testing**:  
- Always run unit & integration tests **before pushing**.  
- UI/UX must ensure visual consistency across platforms.  
- C++ Expert must validate performance and memory usage.

✅ **Communication**:  
- Use designated chat (Slack, Discord, etc.).  
- If unsure, ask the PM Agent or the file's owner Agent.

---

## 📦 5️⃣ Example: Collaborative Component Ownership (Game Menu)

| Task                          | Agent Responsible  | Notes                                                   |
|-------------------------------|--------------------|----------------------------------------------------------|
| Create menu layout + style    | UI/UX Agent        | Designs menu assets, transitions, and visual feedback    |
| Add menu navigation logic     | C++ Expert Agent   | Implements state management and input handling           |
| Platform-specific adaptations | CloudOps Agent     | Ensures menu works across Windows, macOS, Linux          |
| Gameplay balance testing      | QA Agent           | Validates difficulty levels and progression              |
| Feature integration           | PM Agent           | Coordinates all aspects into cohesive gameplay           |

---

## 🔍 6️⃣ Missing Elements / Gaps to Be Filled

1️⃣ **Shared File Ownership Clarification**  
- Some files (like `Game.cpp`) may require both C++ Expert and UI/UX edits—**always coordinate**.

2️⃣ **Agent-Level Documentation**  
- Each Agent should maintain a **mini README** in their worktree folder explaining:  
  - Key files they're touching  
  - Current status  
  - Any known issues

3️⃣ **Testing Strategy**  
- Establish consistent testing standards (unit, integration, gameplay).  
- Assign **QA Agent** to oversee this process.

4️⃣ **CI/CD Pipeline**  
- CloudOps Agent will configure a `.github/workflows/ci.yml` pipeline for:  
  - Compilation checks  
  - Tests  
  - Build previews for PRs

---

## 📝 7️⃣ Conclusion

By following these guidelines:  
✅ Each Agent knows what they **can** and **cannot** do.  
✅ Tasks are clear and conflict-free.  
✅ PM Agent ensures consistent, reliable progress.

Let's conquer the galaxy together—safely and collaboratively!
