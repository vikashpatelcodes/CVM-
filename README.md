# CVM++ - Stack-Based Virtual Machine & Custom Compiler

CVM++ is a custom scripting language and execution environment built entirely in C++. The project demonstrates the complete compilation pipeline, from converting source code into tokens and Abstract Syntax Trees (ASTs) to generating bytecode and executing it on a custom stack-based Virtual Machine.

The goal of this project is to understand how programming languages, compilers, and runtime environments work internally by implementing them from scratch.

---

## Features

* Custom scripting language syntax
* Integer and Boolean data types
* Variable declaration and assignment
* Arithmetic operations (`+`, `-`, `*`, `/`)
* Comparison operators (`==`, `<`, `>`, `<=`, `>=`)
* Conditional statements (`if`, `else`)
* Iterative loops (`while`)
* User input and console output
* Abstract Syntax Tree (AST) generation
* Custom bytecode compiler
* Stack-based Virtual Machine execution
* Debug support for token inspection and bytecode visualization

---


## Sample scripts

There are a few example scripts in the `scripts/` folder:

| File | What it does |
|---|---|
| `test.cvm` | basic variables and arithmetic |
| `loop.cvm` | simple while loop |
| `ifelse.cvm` | if/else branching |
| `countdown.cvm` | counts down from 10 to 1 |
| `input_test.cvm` | takes two numbers from user and adds them |
| `fizzbuzz.cvm` | fizzbuzz up to 15 |

---

## Project structure

```
CVM++/
├── src/
│   ├── lexer.h
│   ├── lexer.cpp
│   ├── parser.h
│   ├── parser.cpp
│   ├── compiler.h
│   ├── compiler.cpp
│   ├── vm.h
│   ├── vm.cpp
│   └── main.cpp
├── scripts/
│   ├── test.cvm
│   ├── loop.cvm
│   ├── ifelse.cvm
│   ├── countdown.cvm
│   ├── input_test.cvm
│   └── fizzbuzz.cvm
└── README.md
```

## Compilation Pipeline

Source Code
↓
Lexer
↓
Tokens
↓
Parser
↓
Abstract Syntax Tree (AST)
↓
Compiler
↓
Bytecode
↓
Virtual Machine
↓
Program Output

Each stage is implemented as a separate module to keep the architecture clean and extensible.

---



## Building the project

You need g++ installed. Then just run:

```bash
g++ src/main.cpp src/lexer.cpp src/parser.cpp src/compiler.cpp src/vm.cpp -o cvm
```

That produces a `cvm` executable in the current folder.

---

## Running it

Three ways to use it:

**Run a script file**
```bash
./cvm scripts\test.cvm
```

**REPL type and run code interactively**
```bash
./cvm
```

Inside the REPL:
- type code line by line, variables persist across lines
- type `reset` to clear all variables and start fresh
- type `debug` to toggle bytecode printing
- type `exit` to quit

**Debug mode see the bytecode your code compiles to**
```bash
./cvm --debug scripts\test.cvm
```

---

## The language

Pretty minimal by design. Here's what it supports:

**Variables**
```
let x = 10
let y = x + 5
```

**Arithmetic**
```
let result = x + y * 2 - 1
```
Supports `+`, `-`, `*`, `/`

**Comparisons**
```
x == 10
x < 20
x > 5
```

**If / else**
```
if (x < 20) {
    print x
} else {
    print 0
}
```

**While loops**
```
let i = 0
while (i < 5) {
    print i
    let i = i + 1
}
```

**Print**
```
print x
print x + y
```

**Input**  reads an integer from the user
```
let x = input
print x
```

**Booleans**
```
let flag = true
let flag = false
```


---

## Learning Outcomes

This project provided hands-on experience with:

* Compiler Design
* Lexical Analysis
* Recursive Descent Parsing
* Abstract Syntax Trees (AST)
* Bytecode Generation
* Instruction Set Architecture (ISA)
* Virtual Machine Design
* Stack-Based Execution Models
* Runtime Environment Implementation
* Modern C++ Development

---

## Future Improvements

* Functions and function calls
* Arrays and strings
* Garbage collection
* Optimized bytecode execution
* Better error diagnostics
* Standard library support
* Interactive REPL

---

## Author

Vikash Patel

Built as a systems programming project to explore how modern programming languages and virtual machines work internally.
