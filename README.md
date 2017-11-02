# Simple C kernel x86
A x86 simple kernel that echos input from keyboard

## Build and Run:
### Grub:
  1) Build the kernel:
    $    ./build
  2) Move the kernel to /Boot. (also better to rename kernel to kernel-###); where the ### is a number between 100-200 
  3) Run the commands from "commands.grub" in a grub shell.

### qemu:
1) Build the kernel:
    $    ./build
2) Run qemu:
    $     qemu-system-i386 -kernel kernel

## Screenshots:
<p align="center">
  <img src="https://github.com/dgurnani12/Simple-C-Kernel-x86/blob/master/Screenshot.png"/>
</p>
