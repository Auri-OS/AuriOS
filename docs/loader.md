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
 
a