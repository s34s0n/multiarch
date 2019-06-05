# AVR Architecture Overview
The AVR architecture is a modified Harvard architecture. Were looking into the 8-bit RISC avr architecture. This architecture is mainly used in microcontrollers.
## Registers
There are a total of 31 registers in AVR `r1-r31` and from there registers the registers `r26-r31` are 16-bit address pointers for indirect addressing. The three indirect address registers X, Y, and Z are defined as described in the figure.
- `r1-r25` general purpose 8-bit registers
- `X` Register
	- `r26` - X register's lower byte
	- `r27` - X register's higher byte
- `Y` Register
	- `r28` - Y register's lower byte
	- `r29` - Y register's higher byte
- `Z` Register
	- `r30` - Z register's lower byte
	- `r31` - Z register's higher byte

## Basic Instructions

### Addressing
There are two types of intructions used are loading instructions and storing instructions 

#### Loading from the memory
-  `ld` - Load Indirect from data space
	- Loads one byte indirect with or without displacement from the data space to a register.
	- To Load with displacement you can use the `ldd` instructionwhich is usefull for loading values from consecutive addresses
```
ld r0,Z                                 # Load r0 with data space with loc stored in Z
ld r1,Y+                                # Load r1 with data space loc stored in Y and inc the addr in Y
ld r3,-X                                # Decr the addr in X and Load r3 with data in the corresponding addr
ldd r4,Z+k                              # Load r4 with data in the addr stored in Z + k
```
- `ldi` - Load Immediate
	- This instruction can be used to directly load a byte value into the registers `r16-r31`
```
ldi r16,f0
``` 
- `lds` - Load Direct from data space
	- This instruction can be used to load a value from a particular address in the memory to a register
```
lds r17,0xff00
```

#### Storing into the memory
-  `st` - Store Indirect from the register to data space
	- Stores one byte indirect with or without displacement from the register to the data space.
	- To Store with displacement you can use the `std` instruction which is usefull for loading values from consecutive addresses
```
st Z,r0                                 # Store the value in r0 to the loc stored in Z
st Y+,r1                                # Store the value in r1 to the loc stored in Y and inc the addr in Y
st -X,r2                                # Decr the addr in X and Load r3 with data in the corresponding addr
std Z+k,r3                              # Load r4 with data in the addr stored in Z + k
```
- `sts` - Store Direct to data space
	- This instruction can be used to store a value in a register to a particular address in the memory
```
sts 0xff00,r20
```

#### Data Transfer between registers
We can use the `mov` instruction to copy the value in one register to another 
```
mov r0,r1
```

### Input/Output
Since this architecture is manly used in microcontrollers the inputs and outputs are through ports, timers, physical pins etc.
We use the `in` instruction for input and `out` instruction for output
```
in r25,0x16
out 0x3f,r1
```

### Arithemetic operations
Most of the arithemetic operations require 3 operands and operand size is word(4 bytes)<br/>
**Addition**:-<br/>
The instruction for addition without carry is `add` and for addition with carry is `adc`: 
```
 add r1,r2
 adc r3,r4
```
The `adc` command adds the two registers and also adds the carry from the previous instruction.<br/>
There is also the add immediate word `adiw` instruction that can be used to a "word" value to a register.

**Subtraction**:-<br/>
The instruction for subtraction without carry is `sub` and for subtraction with carry we use the `sbc` instruction
```
sub r1,r2
sbc r3,r4
```
For immediate subtraction there is the `sbci` which is thw subtract immediate with carry and `sbiw` which is subtract immediate from word

### Bitwise instructions
The basic logical instructions in avr are:
- **Bitwise AND**:-<br/>
The instruction used is `and` for bitwise **and** for the values `andi` for bitwise **and** of a register with an immediate value. This instruction does a bitwise **and** of the operands an stores it in the first operand.  
```
and r1,r2
andi r2,0x34
```
- **Bitwise OR**:-<br/>
The instruction used is `or` for bitwise **or** for the values `ori` for bitwise **or** of a register with an immediate value. This instruction does a bitwise **or** of the operands an stores it in the first operand. 
```
and r1,r2
andi r2,0x34
```
- **Bitwise XOR**:-<br/>
The instruction used for performing bitwise **XOR** is `eor`. This instruction does a bitwise xoring of the two operands and stores it in the first operand.

## Control Structure
### Unconditional Jumps
For an unconditional jump to a particular instruction we can use the `jmp` instruction
```
jmp k              
```
This instruction moves the value of k into the program counter

There is also relative jump `rjmp` to jump relative to the current value of program counter
```
rjmp k             ; pc <- pc + k + 1
```

### Conditional Branching
The Program execution can branch according to a particular condition based on the compare statements `cp`, `cpc`(Compare with carry), `cpi`(Compare immediate) 
```
cp r0,r1
cpc r2,r3
cpi r4,k
```   
This statement subtracts the second operand from the first operand and sets the necessary flags.<br/>
The branch statements work according to the values in the flags
```
breq k			; if Z = 1 then pc <- pc + k + 1
brne k 			; if Z = 0 then pc <- pc + k + 1
brcs k 			; if C = 1 then pc <- pc + k + 1
brcc k 			; if C = 0 then pc <- pc + k + 1
brge k 			; if S = 0 then pc <- pc + k + 1
brlt k 			; if S = 1 then pc <- pc + k + 1
brmi k 			; if N = 1 then pc <- pc + k + 1
brpl k 			; if N = 0 then pc <- pc + k + 1
```

The flag registers are:
- C : Carry flag
- Z : Zero flag
- N : Negative flag
- V : Two's Compliment Overflow Flag
- S : Sign Flag , S = N ^ V
- H : Half Carry Flag
- T : Copy Storage
- I : Global Interrupt Enable

###Subroutine Call
You can call a subroutine using the `call` instruction. The current value of program counter will be pushed to the stack and the program counter will be modified.
```
call k 			; push pc and pc <- k
```
To return to the calling function we can use the `ret ` statement

For the full instruction list refer this [link](http://www.avr-tutorials.com/sites/default/files/Instruction%20Set%20Summary.pdf)
