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

#pragma once

#include <string>

class LoggerBase
{
public:
    void Error(std::string msg, std::string end = "\n")
    { 
        Print("ERROR", msg, end);
    }

    void Info(std::string msg, std::string end = "\n")
    {
        Print("INFO", msg, end);
    }

    void Warn(std::string msg, std::string end = "\n")
    {
        Print("WARN", msg, end);
    }

protected:
    void virtual Print(std::string msgType, std::string msg, std::string end) = 0;
    std::string FormatLogMsg(std::string msgType, std::string msg);
};

class ConsoleLogger : public LoggerBase
{
protected:
    void Print(std::string msgType, std::string msg, std::string end) override;
};

class FileLogger : public LoggerBase
{
public:
    FileLogger(std::string path) :
        m_path(path)
    {}

protected:
    void Print(std::string msgType, std::string msg, std::string end) override;

private:
    const std::string m_path;
};