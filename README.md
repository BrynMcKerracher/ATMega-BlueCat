# BlueCat: An ATMega Game Console
**TLDR:** BlueCat is a language implementation and VM platform for game-development on Atmel ATMega chips.

## Project Overview 
BlueCat is a general-purpose programming language (GPL) implementation and stack-based virtual-machine (VM) written in Arduino/C++ for game development on Atmel [ATMega megaAVR](https://en.wikipedia.org/wiki/AVR_microcontrollers#:~:text=megaAVR%20%E2%80%93%20the%20ATmega%20series) chips. It aims to provide a Reduced Instruction Set Computer (RISC)-based VM and accompanying GPL for 2D graphics-based game-development on AVR ATMega chips. It is designed to work specifically with the XC4630 320x240 Colour TFT-LCD Display from Duinotech, an 8-button array for user input, a single 2-wire speaker, and a ANMBEST_MD272 MicroSD Card Reader for reading BlueCat Assembly Language (BAL) files to be interpreted by the VM.

## BlueCat General-Purpose Language
BlueCat GPL (BCGPL) is a weakly-typed, late-bound language based heavily on the [Lox](https://craftinginterpreters.com/the-lox-language.html) language by Robert Nystrom. It aims to provide a simple grammar for Object-Oriented Programming (OOP), with an emphasis on game-development targeting [ATMega megaAVR](https://en.wikipedia.org/wiki/AVR_microcontrollers#:~:text=megaAVR%20%E2%80%93%20the%20ATmega%20series) series chips from Atmel. The compilation process is a single pass to reduce the complexity of implementation. The compiler's output language is BlueCat Assembly Language.

## BlueCat Assembly Language
BlueCat Assembly Language (BAL) is the intermediary language produced by the BCGPL compiler and interpreted by the BlueCat VM (BCVM), and has the following properties:

**8-Bit Instruction Set**   
BlueCat assembly operates using an 8-bit instruction set. This is designed to maximise the interpretation speed of the BlueCat VM by using the smallest possible word size per instruction, which requires fewer RAM accesses per instruction sequence than using a larger word size. This comes at the cost of limiting the maximum size of the instruction set to 256, but provides the additional benefit of minimising RAM usage in a low-RAM environment.  

**16-Bit Address Space**  
The VM's total addressable space is 16-bits, split into two 8-bit words. A 16-bit address space is sufficient for addressing memory spaces of up to 64KB, which covers the entire range of megaAVR chips this project is interested in. Provisions have been made for targeting larger address spaces (and therefore chips outside the megaAVR range) by abstracting address implementation into its own module in which the underlying type for an address can be changed, but it should be noted that extending the address space will incur a performance loss in the VM and increase the size of BAL sequences produced by the compiler.

## The BlueCat VM
The BlueCat VM is a stack-based RISC-architecture machine that interprets and executes BAL sequences. Each BAL instruction takes exactly one VM instruction-cycle to execute.

# Documentation
Documentation is automatically deployed from the ```main``` branch and is found <a href="https://brynmckerracher.github.io/ATMega-BlueCat/">here</a>.
