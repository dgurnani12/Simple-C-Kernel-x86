#include "keyboard.h"
#include "constants.h"
#include "idt.h"

// kbdus == keyboard-US:
extern unsigned char kbdus[128];

extern void keyboard_handler(void);

extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

// Location of cursor:
unsigned int cursor_location = 0;

// video memory begins at address 0xb8000:
char *vidptr = (char*)0xb8000;

// Interupt Descriptor Table (IDT):
struct IDT_entry IDT[IDT_SIZE];

void idt_init(void)
{
        unsigned long keyboard_address;
        unsigned long idt_address;
        unsigned long idt_ptr[2];

        // keyboard's interrupt descriptor table:
        keyboard_address = (unsigned long) keyboard_handler;
        IDT[0x21].offset_low = keyboard_address & 0xffff;
        IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
        IDT[0x21].zero = 0;
        IDT[0x21].type_attr = INTERRUPT_GATE;
        IDT[0x21].offset_high = (keyboard_address & 0xffff0000) >> 16;

        // Ports:
        /*         PIC1   PIC2
        * Command 0x20   0xA0
        * Data    0x21   0xA1
        */

        // ICW1 - begin initialization:
        write_port(0x20 , 0x11);
        write_port(0xA0 , 0x11);

        // ICW2 - remap offset address of IDT:
        /*
        * In x86 protected mode, we have to remap the PICs beyond 0x20 because
        * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
        */
        write_port(0x21 , 0x20);
        write_port(0xA1 , 0x28);

        // ICW3 - setup cascading:
        write_port(0x21 , 0x00);
        write_port(0xA1 , 0x00);

	// ICW4 - environment info:
        write_port(0x21 , 0x01);
        write_port(0xA1 , 0x01);

        // mask interrupts
        write_port(0x21 , 0xff);
        write_port(0xA1 , 0xff);

        // fill the IDT descriptor:
        idt_address = (unsigned long)IDT ;
        idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
        idt_ptr[1] = idt_address >> 16 ;

        load_idt(idt_ptr);
}

void kb_init(void)
{
	// 0xFD is 11111101 - enables only IRQ1 (keyboard)
	write_port(0x21 , 0xFD);
}

void kprint(const char *str)
{
	unsigned int i = 0;
	
	while (str[i] != '\0') {
		vidptr[cursor_location++] = str[i++];
		vidptr[cursor_location++] = 0x02;
	}
	
	// the last iteration shouldn't get incremented
	cursor_location--;
}

void kprint_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	unsigned int line_remainder = line_size - (cursor_location % line_size);
	
	cursor_location = cursor_location + line_remainder;
}

void clear_screen(void)
{
	unsigned int i = 0;

	while (i < SCREENSIZE) {
		vidptr[i++] = ' ';
		vidptr[i++] = 0x02;
	}
}

void keyboard_handler_main(void)
{
	unsigned char status;
	char keycode;

	// write EOI
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	
	// Lowest bit f status will be set if buffer is not empty
	if (status & 0x01) {
		
		keycode = read_port(KEYBOARD_DATA_PORT);
		
		if(keycode < 0) {
			return;
		}

		if(keycode == ENTER_KEY_CODE) {
			kprint_newline();
			return;
		}

		vidptr[cursor_location++] = kbdus[(unsigned char) keycode];
		vidptr[cursor_location++] = 0x0f;
	}
}

void kmain(void)
{
	const char *greeting = "### This kernel echos from the Keyboard ###";
	clear_screen();
	kprint(greeting);
	kprint_newline();
	kprint_newline();

	idt_init();
	kb_init();

	while(1);
}
