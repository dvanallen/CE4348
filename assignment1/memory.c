/**
 * memory.c
 **/

#include <stdio.h>
#include <string.h> //memset

#define MEMSIZE 2000
#define INTLOC 1000

int readMemoryFile(const char* fileName, int* array, int startIndex) {
    FILE *memoryFile = fopen(fileName, "r");
    char buff[100];
    int memsize = 0;
    while (!feof(memoryFile)) {
    	int command;
    	fgets(buff, INTLOC, memoryFile); // read line to buffer
    	sscanf(buff, "%d", &command); // read int from buffer
    	array[startIndex + memsize++] = command; // store in array pointer
    }
    fclose(memoryFile);
    return memsize;
}

int main(int argc, const char* argv[]) {
    int memory[MEMSIZE];
    int programLength;
    int interruptLength;
    int continueRead = 1;

    setbuf(stdout, NULL);  // because buffers suck.
    setbuf(stdin, NULL);

    memset(memory, -1, sizeof(memory));

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Expected 2 or 3 arguments. Recieved %d argument(s).\n", argc);
        return -1;
    }

    programLength = readMemoryFile(argv[1], memory, 0); // load the mandatory program file
    if (argc == 3) { // if we have an interrupt file, load it too.
    	interruptLength = readMemoryFile(argv[2], memory, INTLOC);
    }


    while (continueRead) {
    	int command;
    	int address;
    	int data;
    	scanf("%d", &command); // read command
    	//printf("got command %d\n", command);


    	if (command == -1) {
    		continueRead = 0;
    		break;
    	}

	    scanf("%d", &address);
    	//printf("got address %d\n", address);
		if (address >= MEMSIZE || 
		address >= INTLOC + interruptLength ||
		address < 0) {
			fprintf(stderr, "memory[%d] = ERR\n", address);
			fprintf(stderr, "out of range error!!!!!!");
			return -1;
		}

    	switch(command) {
    		case 0:
	    		data = memory[address];
    			//fprintf(stderr, "memory[%d] = %d\n", address, data);
	    		printf("%d\n", data);
	    		break;
	    	case 1:
	    		scanf("%d", &data); //read data
    			//printf("got data %d\n", data);
    			memory[address] = data;
    			//fprintf(stderr, "setting memory[%d] = %d\n", address, data);
	    		break;
	    	default:
	    		fprintf(stderr, "unrecognized command error!!!!");
	    		return -1;
    	}
    }
    return 0;
}