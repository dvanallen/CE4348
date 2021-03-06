/**
*	cpu.cpp
*	Authors:	Daniel Van Allen, Ben Privat, Desmond Lee
*	Class:		CE4348.001 - OS Concepts
*	Date:		2013-9-23
*	Description:	This program emulates a CPU by providing an instruction set that is used to manipulate data stored in memory.cpp
**/

#include <unistd.h>
#include <cstdlib>
#include <iostream>

//Registers:
int PC = 0, IR = 0, AC = 0, X = 0, Y = 0, SP = 999;
//Pipes:
int pipeIn[2], pipeOut[2];

//Memory I/O
int getFromAddress(int addr) {
	std::cout << addr << std::endl;
	int value = 0;
	std::cin >> value;
    return value;
}

void putToAddress(int addr, int value) {
    std::cout << addr << value << std::endl;
}

//Instruction set: Instructions return true if they are executed successfully and false if they fail.
//1
bool loadValue(int value) {
	AC = value;
	return true;
}

//2
bool loadAddr(int addr) {
	int value = getFromAddress(addr);
	AC = value;
	return true;
}

//3
bool storeAddr(int addr) {
	putToAddress(addr, AC);
	return true;
}

//4
bool addX() {
	AC += X;
	return true;
}

//5
bool addY() {
	AC += Y;
	return true;
}

//6
bool subX() {
	AC -= X;
	return true;
}

//7
bool subY() {
	AC -= Y;
	return true;
}

//8
bool get() {
	AC = rand() % 100 + 1;
	return true;
}

//9
/*bool put() {
	
}

//10
bool copyToX() {

}

//11
bool copyToY() {

}

//12
bool copyFromX() {

}

//13
bool copyFromY() {

}

//14
bool jumpAddr() {

}

//15
bool jumpIfEqualAddr() {

}

//16
bool jumpIfNotEqual() {

}

//17
bool callAddr() {

}

//18
bool ret() {

}

//19
bool incX() {

}

//20
bool decX() {

}

//21
bool loadIndXAddr() {

}

//22
bool loadIndYAddr() {


}

//23
bool push() {

}

//24
bool pop() {

}

//25
bool int() {

}

//26
bool iRet() {

}

//50
void end() {

}*/

int main(int argc, char* argv[]) {
    if(pipe(pipeIn) < 0 || pipe(pipeOut) < 0)
    {
        std::cout << "Error: Pipe failed." << std::endl;
        return 0;
    }

	switch(fork())
    {
        case -1: 
            std::cout << "Error: Fork failed." << std::endl;
            return 0;
            
        case 0:
            dup2(pipeIn[0], STDIN_FILENO);
            dup2(pipeOut[1], STDOUT_FILENO);
            close(pipeIn[0]);
            close(pipeIn[1]);
            close(pipeOut[0]);
            close(pipeOut[1]);
            execl("memory", "memory", argv, (char *)0);
            //If we make it through
            std::cout << "Exec failed." << std::endl;
            return 0;

        default: break;
    }

    close(pipe1[0]); // close unneeded pipes
    close(pipe2[1]);

	return 0;
}
