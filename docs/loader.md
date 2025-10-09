10/07 : just create an basic loader to create an loader.o files craft with : 

```bash
nasm -f elf32 -o loader.s
```
so now, we have an loader.o file and an link.ld file (linker)

we use this 2 script to create an v0.1 of kernel 
compile like that 
```bash
    ld -T link.ld -melf_i386 loader.o -o kernel.elf
 ```
 
 So now we have an compiled file named (kernel.elf)


compile kernel.c to print basic Hello World 
```bash
gcc -m32 -ffreestanding -c kernel.c -o kernel.o
```
now move kernel.o to boot/
and do that : 
```bash
ld -m elf_i386 -T link.ld -o kernel.bin loader.o kernel.o 
```
(not finished )
