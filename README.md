[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description

# Assumptions

1. `Pastevents execute <index>` - Assumed that index is 1-indexed (going from 1,2...).
2. `Proclore` - Assume that executable path is the one we get by using `readlink()` on the pid.
3. `Seek` - Argument to search is case-sensitive.
4. Multiple commands executed using `;` have a space before and after the semicolon.
5. `seek` - For seek command, I have assumed that file and directory names do not have a '.' in it
6. `peek` - Colors for directories - blue, files - white, executables - green.
7. `seek` - assumed that flags can only be input as "-e -d" or "-e -f" or "-d" or "-f"
