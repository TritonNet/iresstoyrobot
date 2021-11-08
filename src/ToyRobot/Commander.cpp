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

#include "Commander.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

CommanderBase::CommanderBase(ToyRobot& robot, LoggerBase& logger)
    : m_robot(robot),
    m_logger(logger)
{
    for (const auto& entry : m_facingDirections)
        m_facingDirectionStrings.emplace(entry.second, entry.first);
}

void CommanderBase::Launch()
{
    m_logger.Info("Toy robot starting..");

    auto cmd = cmdUNKNOWN;
    while (cmd != cmdEXIT)
    {
        std::vector<std::string> args;

        cmd = GetCommand(args);
        switch (cmd)
        {
        case cmdPLACE:
            Place(args);
            break;
        case cmdMOVE:
            Move();
            break;
        case cmdTURN_LEFT:
        {
            const auto sucess = m_robot.TryTurnLeft();
            if (!sucess)
                m_logger.Error("Turn left failed.");
            break;
        }
        case cmdTURN_RIGHT:
        {
            const auto sucess = m_robot.TryTurnRight();
            if (!sucess)
                m_logger.Error("Turn right failed.");
            break;
        }
        case cmdREPORT:
            Report();
            break;
        case cmdEXIT:
            continue;
            break;
        case cmdUNKNOWN:
        default:
            m_logger.Error("Unknown command");
            break;
        }
    }

    m_logger.Info("Toy robot quitting..");
}

void CommanderBase::Place(std::vector<std::string>& xargs)
{
    if (xargs.size() != 1)
    {
        m_logger.Error("Invalid number of arguments for place command. Command expects 3 arguments in the form of (place x,y,direction)");
        return;
    }

    const auto args = Split(xargs[0], ",");

    if (args.size() != 3)
    {
        m_logger.Error("Invalid number of arguments for place command. Command expects 3 arguments in the form of (place  x,y,direction)");
        return;
    }

    int x = 0;
    if (!TryParseInt(args[0], x))
    {
        m_logger.Error("Invalid x value: " + args[0]);
        return;
    }

    int y = 0;
    if (!TryParseInt(args[1], y))
    {
        m_logger.Error("Invalid y value : " + args[1]);
        return;
    }

    const auto facingDirectionItr = m_facingDirections.find(ToLower(args[2]));
    if (facingDirectionItr == m_facingDirections.end())
    {
        m_logger.Error("Invalid facing direction: " + args[2]);
        return;
    }

    const auto sucess = m_robot.TryPlace(x, y, facingDirectionItr->second);
    if (!sucess)
    {
        m_logger.Error("Placement failed.");
        return;
    }
}

void CommanderBase::Move()
{
    const auto sucess = m_robot.TryMove();
    if (!sucess)
        m_logger.Error("Move failed.");
}

void CommanderBase::Report()
{
    uint8_t x;
    uint8_t y;
    FacingDirection facingDirection;

    m_robot.Report(x, y, facingDirection);
    m_logger.Info("Output: " + std::to_string(x) + "," + std::to_string(y) + "," + ToUpper(m_facingDirectionStrings[facingDirection]));
}

std::vector<std::string> CommanderBase::Split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t prev = 0;
    size_t pos = 0;
    do
    {
        pos = str.find(delimiter, prev);
        if (pos == std::string::npos)
            pos = str.length();

        std::string token = str.substr(prev, pos - prev);
        if (!token.empty())
            tokens.push_back(token);

        prev = pos + delimiter.length();

    } while (pos < str.length() && prev < str.length());

    return tokens;
}

std::string CommanderBase::ToLower(std::string str)
{
    std::string strlower = str;
    std::for_each(strlower.begin(), strlower.end(), [](char& c) {
        c = ::tolower(c);
    });

    return strlower;
}

std::string CommanderBase::ToUpper(std::string str)
{
    std::string strupper = str;
    std::for_each(strupper.begin(), strupper.end(), [](char& c) {
        c = ::toupper(c);
        });

    return strupper;
}

bool CommanderBase::TryParseInt(std::string str, int& num)
{
    char* end;
    num = strtol(str.c_str(), &end, 10);
    return *end == '\0';
}

Command CommanderBase::GetCommand(std::vector<std::string>& args)
{
    m_logger.Info("Please enter command : ", "");

    std::string input;
    if (!TryReadLine(input))
        return cmdEXIT;

    const auto cmdArgs = Split(input, " ");
    if (cmdArgs.size() == 0)
        return cmdUNKNOWN;

    const auto commandItr = m_commands.find(ToLower(cmdArgs[0]));
    if (commandItr == m_commands.end())
        return cmdUNKNOWN;

    for (size_t idx = 1; idx < cmdArgs.size(); idx++)
        args.push_back(cmdArgs[idx]);

    return commandItr->second;
}

bool ConsoleCommander::TryReadLine(std::string& input)
{
    std::getline(std::cin, input);

    return true;
}

bool FileCommander::TryReadLine(std::string& input)
{
    if (!m_filestream.is_open())
        return false;

    if (!std::getline(m_filestream, input))
        return false;

    return true;
}