# Simple C kernel x86
A x86 simple kernel that echos input from keyboard

## Build and Run:
### Grub:
  1) Build the kernel:
    $    ./build
  2) Move the kernel to /Boot (also better to rename kernel to kernel-<a number lower then linux kernel>)
  3) Run the commands from "commands.grub" in grub shell.

### qemu:
1) Build the kernel:
    $    ./build
2) Run qemu:
    $     qemu-system-i386 -kernel kernel
