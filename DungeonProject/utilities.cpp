#include "utilities.h"

#include <Windows.h>
#include <string>
#include <iostream>

void errorMessage(std::string error, int line, std::string file)
{
    std::cerr << "Error on line " << line << " in the file " << file << "\n- " << error;
    Sleep(10000);
    exit(1);
}

int getColor(int letterColor, int backgroundColor, int numberOfColors)
{
    return (letterColor + (backgroundColor + numberOfColors));
}