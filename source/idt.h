#define __IDT

struct IDT_entry {
        unsigned short int offset_low;
        unsigned short int selector;
        unsigned char zero;
        unsigned char type_attr;
        unsigned short int offset_high;
};
