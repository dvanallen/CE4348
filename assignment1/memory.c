/**
 * memory.c
 **/

 #include <stdio.h>
 #include <string.h> //memset

int readMemoryFile(const char* fileName, int* array, int startIndex) {
    FILE *memoryFile = fopen(fileName, "r");
    char buff[100];
    int memsize = 0;
    while (!feof(memoryFile)) {
    	int command;
    	fgets(buff, 1000, memoryFile); // read line to buffer
    	sscanf(buff, "%d", &command); // read int from buffer
    	array[startIndex + memsize++] = command; // store in array pointer
    }
    fclose(memoryFile);
    return memsize;
}

int main(int argc, const char* argv[]) {
    int memory[2000];
    int programLength;
    int interruptLength;

    memset(memory, -1, sizeof(memory));

    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Expected 2 or 3 arguments. Recieved %d argument(s).\n", argc);
        return -1;
    }

    programLength = readMemoryFile(argv[1], memory, 0); // load the mandatory program file
    if (argc == 3) { // if we have an interrupt file, load it too.
    	interruptLength = readMemoryFile(argv[2], memory, 1000);
    }
    return 0;
}