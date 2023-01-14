# ATMega-BlueCat
A stack-based virtual-machine (VM) written in C for the Atmel ATMega1280.

## Why a VM? 
I like writing VMs, and I'm keen to improve my implementations of stack-based VMs in general. For this specific project, a VM makes it easy to switch game cartridges while the console is running: Each cartridge will contain programs written in BlueCat assembly code, which can then be directly interpreted by the VM. In this case, each 'cartridge' will be a data interface (such as an SD card) that can be swapped at runtime to load different games from different sources. Using a VM also allows us the portability to retarget to any hardware that supports the C language - This will be especially useful if we find that the scope of the project outgrows the hardware capabilities of the ATMega1280.

## The BlueCat VM
The BlueCat VM is stack-based because it's easier to implement than a register-based VM. It uses an 8-bit instruction set with an absolute maximum of 256 unique instructions, although the actual number of instructions is likely to be much lower. BlueCat has a RISC archictecture, meaning that each instruction takes exactly one VM cycle to execute (Note that this is distinct from 'one clock-cycle on the ATMega1280'). 

## BlueCat vs BlueCat Assembly
The BlueCat project has two distinct languages - The BlueCat programming language itself, and BlueCat assembly. The BlueCat language is a weakly-typed general-purpose programming language, which is interpreted by the VM during compilation and output as BlueCat assembly (a series of BlueCat assembly commands which can be executed by the VM).

