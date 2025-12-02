# uboot-bootelf-interactive
ARM64 ELF file that scans u-boot memory for cli_loop function signature and jumps there. For devices with "rabbit hole" protection and without "go" command.  
cli_loop signature:  
FD 7B BF A9 FD 03 00 91 ? ? ? 97 00 00 00 14  
Signature checked on Yandex TV Station (magritte), Yandex Module 2, Yandex Station Max.

Compile with:  
aarch64-linux-gnu-gcc -Os -nostdlib -ffreestanding -fno-stack-protector -c ./cli_loop.c -o ./cli_loop.o  
aarch64-linux-gnu-ld -Ttext=0x02000000 --entry=_start --no-dynamic-linker -nostdlib --gc-sections -o cli_loop.elf cli_loop.o

Run:  
fatload usb 0 0x01ff0000 cli_loop.elf   
bootelf -p 0x01ff0000   
(bootelf without parameters should also work)  

Works on devices with "fatload" and "bootelf" commands available. I use it on Yandex TV Station (magritte) with Amlogic T3 CPU.
