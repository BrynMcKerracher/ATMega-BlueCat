# ATMega-BlueCat
A stack-based virtual-machine (VM) written in C++ for the Atmel ATMega chips.

## Project Overview 
Project BlueCat is a language implementation and VM platform for game-development on Atmel ATMega chips. It aims to provide a Reduced Instruction Set Computer (RISC) based VM and accompanying general-purpose programming language (GPL) for 2D graphics-based game-development on AVR ATMega chips.

The whole BlueCat project covers three main things:
### BlueCat GPL
BlueCat GPL is a weakly-typed, late-bound language based heavily on the Lox language by Robert Nystrom. It aims to provide a simple grammar for Object-Oriented Programming (OOP), with an emphasis on game-development for targeting [ATMega megaAVR](https://en.wikipedia.org/wiki/AVR_microcontrollers#:~:text=megaAVR%20%E2%80%93%20the%20ATmega%20series)-series chips from Atmel. The compilation process is a single pass to reduce the complexity of implementation, and the compiler's output language is BlueCat Assembly.

### BlueCat Assembly Language
BlueCat Assembly is an 8-bit instruction set designed to maximise the interpretation speed of the BlueCat VM by using the smallest possible word size for an instruction and therefore requiring fewer SRAM accesses compared to a larger word size, at the cost limiting the number of unique instructions to 256. The total address space addressable by the VM is 16-bits, split into two 8-bit words, though provisions for targeting larger address spaces have been made by abstracting address imnplementation into its own module.

### The BlueCat VM
The BlueCat VM is a stack-based RISC-architecture machine. Each operation takes exactly one VM instruction-cycle to execute. 

# Documentation
Documentation is automatically deployed from the ```main``` branch and is found <a href="https://brynmckerracher.github.io/ATMega-BlueCat/">here</a>.
