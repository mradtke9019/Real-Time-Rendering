#pragma once
#include <string>
#include <iostream>


enum LogLevel { Debug, Info, Warning, Error, Fatal };

class Log {
public:
	//static LogLevel Level;
	static void WriteLog(std::string message, LogLevel level)
	{
		LogLevel l = Info;
		if (level > l)
		{
			std::cout << message << std::endl;
		}
	}
};