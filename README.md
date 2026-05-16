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

Or on new versions with UBIFS:   
ubi part data  
ubifsmount ubi0  
ubifsload ${loadaddr} cli_loop.elf  
bootelf  

Works on devices with "fatload"/"ubifsload" and "bootelf" commands available. I use it on Yandex TV Station (magritte) with Amlogic T3 CPU and on some other models.

# For devices whith GO command in u-boot
Run the following u-boot commands:  
mw.l 0x2000000 0xD10043FF;mw.l 0x2000004 0xAA1E03E0;mw.l 0x2000008 0x90000001;mw.l 0x200000c 0x91017021;mw.l 0x2000010 0x92689C00;mw.l 0x2000014 0xA9400C22;mw.l 0x2000018 0xA9000FE2;mw.l 0x200001c 0x910003E3;mw.l 0x2000020 0xD2800001;mw.l 0x2000024 0x38636822;mw.l 0x2000028 0x7103FC5F;mw.l 0x200002c 0x54000080;mw.l 0x2000030 0x38616804;mw.l 0x2000034 0x6B02009F;mw.l 0x2000038 0x540000E1;mw.l 0x200003c 0x91000421;mw.l 0x2000040 0xF100403F;mw.l 0x2000044 0x54FFFF01;mw.l 0x2000048 0xAA0003F0;mw.l 0x200004c 0x910043FF;mw.l 0x2000050 0xD61F0200;mw.l 0x2000054 0x91000400;mw.l 0x2000058 0x17FFFFF2;mw.l 0x200005c 0xA9BF7BFD;mw.l 0x2000060 0x910003FD;mw.l 0x2000064 0x97FFFFFF;mw.l 0x2000068 0x14000000;go 0x2000000;

It constructs the program in RAM at 0x2000000 and executes it. Code should be position-independed, but loading on different addresses was never tested.
