# Seccomp-Test
## _Use example for Seccomp_

Seccomp is a tool used to filter system calls, and in the same way, debug a program against possible vulnerabilities. If you need more inforamtion about it, you may want to check the Seccomp entry in the [Linux Manual (man)](https://man7.org/linux/man-pages/man2/seccomp.2.html) or in the [Wikipedia](https://en.wikipedia.org/wiki/Seccomp).

You may also want to check the [pdf file](https://github.com/jagumiel/seccomp-test/blob/master/SSC-Desarrollo_y_Demostracion-JA_Gumiel.pdf) on this repository. It is written in Spanish.


## Tree Structure

    .
    ├── Paso1                                           # Step 1: First scenario.
    ├── Paso2                                           # Step 2: Second scenario.
    ├── Paso3                                           # Step 3: Third scenario.
    ├── Paso4                                           # Step 4: Fourth scenario.
    ├── Instructions.txt                                # Compiling instructions.
    ├── SSC-Desarrollo_y_Demostracion-JA_Gumiel.pdf     # PDF in Spanish.
    └── README.md                                       # You are here.


## Requirements
An application will be programmed in C language. "Autoconf", "autoheader" and "make" will also be used. If it is not installed on your system, you will have to update the list of repositories and add these programs.

```sh
sudo apt-get update
sudo apt-get install gcc
sudo apt-get install autoconf
sudo apt-get install make
```

## How to compile?
Open the desired folder, E.g. Paso1 and generate the executable. You just have to introduce the following commands on your Linux console:

```sh
autoconf
autoheader
./configure
make
./procFork
```

The application is a fork. A father program creates a son program. It uses system calls, so this tutorial uses Seccomp to control the use of system calls, filtering and allowing ow denying the system call execution.

## Which are the test-cases?

The cases are divided into the 4 folders, which are based on the same base code.

### Paso 1 _(or **Step 1**):_ Initial application development
This is the base program. I am doing the test over this code. It is a simple application which creates a son process invocking the **fork()** system call.

You can compile and execute this program and you will see the execution is successfull.

### Paso 2 _(or **Step 2**):_ Adding filters to the application

Next, you have to add the "seccomp-bpf.h" library and update the "configure.ac" file to also add the "linux/seccomp.h" library.

An architecture will have to be made in the programming file. It will check if system calls are allowed. If it is not, the process will be terminated.

The result after the execution is an error message that does not give too much information. What be deduced from this result is that it applies the filter, since there is at least one call to the system that is not contemplated and that "seccomp" does not allow it to be done, ending the process inmediatly.

What is missing at this point is to get the system call that is being done, so it can be debugged afterwards.

### Paso 3 _(or **Step 3**):_ Reporting the system calls

In this step one of the added features of the “seccomp” filter will be used. This will allow to capture failed system calls and report the user immediately before terminating.

The meaning of this stage is to launch an execution, see what system calls are done and know them. In this way, the programmer can decide whether to allow them and add them to the "white list" or if, on the contrary, he must think of another way to program the application without making that call. The latter in the event that it could be a vulnerability for the system.

For this scenario, three files are necessary: “syscall-reporter.c”, “syscall-reporter.h” and “syscall-reporter.mk”. The latter has had to be modified, in Debian 9 the library path is different from the one that appears in the original file.

When compiling the program, the compiler warns that the "syscall-reporter" has been included in the program, and says that it is not used in production. It is fine to debugging purposes, but in a final application it is not frowned upon.

When executing, it tells us which system call it has stopped on. In this case, it warns of the first one it hunts, which is "clone ()",

### Paso 4 _(or **Step 4**):_ Debugging or allowing system calls

In this step we intend to see what happens when the "clone ()" system call, typical of "fork ()", is allowed. What will not be allowed is the "wait ()".

The "sleep ()" and "clone ()" have been allowed, but not the "wait ()". The parent does "wait ()" while the child is running, then it "kills" that process. However, the child uses a "sleep ()", which is allowed, and it finishes executing correctly.
