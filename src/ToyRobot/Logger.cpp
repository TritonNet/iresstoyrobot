/*
 *	The MIT License (MIT)
 *	Copyright © 2021 Kushan Fernando (info@kushan.me)
 *
 *	Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the “Software”),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 *  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 *  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

std::string LoggerBase::FormatLogMsg(std::string msgType, std::string msg)
{
	std::stringstream sstream;
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	sstream
		<< std::put_time(&tm, "%d-%m-%Y %H-%M-%S")
		<< " - "
		<< msgType
		<< " - "
		<< msg;

	return sstream.str();
}


void ConsoleLogger::Print(std::string msgType, std::string msg, std::string end)
{
	std::cout << FormatLogMsg(msgType, msg) << end;
}

void FileLogger::Print(std::string msgType, std::string msg, std::string end)
{
	std::ofstream myfile;
	myfile.open(m_path, std::ios_base::app);
	myfile << FormatLogMsg(msgType, msg) << end;
	myfile.close();
}