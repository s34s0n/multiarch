# Tools
## QEMU
QEMU is a generic and open source machine emulator and virtualizer.When used as a virtualizer, QEMU achieves near native performance by executing the guest code directly on the host CPU. QEMU supports virtualization when executing under the Xen hypervisor or using the KVM kernel module in Linux. When using KVM, QEMU can virtualize x86, server and embedded PowerPC, 64-bit POWER, S390, 32-bit and 64-bit ARM, and MIPS guests. 
To get started you'll need to do the following:
```console
$ sudo apt-get install qemu-user-static
```
## Binwalk
Once you have qemu-user-static installed you'll next want to install [Binwalk](https://github.com/ReFirmLabs/binwalk). Make sure to follow instructions so you have all the dependancies.
## Buildroot
Buildroot is a simple, efficient and easy-to-use tool to generate embedded Linux systems through cross-compilation.Once Binwalk is installed you will also want to download the uClibc **Buildroot** tar file. This is crucial since this is going to be your bread and butter for cross compiling and also debugging, especially if you don't have IDA. To get started do the following:
```console
$ wget https://buildroot.org/downloads/buildroot-20xx.xx.x.tar.gz
$ tar xzf buildroot-20xx.xx.x.tar.gz
$ cd buildroot-20xx.xx.x/
```
Now you're going to type make menuconfig and when it's done you're going to see the following:
```console
$ make menuconfig
```

![wire](https://github.com/s34s0n/multiarch/blob/master/mips/images/1.png)

Under Target you're going to select MIPS little endian, ELF, and mips32.

![wire](https://github.com/s34s0n/multiarch/blob/master/mips/images/2.png)

Under Toolkit you'll want to set the C Library to uClibc since the binary is compiled with this library and most devices you'll come across will be using this C library.<br/>
Also under Toolkit you'll want to enable "Build cross gdb for the host." This will create a gdb binary that will run on your host but will support your target Architecture (e.g. MIPS). This is helpful for debugging applications when using the -g argument in Qemu.

![wire](https://github.com/s34s0n/multiarch/blob/master/mips/images/3.png)

Make sure to save your configuration changes so that your toolkit will compile for the right Architecture we've chosen.

![wire](https://github.com/s34s0n/multiarch/blob/master/mips/images/4.png)

Now feel free to either explore what other options the toolkit can provide or exit the menu and type "make" but be warned that this process does take a while and also make sure you have an internet connection since this process will download tar files that are needed for compiling.

```console
$ make
```
Once the toolkit has finished compiling you'll see a new folder called **output**. This is where the toolkit compiled, but we'll only care about what's been compiled into the host folder within output.
```console
➜  buildroot-2018.02.9 ls
arch   boot   CHANGES    Config.in.legacy  COPYING     dl    fs     Makefile         output   README   system     utils
board  build  Config.in  configs           DEVELOPERS  docs  linux  Makefile.legacy  package  support  toolchain
➜  buildroot-2018.02.9 cd output 
➜  output ls
build  host  images  staging  target
➜  output cd host/usr 
➜  usr ls
bin  etc  include  lib  lib64  libexec  mipsel-buildroot-linux-uclibc  share  usr
➜  usr cd bin 
➜  bin ls *-build*
mipsel-buildroot-linux-uclibc-addr2line          mipsel-buildroot-linux-uclibc-gcc-ar       mipsel-buildroot-linux-uclibc-ldd
mipsel-buildroot-linux-uclibc-ar                 mipsel-buildroot-linux-uclibc-gcc.br_real  mipsel-buildroot-linux-uclibc-nm
mipsel-buildroot-linux-uclibc-as                 mipsel-buildroot-linux-uclibc-gcc-nm       mipsel-buildroot-linux-uclibc-objcopy
mipsel-buildroot-linux-uclibc-cc                 mipsel-buildroot-linux-uclibc-gcc-ranlib   mipsel-buildroot-linux-uclibc-objdump
mipsel-buildroot-linux-uclibc-cc.br_real         mipsel-buildroot-linux-uclibc-gcov         mipsel-buildroot-linux-uclibc-ranlib
mipsel-buildroot-linux-uclibc-c++filt            mipsel-buildroot-linux-uclibc-gcov-dump    mipsel-buildroot-linux-uclibc-readelf
mipsel-buildroot-linux-uclibc-cpp                mipsel-buildroot-linux-uclibc-gcov-tool    mipsel-buildroot-linux-uclibc-run
mipsel-buildroot-linux-uclibc-cpp.br_real        mipsel-buildroot-linux-uclibc-gdb          mipsel-buildroot-linux-uclibc-size
mipsel-buildroot-linux-uclibc-elfedit            mipsel-buildroot-linux-uclibc-gprof        mipsel-buildroot-linux-uclibc-strings
mipsel-buildroot-linux-uclibc-gcc                mipsel-buildroot-linux-uclibc-ld           mipsel-buildroot-linux-uclibc-strip
mipsel-buildroot-linux-uclibc-gcc-6.4.0          mipsel-buildroot-linux-uclibc-ld.bfd
mipsel-buildroot-linux-uclibc-gcc-6.4.0.br_real  mipsel-buildroot-linux-uclibc-ldconfig
```
You can see that we have cross compilers and an instance of gdb. We will use this for crafting our exploits. 
## gdb-multiarch
The GNU Debugger for all gdb supported architectures (i386/arm/mips...).
```console
$ sudo apt-get install gdb-multiarch
```
## radare2
[radare2](https://github.com/radare/radare2) is a unix-like reverse engineering framework and commandline tools security.
## IDA
[IDA](https://www.hex-rays.com/products/ida/) is a Windows, Linux or Mac OS X hosted multi-processor disassembler and debugger.

