# C Shell


## Display
On every line when waiting for user input, a shell prompt of the following form must appear along with it.
```
<Username@SystemName:~>
```
## Input Requirements
Shell supports ‘;’ or ‘&’ separated list of commands.

## Hop
#### Flags:
   - `.` : Stay in the current directory.
   - `..` : Move to the parent directory.
   - `~` : Move to the home directory.
   - `--` : Move to the previous directory (before the last hop command).
        
## Reveal 
   - `-l` : displays extra information
   - `-a` : displays all files, including hidden files
    Color Coding:
   - `Green` for executables.
   - `White` for regular files.
   - `Blue` for directories.
        
## Log
- `log`:
        Track the last 15 commands entered (excluding duplicates and the log command itself).
        Commands with ; or & are stored as a single entry.
- `log purge`:
        Clears all entries from the log.
- `log execute <index>`:
        Execute the command at the given index in the log. 

## System commands
- `Foreground`: Executing a command in foreground means the shell will wait for that process to complete and regain control afterwards. Control of terminal is handed over to this process for the time being while it is running.
- `Background`:Any command invoked with “&” is treated as a background command. This implies that your shell will spawn that process but doesn’t hand the control of terminal to it.

## Proclore
proclore is used to obtain information regarding a process. If an argument is missing, print the information of your shell.

Information required to print :
```sh

    pid

    Process Status (R/R+/S/S+/Z)

    Process group

    Virtual Memory

    Executable path of process
```


## Seek
#### Flags:
- `-d`: Only look for directories.
- `-f`: Only look for files.
- `-e`: If exactly one match is found, print the file contents (for files) or change to the directory (for directories).


## .myshrc 

The `.myshrc` file is a configuration file for customizing your shell environment. It can include aliases and functions to enhance your command-line experience.

### Aliases

You can create aliases for commands to simplify your workflow. Here's how you can define them:

**With alias keyword:**

```sh
alias ll='reveal -l'
alias home='hop ~'
alias parent='hop ..'
alias reveala='reveal -a'
```


## i/o Redirection

Your shell should support input and output redirection. Implement the following:

    - >: Redirects output to a file, overwriting if the file exists.
    - >>: Redirects output to a file, appending if the file exists.
    - <: Redirects input from a file.


## Pipes

Your shell should support piping commands using |. Commands should execute sequentially from left to right.

## Redirection along with Pipes 

Combine I/O redirection and pipes in your shell. Ensure proper handling of both features together.

## Activities 

Implement a command to list all running processes spawned by the shell, showing command name, PID, and state (running or stopped).

## Signals 

Implement a command to send signals to processes using their PID.

```
ping <pid> <signal_number>
```
Keyboard Shortcuts:

    Ctrl-C: Interrupts any running foreground process.
    Ctrl-D: Logs out of the shell, killing all processes.
    Ctrl-Z: Stops the currently running foreground process.

## fg and bg 

Implement commands to manage foreground and background processes.

```
fg <pid>: Brings a background process to the foreground.

bg <pid>: Resumes a stopped background process.
```

## Neonate 

Implement the neonate -n [time_arg] command to print the PID of the most recently created process every [time_arg] seconds until 'x' is pressed.

## iMan 

Implement iMan to fetch man pages from http://man.he.net/ and output them to the terminal.
