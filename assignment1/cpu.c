#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//Registers:
int PC = 0, IR = 0, AC = 0, X = 0, Y = 0, SP = 999;
//Pipes:
int pipe1[2], pipe2[2];

FILE* to_child;
FILE* from_child;

char buff[BUFSIZ]; // read buffer 

int getFromAddress(int);
void putToAddress(int, int);

int main(int argc, char* argv[]) {
    if(pipe(pipe1) < 0 || pipe(pipe2) < 0)
    {
        printf("Pipe failed\n");
        return 0;
    }

	switch(fork())
    {
        case -1: 
            printf("Fork failed\n");
            return 0;
            
        case 0:
            dup2(pipe1[0], STDIN_FILENO);
            dup2(pipe2[1], STDOUT_FILENO);
            close(pipe1[0]); // close unneeded pipes
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);
            execv("memory", argv);
            //If we make it through
            printf("Exec failed\n");

            printf("The error is %d\n", errno);
            return 0;

        default: break;
    }

    // cpu process
    close(pipe1[0]); // close unneeded pipes
    close(pipe2[1]);

    to_child = fdopen(pipe1[1], "w");
    from_child = fdopen(pipe2[0], "r");
    setbuf(to_child, NULL);  // because buffers suck.
    setbuf(from_child, NULL);

    memset(buff, 0, sizeof(buff)); // clear buffer

    printf("starting cpu...\n");
    int value = getFromAddress(10);
    printf("value at addr 10 is: %d\n", value);
    printf("Now writing 1000 to address 10.\n");
    putToAddress(10, 1000);
    value = getFromAddress(10);
    printf("value at addr 10 is now: %d\n", value);

	return 0;
}

//Memory I/O
int getFromAddress(int addr) {
    int value = 0;
    //printf("sending.\n");
    fprintf(to_child, "0 %d\n", addr);
    //printf("waiting.\n");
    fscanf(from_child, "%d", &value);
    return value;
}

void putToAddress(int addr, int value) {
    fprintf(to_child, "1 %d %d\n", addr, value);
    return;
}