#include "colorstring.h"
#include "utilities.h"

#include <string>
#include <vector>

//---------------------------------------------------------------------
// ColorChar functions

ColorChar::ColorChar()
{
    character = ' ';
    color = getColor(dngutil::LIGHTGRAY, dngutil::BLACK);
}

ColorChar::ColorChar(char character, int color)
{
    this->character = character;
    this->color = color;
}

bool ColorChar::operator==(const ColorChar& other)
{
    return (character == other.character && color == other.color);
}

bool ColorChar::operator!=(const ColorChar& other)
{
    return !(*this == other);
}

//---------------------------------------------------------------------

//---------------------------------------------------------------------
// ColorString functions

ColorString::ColorString(std::string str, int color)
{
    reserve(str.size());
    for (auto it = str.begin(); it != str.end(); it++)
    {
        push_back(ColorChar(*it, color));
    }
}

ColorString::ColorString(std::string str, std::vector<int> colors)
{
    if (str.size() != colors.size())
    {
        errorMessage("String size does not match vector size. String is \"" + str + "\".", __LINE__, __FILE__);
    }

    reserve(str.size());
    for (unsigned int i = 0; i < str.size(); i++)
    {
        push_back(ColorChar(str[i], colors[i]));
    }
}

std::string ColorString::getString()
{
    std::string str;

    str.reserve(size());
    for (auto it = begin(); it != end(); it++)
    {
        str += it->character;
    }

    return str;
}

ColorString ColorString::operator+(const ColorString& other)
{
    ColorString colstr(*this);

    colstr.reserve(size() + other.size());
    for (auto it = other.begin(); it != other.end(); it++)
    {
        colstr.push_back(*it);
    }

    return colstr;
}

ColorString ColorString::operator+(const ColorChar& other)
{
    ColorString colstr(*this);
    colstr.push_back(other);
    return colstr;
}


void ColorString::operator+=(const ColorString& other)
{
    reserve(size() + other.size());
    for (auto it = other.begin(); it != other.end(); it++)
    {
        push_back(*it);
    }
}

void ColorString::operator+=(const ColorChar& other)
{
    push_back(other);
}

void ColorString::setColor(int color)
{
    for (auto it = begin(); it != end(); it++)
    {
        it->color = color;
    }
}

//---------------------------------------------------------------------