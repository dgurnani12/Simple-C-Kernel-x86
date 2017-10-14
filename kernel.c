void kmain(void)
{
	const char *msgToDisplay = "Simple_C_Kernel -- Success!";
	
	// video memory begins at address 0xb8000
	char *ptrToVidMem = (char*) 0xb8000;
	
	// loop counters:
	unsigned int i = 0;
	unsigned int j = 0;

	// Clear the screen:
	// There are 25 lines, 80 columns per line, a single element takes 2 bytes each
	unsigned int screen = 80 * 25 * 2;
	
	while (j < screen) {

		// blank character
		ptrToVidMem[j] = ' ';

		//attribute-byte
		ptrToVidMem[j+1] = 0x07;

		// update loop variables:
		j += 2;

	}

	// reset loop counters:
	j = 0;

	// Write the string to video memory:
	while (msgToDisplay[j] != '\0') {
		
		ptrToVidMem[i] = msgToDisplay[j];

		// attribute byte -- give all characters black bg and green fg:
		ptrToVidMem[i+1] = 0x02;

		// update loop variables:
		j++;
		i += 2;
	}

	return;
}
