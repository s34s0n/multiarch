# MIPS
**MIPS**(Microprocessor without Interlocked Pipelined Stages) is RICS instruction set architecture developed by MIPS Technologies<br/>
MIPS processors are used mainly in embedded systems like residential gateways and routers<br/>
We will be mainly looking into MIPS 32-bit architecture

### Emulating MIPS Images
The images directory has already complied binaries with the 4.9.0.8 Malta kernel, which running Debian 9 (stretch)
`
qemu-system-mips -M malta \
            -m 512 -hda hda.img \
            -kernel vmlinux-4.9.0-8-4kc-malta \
            -initrd initrd.img-4.9.0-8-4kc-malta \
            -append "root=/dev/sda1 console=ttyS0 nokaslr" \
            -nographic \
            -nic user,hostfwd=tcp::5555-:22
`

You can then ssh via your host machine using.
`
ssh -p 5555 multiarch@localhost
`
