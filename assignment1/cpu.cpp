/**
* cpu.cpp
*	Authors:			Daniel Van Allen, Ben Privat, Desmond Lee
*	Class:				CE4348.001 - OS Concepts
*	Date:				2013-9-23
*	Description: 		This program emulates a CPU by providing an instruction set that is used to manipulate data stored in memory.cpp
**/

#include "memory.h"
#include <cstdlib>

//Registers:
int PC = IR = AC = X = Y = 0;
int SP = 999;


//Instruction set: Instructions return 0 if they are executed successfully and 1 if they fail.

//1
bool loadValue(int value) {
	if(!value)
		return false;
	AC = value;
	return true;
}

//2
bool loadAddr(int addr) {
	if(!addr)
		return false;
	int value = memory.load(addr)
	if(!value)
		return false;
	AC = value;
	return true;
}

//3
bool storeAddr(int addr) {
	if(!addr)
		return false;
	memory.store(addr, AC);
	return true;
}

//4
bool addX() {
	if(!X)
		return false;
	AC += X;
	return true;
}

//5
bool addY() {
	if(!Y)
		return false;
	AC += Y;
	return true;
}

//6
bool subX() {
	if(!X)
		return false;
	AC -= X;
	return true;
}

//7
bool subY() {
	if(!Y)
		return false;
	AC -= Y;
	return true;
}

//8
bool get() {
	AC = rand() % 100 + 1;
	return true;
}

//9
bool put() {
	
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

}


















int main {
	//TODO: Read values from memory and execute them using the appropriate instructions
	
	return 0;
}