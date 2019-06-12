# MIPS
**MIPS**(Microprocessor without Interlocked Pipelined Stages) is RICS instruction set architecture developed by MIPS Technologies<br/>
MIPS processors are used mainly in embedded systems like residential gateways and routers<br/>
We will be mainly looking into MIPS 32-bit architecture

## Emulating MIPS Images
The images directory has already complied binaries with the 4.9.0.8 Malta kernel, which running Debian 9 (stretch).

```bash
qemu-system-mips -M malta \
            -m 512 -hda hda.img \
            -kernel vmlinux-4.9.0-8-4kc-malta \
            -initrd initrd.img-4.9.0-8-4kc-malta \
            -append "root=/dev/sda1 console=ttyS0 nokaslr" \
            -nographic \
            -nic user,hostfwd=tcp::5555-:22 # Use this option for (QEMU version > 2.6.0) else -redir tcp:5555::22
`
            -nic user,hostfwd=tcp::5555-:22
```
You can then ssh via your host machine using.

```bash
ssh -p 5555 multiarch@localhost
```
## Materials
### mips

[MIPS IV Instruction Set](http://math-atlas.sourceforge.net/devel/assembly/mips-iv.pdf) [book]

[MIPS Calling Convention](https://courses.cs.washington.edu/courses/cse410/09sp/examples/MIPSCallingConventionsSummary.pdf) [article]

[EXPLOITING BUFFER OVERFLOWS ON MIPS ARCHITECTURES](https://www.vantagepoint.sg/papers/MIPS-BOF-LyonYang-PUBLIC-FINAL.pdf) [article]

[Exploiting a MIPS Stack Overflow](http://www.devttys0.com/2012/10/exploiting-a-mips-stack-overflow/) [article]

Notes:

1. `mips` has branch delay slot.

### mips64

[MIPS64 Architecture For Programmers Volume II: The MIPS64 Instruction Set](https://scc.ustc.edu.cn/zlsc/lxwycj/200910/W020100308600769158777.pdf) [book]

[Linux MIPS ELF reverse engineering tips](https://www.cr0.org/paper/mips.elf.external.resolution.txt) [article]

Notes:

1. `mips64` has branch delay slot.

2. Functions expect to be called through `$t9`.
