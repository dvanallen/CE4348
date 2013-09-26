#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SYSTEM 1000
#define USER 0

//Registers:
int PC = 0, IR = 0, AC = 0, X = 0, Y = 0, SP = 999, MODE = USER;
//Pipes:
int pipe1[2], pipe2[2];

FILE* to_child;
FILE* from_child;

char buff[BUFSIZ]; // read buffer 

int getFromAddress(int);
void putToAddress(int, int);
void pushStack(int);
int popStack();
void handleInstruction(int, int*, int*);

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

    int keepGoing = 1;
    int incrementPC = 1;
    while(keepGoing) {
        //printf("reading from PC %d\n", PC);
        IR = getFromAddress(PC);
        //printf("got instruction %d\n", IR);
        handleInstruction(IR, &keepGoing, &incrementPC);
        if (incrementPC) {
            PC++;
        }
        else {
            incrementPC = 1;
        }
    }

    /**
    printf("starting cpu...\n");
    int value = getFromAddress(10);
    printf("value at addr 10 is: %d\n", value);
    printf("Now writing 1000 to address 10.\n");
    putToAddress(10, 1000);
    value = getFromAddress(10);
    printf("value at addr 10 is now: %d\n", value);
    **/

	return 0;
}

//Memory I/O
int getFromAddress(int addr) {
    int value = 0;
    //printf("sending.\n");
    fprintf(to_child, "0 %d\n", MODE + addr);
    //printf("waiting.\n");
    fscanf(from_child, "%d", &value);
    return value;
}

void putToAddress(int addr, int value) {
    fprintf(to_child, "1 %d %d\n", MODE + addr, value);
    return;
}

void handleInstruction(int inst, int *keepGoing, int *incrementPC) {
    switch(inst) {
        int value;
        int address;
        case 1: // load value into AC
            PC = PC + 1;
            AC = getFromAddress(PC);
            break;
        case 2: // load address value into AC
            PC = PC + 1;
            address = getFromAddress(PC);
            AC = getFromAddress(address);
            break;
        case 3: // store value in AC to address
            PC = PC + 1;
            address = getFromAddress(PC);
            putToAddress(address, AC);
            break;
        case 4: // add value in X to AC
            AC += X;
            break;
        case 5: // add value in Y to AC
            AC += Y;
            break;
        case 6: 
            AC -= X;
            break;
        case 7:
            AC -= Y;
            break;
        case 8:
            // get random int from 1 to 100
            AC = 10; // implement later.
            break;
        case 9:
            PC = PC + 1;
            value = getFromAddress(PC);
            if (value == 1) {
                printf("%d", AC);
            }
            else if (value == 2) {
                printf("%c", (char) AC);
            }
            break;
        case 10:
            X = AC;
            break;
        case 11:
            Y = AC;
            break;
        case 12:
            AC = X;
            break;
        case 13:
            AC = Y;
            break;
        case 14:
            PC+=1;
            address = getFromAddress(PC);
            PC = address;
            *incrementPC = 0;
            break;
        case 15:
            PC+=1;
            address = getFromAddress(PC);
            if (AC == 0) {
                PC = address;
                *incrementPC = 0;
            }
            break;
        case 16:
            PC+=1;
            address = getFromAddress(PC);
            if (AC != 0) {
                PC = address;
                *incrementPC = 0;
            }
            break;
        case 17:
            PC+=1;
            address = getFromAddress(PC);
            pushStack(PC+1);
            PC = address;
            *incrementPC = 0;
            break;
        case 18:
            address = popStack();
            PC = address;
            *incrementPC = 0;
            break;
        case 19:
            X++;
            break;
        case 20:
            X--;
            break;
        case 21:
            address = getFromAddress(++PC);
            AC = getFromAddress(address + X);
            break;
        case 22:
            address = getFromAddress(++PC);
            AC = getFromAddress(address + Y);
            break;
        case 23:
            pushStack(AC);
            break;
        case 24:
            AC = popStack();
            break;
        case 25:
            pushStack(PC + 1);
            MODE = SYSTEM;
            PC = 0;
            *incrementPC = 0;
            break;
        case 26:
            PC = popStack();
            *incrementPC = 0;
            MODE = USER;
            break;
        case 50:
            *keepGoing = 0;
            break;
            // end exec;
    }
}

int popStack() {
    int value = 0;
    fprintf(to_child, "0 %d\n", ++SP);
    fscanf(from_child, "%d", &value);
    //printf("POPED %d FROM STACK. SP: %d\n", value, SP);
    return value;
}

void pushStack(int val) {
    fprintf(to_child, "1 %d %d\n", SP--, val);
    //printf("PUSHED %d TO STACK. SP: %d\n", val, SP);
}