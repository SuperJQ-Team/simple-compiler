#include <iostream>

#include "Logger.h"

void Logger::log(const std::string& content)
{
	std::cout << content << '\n';
}