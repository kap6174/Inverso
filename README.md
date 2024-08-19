<!-- To view the README.md in proper format open preview-->
# Inverso

Junaid Ahmed  
Roll No: 2024201018  
M.Tech CSE, IIITH

---

This assignment consists of two programs:

- **2024201018_A1_Q1.cpp**: This program reverses the content of a given file. Depending on the provided flag, it either reverses the entire file or specific portions of it.
- **2024201018_A1_Q2.cpp**: This program checks the permissions of two files and a directory, and verifies if the content of one file is the reverse of another.

---

### Files Included

- **2024201018_A1_Q1.cpp**: Code for reversing the file content based on flag.
- **2024201018_A1_Q2.cpp**: Code for checking permissions and verifying file content reversal.
- **README.md**: This file, containing instructions for executing the code.

---

### Requirements

- **C++ compiler** (e.g., g++)
- **Linux environment** (for system calls like `stat`, `mkdir`, etc.)

---

### Compilation Instructions

To compile the programs, run the following commands:

```bash
g++ -o 2024201018_A1_Q1 2024201018_A1_Q1.cpp
g++ -o 2024201018_A1_Q2 2024201018_A1_Q2.cpp 
```
---

### Execution Instructions

1. **Q1.cpp**

   To reverse a file, run the following command:

   ```bash
   ./2024201018_A1_Q1 <input_file> <flag> [<start_index> <end_index>]
   ```

   - `<input_file>`: Path to the file to be reversed.
   - `<flag>`:
     - `0` to reverse the entire file.
     - `1` to reverse the file in parts (beginning to `start_index` and `end_index` to end, leaving the middle part unchanged).
   - `[<start_index> <end_index>]`: (Only required when flag is `1`) Indices to specify the portion of the file that should remain unchanged.

   **Example:**

   - **Reverse the entire file:**

     ```bash
     ./2024201018_A1_Q1 input.txt 0
     ```

   - **Reverse parts of the file:**

     ```bash
     ./2024201018_A1_Q1 input.txt 1 2 7
     ```

   The output will be saved in a file named `Assignment1/<flag>_<input_file>`.

2. **Q2.cpp**

   To check permissions and verify file content, run the following command:

   ```bash
   ./2024201018_A1_Q2 <newfile_path> <oldfile_path> <directory_path>
   ```

   - `<newfile_path>`: Path to the new file (the reversed file).
   - `<oldfile_path>`: Path to the original file.
   - `<directory_path>`: Path to the directory created by the first program.

   **Example:**

   ```bash
   ./2024201018_A1_Q2 Assignment1/0_input.txt input.txt Assignment1
   ```

   This program will output:
   - Whether the directory was created successfully.
   - Whether the file contents are reversed correctly.
   - If both files have the same size.
   - User, group, and others' permissions for the new file, old file, and the directory.

   **Notes**
   - The programs use system calls for file manipulation and permission checks.
   - Ensure that the input files and directories exist before running the programs.
