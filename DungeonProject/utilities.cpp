#include "utilities.h"

#include <string>
#include <iostream>

void errorMessage(std::string error, int line, std::string file)
{
    std::cerr << "Error on line " << line << " in the file " << file << "\n- " << error;
}