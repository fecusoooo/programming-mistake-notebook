# Programming Mistake Notebook

A Qt-based desktop application for recording, organizing, searching, and reviewing programming mistakes.

## Overview

Programming Mistake Notebook is a course project developed by a three-person team at Peking University in 2025. The application is designed to help learners keep a structured record of programming problems, understand recurring mistakes, and review them over time.

## Features

### Organizing programming mistakes

The application supports creating, displaying, and editing mistake entries. The report describes entry fields for problem content, explanations of mistakes, code examples, algorithm tags, dates, and difficulty levels.

### Searching and reviewing

Mistakes can be searched using names and tags. The report also describes fuzzy search and combined queries, as well as a random-review feature for revisiting previously recorded problems.

### Learning progress

The project includes daily check-ins and learning goals, achievement milestones (such as consecutive check-in days and mastered mistakes), and review statistics. The report describes statistical views for analyzing frequent mistake types and difficulty distributions.

### Reading and appearance

Notes support Markdown-style content. The application also provides light and dark themes managed across its windows.

## Technology

- **Language:** C++
- **GUI framework:** Qt
- **Additional components described in the report:** Markdown rendering/highlighting and an external AI API integration

Please consult the project files and report for implementation details and the exact build configuration.

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

