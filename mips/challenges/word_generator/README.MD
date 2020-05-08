First, let's take a look at the file.

```
➜ file word_generator
word_generator: ELF 32-bit MSB executable, MIPS, MIPS32 rel2 version 1 (SYSV), statically linked, for GNU/Linux 3.2.0,
```

It's a 32 MIPS bit ELF binary.

So, let's load it up on radare2, with the architecture set as MIPS with the `-a` flag.

```
➜ r2 -a mips word_generator
 -- V is for Visual
[0x004004d0]> 
```

After analyzing, using `aaa`, you'll be able to seek to `main` 
```
           0x00400a44      8f828130       lw v0, -sym.initialize(gp)    ; [0x4
           0x00400a48      0040c825       move t9, v0                          
           0x00400a4c      0411ff10       bal sym.initialize          ;[1]     
           0x00400a50      00000000       nop                                  
           0x00400a54      8fdc0010       lw gp, 0x10(fp)                      
           0x00400a58      8f828134       lw v0, -sym.challenge(gp)    
```

Where, we can see function calls to too functions `sym.initialize` and `sym.challenge`.

In `sym.challenge`,  firstly we can see English alphabets, a-z being imported.

```
0x004007b0      24434f58       addiu v1, v0, 0x4f58        ; 0x474f58 ;    
```
Then, we can see scanf() being called inside a loop that runs 0x11 times at `0x00400868`

Let's see what the format string is. That funcion argument would be in `a0` just before the function is called. 

```
│      ╎│   0x00400840      24444ee8       addiu a0, v0, 0x4ee8
```
And there you go:

```
:> izz~ %d
1335 0x000762bc 0x004762bc 10  11   .rodata         ascii   Arena %d:\n
```
So, we need to input 17 integers.

Below that, we can see 'Generated word:' being printed using printf() and another giant loop that runs for 17 times again.
And finally, a string is compared to 'thunderstricken'.

We can infer that the string is created inside the loop. So, let's closely study that.

And what's notable about that loop is that it has 3 XOR instructions

```
:> pdf~xor
│      ╎│   0x004008d0      00621026       xor v0, v1, v0
│      ╎│   0x00400904      00621026       xor v0, v1, v0
│      ╎│   0x00400914      00621826       xor v1, v1, v0
```
Upon examination, we can see that the result of that is used as an index to find a character from the array of alphabets.

The index created is the XOR-ed value of the first and second values XOR-ed together and the second and third values XOR-ed togerther, in a specefic iteration.

To put clearly,
> output[i]=alphabet[(input[i]^input[i+1])^(input[i]^input[i+2])]

Which breaks down to,
> output[i]=alphabet[(input[i])^(input[i+2])]

So, we just need to find some pairs of values with XOR-s into the index of the characters of the word 'thundestricken'

```
➜ qemu-mips word_generator
*****Welcome to the word generator*****

Enter the magic numbers :
7 14 20 9 0 4 3 0 18 18 1 3 9 1 3 5 14

Generated word : thunderstricken
Yaayy....you solved it
Now go get your reward from the server... :D
```

(Here, 7 ^ 20= 19, which is the index of 't'.)

