# Programming Mistake Notebook

A Qt-based desktop application for recording, organizing, searching, and reviewing programming mistakes.

## Overview

Programming Mistake Notebook is a course project developed by a three-person team at Peking University in 2025. The application is designed to help learners keep a structured record of programming problems, understand recurring mistakes, and review them over time.

The project report describes four main areas of functionality:

- **User management:** registration and login, with account credentials stored using encryption.
- **Mistake management:** structured records for problem statements, causes of mistakes, incorrect and corrected code, Markdown-formatted notes, tags, dates, and difficulty levels.
- **Learning support:** daily check-ins, random review, achievement milestones, and review statistics.
- **Interface customization:** light and dark themes, with Markdown rendering for notes.

## Features

### Organizing programming mistakes

The application supports creating, displaying, and editing mistake entries. The report describes entry fields for problem content, explanations of mistakes, code examples, algorithm tags, dates, and difficulty levels.

### Searching and reviewing

Mistakes can be searched using names and tags. The report also describes fuzzy search and combined queries, as well as a random-review feature for revisiting previously recorded problems.

### Learning progress

The project includes daily check-ins and learning goals, achievement milestones (such as consecutive check-in days and mastered mistakes), and review statistics. The report describes statistical views for analyzing frequent mistake types and difficulty distributions.

### Reading and appearance

Notes support Markdown-style content. The application also provides light and dark themes managed across its windows.

## Application Structure

The project report organizes the application into the following modules and classes:

| Area | Class | Responsibility described in the report |
| --- | --- | --- |
| User and main interface | `Login` | Login verification and loading user data |
| User and main interface | `MainWindow` | Main window, check-in, and random review |
| Mistake management | `CreatNewFile` | Creating a mistake entry |
| Mistake management | `ShowFile` | Displaying a mistake entry |
| Mistake management | `ShowFileEdit` | Editing a mistake entry |
| Learning support | `ShowMyAchievement` | Calculating and displaying achievements |
| Learning support | `ShowReviewStatistics` | Displaying review statistics |
| Learning support | `Ai_Linking` | API integration for AI-assisted analysis |
| Interface customization | `ThemeManager` | Managing themes across windows and the main interface |
| Interface customization | `MarkdownHighLighter` | Markdown rendering/highlighting in the input area |

The report's project-structure diagram is included in the original course report.

## Technology

- **Language:** C++
- **GUI framework:** Qt
- **Additional components described in the report:** Markdown rendering/highlighting and an external AI API integration

The repository is a course-project codebase. Please consult the project files and report for implementation details and the exact build configuration.

## Repository Contents

```text
.
├── notebook_code/          # Application source code and project files
├── video.mp4               # Project demonstration video
├── 82-作业报告.pdf          # Original course project report (Chinese)
└── README.md
```

## Demo and Project Report

- **Demo video:** [`video.mp4`](./video.mp4)
- **Project report (Chinese):** [`82-作业报告.pdf`](./82-作业报告.pdf)

The report includes the functional specification, module/class design, team task division, and project reflection.

## Project Context

This application was developed as a three-person university course project. The report identifies future directions including collaborative multi-user features, sharing and printing mistake records, and further improvements to the learning-support functions.

---

*This README summarizes the project as described in the accompanying course report. Feature descriptions reflect the report's project specification; consult the source code to inspect the corresponding implementation.*
