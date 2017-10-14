### configure path before running this script as root ###
# ln -s <ENTER_PATH_HERE> ~/PathToKernel;

cd ~/PathToKernel;

### building the kernel ### 
nasm -f elf32 kernel.asm -o kasm.o;
gcc -m32 -c kernel.c -o kc.o;
ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o;
