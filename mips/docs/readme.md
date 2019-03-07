# MIPS Architecture Overview
## Basics
We are Dealing with 32-bit Mips architecture so all instructions are 32 bits.<br/>
### Registers
There are 32 general purpose registers.<br/>
These registers can be represented in two ways:
- Using the register number (**$0 - $31**)
- Using the corresponding register name ( `$v0`, `$gp` and so on )
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
There are two types of addressing Load(Takes data from RAM) and Store(Stores data to Ram):<br/>
**Load address( la )** :- Used for loading an address into a register
```
 la $t0,addr
```
The above instruction will copy the address into register $t0<br/>

**Load Word( lw )** :- Used to load a value into a register
```
 lw $t1,($t0)
```
The above instruction will load the 'word' from the address stored in t0 to the register t1<br/>

```
 lw $t1,k($t0)
```
The above instruction will load the 'word' value from the address `($t0+k)` where **k** is the offset and its value is an integer<br/>
This method of addressing can be used for:
- Accesing array elements from a base address
- To easily access elemnts from stack pointer and frame pointer

**Store Word ( sw )**:- Used to store a value into the memory
``` 
 sw $t2,($t0)
```
The above instruction will store the 'word' value in the register `t2` is stored in the memory on the address in `t0`<br/>
Offsets can be used in this instruction as well..

#### Arithemetic operations
Most of the arithemetic operations require 3 operands and operand size is word(4 bytes)<br/>
**Addition**:-<br/>
The instruction for signed addition is `and` and for unsigned addition is `addu`:
```
 add $t0,$t1,$t2 
 addu $t0,$t1,$t2
```
The above instruction adds the values in registers $t1 and $t2 then stores the result in $t0. The first instruction does signed 2's compliment addition while the second instruction does unsigned addition. <br/>
The equivalent statement is `t0 = t1 + t2`<br/>
To add an immediate value we use the command `addi` instruction
```
 addi $t0,$t1,5
```
The equivalent statement of the above instruction is `t0 = t1+5`<br/>
**Subtraction**:-<br/>
The instruction for signed(2's compliment) subtraction is `sub` and for unsigned addition is `subu`
```
 sub $t0,$t1,$t2
 subu $t0,$t1,$t2
```
The equivalent statement of the above instruction is `t0 = t1-t2`<br/>

**Multiplication**:-<br/>
The command for multiplication is `mult` 
```
 mult $t1,$t2
```
The above statement multiplies the two 32-bit values in `$t1` and `$t2` and stores the 64-bit result in `Hi` and `Lo`, `Hi` contains Most significant bits of the result and `Lo` contains the least significant bits of the result

**Division**:-<br/>
The command used for division is `div`:
```
 div $t1,$t2
```
The above statement stores the quotient in `Lo` register and remainder in `Hi` register<br/>
```
 Lo = t1/t2
 Hi = t1%t2
```
**Move Operations**:- <br/>
To move the value of a register to another we can use the `move` instruction
```
 move $t1,$t2
```
The equivalent statement of the above instruction is `t1 = t2`<br/>
To access the registers `Hi` and `Lo` we can use the instructions `mfhi` and `mflo`:
```
 mfhi $t0        # Moves the value in register Hi to $t0 : t0 = Hi
 mflo $t1        # Moves the value in register Lo to $t1 : t1 = Lo
```

## Control Structure
### Branches
You can unconditionally branch to a target label by using the instruction `b`
```
 b target
```
The above statement will branch to the program label `target`.<br/>
There is also conditional branching which can be used to jump to a target label given a condition
```
 beq	$t0,$t1,target	  #  branch to target if  $t0 = $t1
 blt	$t0,$t1,target	  #  branch to target if  $t0 < $t1
 ble	$t0,$t1,target	  #  branch to target if  $t0 <= $t1
 bgt	$t0,$t1,target	  #  branch to target if  $t0 > $t1
 bge	$t0,$t1,target	  #  branch to target if  $t0 >= $t1
 bne	$t0,$t1,target	  #  branch to target if  $t0 <> $t1
```

### Jumps
For an inconditional jump to a particular statement we can use the `j` instruction
```
 j target
```
<br/>
We can also jump to the address contained in a register using the instruction `jr`
```
 jr $t0
```

#### Function 
For a function call we can use the instruction `jal` which is the 'jump and link' instruction
```
 jal label
```
When the above instruction is executed, the following things happen:
- The program counter is copied to the `$ra` 
- Jump to the target label
For returing back we can use the instruction `jr` with the argument as `$ra` thus returning to the instruction after the function call
```
 jr $ra
```
For the full instruction list refer this [link](https://www2.cs.duke.edu/courses/fall13/compsci250/MIPS32_QRC.pdf)

