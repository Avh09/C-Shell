# PART A

### Assumptions:
  - Size of all inputs and variables should be less than 4096
  - Error messages are color coded in red, username@sysname is done according to me.
  - If -e flag is present alone without any other flag and then it's condition isn't satisfied then, this case will be treated as if no flag was given.
  - Proclore executable path both absolute or relative's valid, mine prints absolute.

## Specification 1: display
On every line when waiting for user input, a shell prompt of the following form must appear along with it.
```
<Username@SystemName:~>
```
## Specification 2: input Requirements
Shell supports ‘;’ or ‘&’ separated list of commands.

## Specification 3: hop
#### Flags:
   - `.` : Stay in the current directory.
   - `..` : Move to the parent directory.
   - `~` : Move to the home directory.
   - `--` : Move to the previous directory (before the last hop command).
        
## Specification 4: reveal 
   - `-l` : displays extra information
   - `-a` : displays all files, including hidden files
    Color Coding:
   - `Green` for executables.
   - `White` for regular files.
   - `Blue` for directories.
        
## Specification 5: log
- `log`:
        Track the last 15 commands entered (excluding duplicates and the log command itself).
        Commands with ; or & are stored as a single entry.
- `log purge`:
        Clears all entries from the log.
- `log execute <index>`:
        Execute the command at the given index in the log. 

## Specification 6: System commands
- `Foreground`: Executing a command in foreground means the shell will wait for that process to complete and regain control afterwards. Control of terminal is handed over to this process for the time being while it is running.
- `Background`:Any command invoked with “&” is treated as a background command. This implies that your shell will spawn that process but doesn’t hand the control of terminal to it.

## Specification 7: proclore
proclore is used to obtain information regarding a process. If an argument is missing, print the information of your shell.

Information required to print :
```sh

    pid

    Process Status (R/R+/S/S+/Z)

    Process group

    Virtual Memory

    Executable path of process
```


## Specification 8: seek
#### Flags:
- `-d`: Only look for directories.
- `-f`: Only look for files.
- `-e`: If exactly one match is found, print the file contents (for files) or change to the directory (for directories).


# PART B 

### Assumptions
- __ > __ < __ is an invalid io input
- `echo hi | wc &` whole this is treated as bg
- `> >` is invalid (written in doubt doc)
- & will not be handled before/after |
- If `sleep 20 ; sleep 10; sleep 5` is my command, `Ctrl C` should be pressed three times to terminate all these running commands.
- For alias, `;` won't work as it won't be recognised by the mapping and hence will send to fg process execution function which will throw `execvp` error.


## Specification 9: .myshrc 

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


## Specification 10: i/o Redirection

Your shell should support input and output redirection. Implement the following:

    - >: Redirects output to a file, overwriting if the file exists.
    - >>: Redirects output to a file, appending if the file exists.
    - <: Redirects input from a file.


## Specification 11: pipes

Your shell should support piping commands using |. Commands should execute sequentially from left to right.

## Specification 12: redirection along with Pipes 

Combine I/O redirection and pipes in your shell. Ensure proper handling of both features together.

## Specification 13: activities 

Implement a command to list all running processes spawned by the shell, showing command name, PID, and state (running or stopped).

## Specification 14: signals 

Implement a command to send signals to processes using their PID.

```
ping <pid> <signal_number>
```
Keyboard Shortcuts:

    Ctrl-C: Interrupts any running foreground process.
    Ctrl-D: Logs out of the shell, killing all processes.
    Ctrl-Z: Stops the currently running foreground process.

## Specification 15: fg and bg 

Implement commands to manage foreground and background processes.

```
fg <pid>: Brings a background process to the foreground.

bg <pid>: Resumes a stopped background process.
```

## Specification 16: neonate 

Implement the neonate -n [time_arg] command to print the PID of the most recently created process every [time_arg] seconds until 'x' is pressed.

## Specification 17: iMan 

Implement iMan to fetch man pages from http://man.he.net/ and output them to the terminal.
