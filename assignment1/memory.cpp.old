/**
 * memory.cpp
 **/
 
#include <exception>
#include <fstream>
#include <string>
#include <iostream>
#include <limits>

//Parent Exception class
//Any exceptions we need will be subclasses of this
class memoryException: public std::exception
{
};

class argsException: public memoryException
{
    virtual const char* what() const throw()
    {
        return "Too few or too many arguments.";
    }
};

class filesizeException: public memoryException
{
    virtual const char* what() const throw()
    {
        return "File too large to read into memory.";
    }
};

class fileException: public memoryException
{
    virtual const char* what() const throw()
    {
        return "Cannot open program or interrupt file.";
    }
};

void loadFile(char *file, int *mem, int start, int max)
{
    std::ifstream in_file(file);
    if(!in_file.is_open())
        throw fileException();
        
    int index = start;
    while(in_file.good())
    {
        //Out of project=specified bounds
        if(index > max)
            throw filesizeException();
        in_file >> mem[index];
        //Ignore the rest (comments) until we hit the new line
        in_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        index++;
    }
    
    in_file.close();
}

int main(int argc, char* argv[])
{
    int mem[2000];
    
    try
    {
        //Need at least a program file, no more than a program and interrupt file
        if(argc < 2 || argc > 3)
            throw argsException();
        //Load program
        loadFile(argv[1], mem, 0, 999);
        //If we have an interupt file, load it, too
        if(argc == 3)
            loadFile(argv[2], mem, 1000, 1999);
    }
    catch (memoryException& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    for(int i = 0; i<10; i++)
    {
        std::cout << mem[i] << std::endl;
    }
    std::cout << std::endl << std::endl;
    for(int i = 1000; i<1010; i++)
    {
        std::cout << mem[i] << std::endl;
    }
    
    return 0;
}