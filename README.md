# ATMega-BlueCat
A stack-based virtual-machine (VM) written in C for the Atmel ATMega1280.

## Project Overview 
Project BlueCat is a language implementation and VM platform for ATMega1280 game-development. It aims to provide a Reduced Instruction Set Computer (RISC) based VM and accompanying general-purpose programming language (GPL) for graphics-based game-development on Arduino ATMega chips.

The whole BlueCat project is pretty big in scope and covers three main things:
### BlueCat GPL
BlueCat GPL is a weakly-typed, late-bound language based heavily on the Lox language by Robert Nystrom. It aims to provide a simple grammar for Object-Oriented Programming (OOP), and specifically game-development. The compilation process is a single pass to reduce the complexity of implementation, and the compiler's output language is BlueCat Assembly.

### BlueCat Assembly Language
BlueCat Assembly is an 8-bit instruction set prioritising fast vector-math calculations.

### The BlueCat VM
The BlueCat VM is a stack-based RISC-architecture machine. Each instruction takes exactly one clock-cycle to execute to allow for the eventual addition of an instruction pipeline. The BlueCat VM is optimised for space-complexity whenever possible, with the exception of vector-math operations which are optimised for speed. The VM is stack-based to reduce the complexity of implementation.

# Docs
Documentation is automatically deployed from the ```main``` branch and is found <a href="https://brynmckerracher.github.io/ATMega-BlueCat/">here</a>.
