#include "Debug.h"
#include <iostream>

void Debug::Log(std::string text)
{
	std::cout << text << std::endl;
}

void Debug::Log(float text)
{
	std::cout << text << std::endl;
}
