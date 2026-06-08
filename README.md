# OSN Mini Project 1 - Mini C Shell

This repository contains the implementation of a custom C shell. The shell is capable of executing built-in commands as well as standard system commands with support for I/O redirection, piping, and background execution.

## Features Implemented (Shell)

The shell supports the following core functionalities:

### 1. Interactive Shell Prompt
- A custom shell prompt is displayed in the format: `<mohan@iiit:~[current_dir]>`.
- The prompt dynamically updates to reflect the current working directory relative to where the shell was launched.

### 2. Built-in Commands

#### `hop` (Change Directory)
Changes the current working directory. It supports the following flags:
- `hop ~` : Navigates to the home directory.
- `hop -` : Navigates to the previous working directory.
- `hop ..`: Navigates to the parent directory.
- `hop .` : Stays in the current directory.
- `hop <path>`: Navigates to a specific absolute or relative path.

#### `reveal` (List Directory Contents)
Lists the files and directories in a specified path (default is the current directory). Supports flags:
- `reveal` : Prints all visible files and directories.
- `reveal -a`: Prints all files and directories, including hidden ones (starting with `.`).
- `reveal -l`: Prints all visible files line-by-line.
- `reveal -al` / `-la`: Prints all files and directories (including hidden) line-by-line.

#### `log` (Command History)
Maintains a persistent history of the last 15 executed commands across multiple sessions.
- `log`: Displays all stored commands from oldest to newest.
- `log purge`: Clears all stored command history.
- `log execute <index>`: Re-executes the command at the specified index from the log.

### 3. System Commands Execution
Executes any standard system command (e.g., `echo`, `sleep`, `cat`) by using `fork()` and `execvp()`. The shell waits for foreground processes to complete before displaying the next prompt.

### 4. Background Processes
Supports running processes in the background by appending `&` to a command. The shell instantly returns control to the user without waiting for the background process to finish, printing the PID of the spawned background process.

### 5. Input/Output Redirection
Supports standard input and output redirection for commands:
- `<` : Redirects input from a file.
- `>` : Redirects output to a file (overwrites).
- `>>`: Redirects output to a file (appends).

### 6. Piping
Supports connecting the standard output of one command to the standard input of another using `|`. The shell can handle a sequence of multiple piped commands.

### 7. Semi-colon Separated Commands
Supports executing multiple independent commands sequentially on a single line, separated by `;`.

## How to Build and Run

1. Open your terminal and navigate to the `shell` directory:
   ```bash
   cd shell
   ```

2. Compile the shell using the provided `Makefile`:
   ```bash
   make
   ```
   This will compile the source code and generate an executable named `shell.out`.

3. Start the custom shell:
   ```bash
   ./shell.out
   ```
   
   *Alternatively, you can compile and start the shell in a single step using:*
   ```bash
   make run
   ```

### Cleaning Up
To remove the compiled object files and the executable, run:
```bash
make distclean
```
