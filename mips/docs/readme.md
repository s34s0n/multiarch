# MIPS Architecture Overview
## Basics
We are Dealing with 32-bit Mips architecture so all instructions are 32 bits.<br/>
### Registers
There are 32 general purpose registers.<br/>
These registers can be represented in two ways:
- Using the register number (**$0 - $31**)
- Using the corresponding register name ( $v0, $gp and so on )
The main registers are:
```
 $zer0     - A register that holds the value zero
 $at       - (Assembler Temporary) This register is reserved by the assembler 
 $v0-$v1   - (Values) Holds values from expression evaluation and function results
 $a0-$a3   - (Arguments) Holds parameters for called function
 $t0-$t7   - (Temporaries) Registers for holding temporary values. 
 $s0-$s7   - (Saved values) Register contains saved values for the calling function. 
 $t8-$t9   - (Temporaries) Register for holding temporary values(Additional to t0-t7)
 $k0-$k1   - Registers reserved for interrupt handling
 $gp       - (Global Pointer) Points to the global area (Static data segment)
 $sp       - (Stack Pointer) Points to the last location of the stack
 $fp       - (Frame Pointer) Points to the start of the stackframe 
 $ra       - (Return Address) Register which stores the return address 
```
The registers are numberes from top to bottom and each register has a corresponding number from 0-31
There are two unnumbered registers `Hi` and `Lo` which are used to store the results of Multiplication and Division

### Instructions
#### Addressing
There are two types of addressing Load(Takes data from RAM) and Store(Stores data to Ram):
**Load address( la )** :- Used for loading an address into a register