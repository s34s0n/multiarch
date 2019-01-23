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
### 2. Emulate the binary in Qemu virtual environment
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

`




