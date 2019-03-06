# AVR Architecture Overview
The AVR architecture is a modified Harvard architecture 8-bit RISC architecture.
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
lds r17,ff00
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
sts ff00,r20
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
There is also the add immediate word `adiw` instruction that can be used to a "word" value to a register

