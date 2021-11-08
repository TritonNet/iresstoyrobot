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
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "ToyRobot.h"
#include "Commands.h"
#include <iostream>
#include "Logger.h"

class CommanderBase
{
public:
    CommanderBase(ToyRobot& robot, LoggerBase& logger);

    void Launch();

protected:
    Command virtual GetCommand(std::vector<std::string>& args) = 0;

    void Place(std::vector<std::string>& args);
    void Move();
    void Report();
    std::vector<std::string> Split(const std::string& str, const std::string& delimiter);
    std::string ToLower(std::string str);
    std::string ToUpper(std::string str);
    bool TryParseInt(std::string str, int& num);

    const std::map<std::string, Command> m_commands = {
        { "place", cmdPLACE },
        { "move", cmdMOVE },
        { "left", cmdTURN_LEFT },
        { "right", cmdTURN_RIGHT},
        { "report", cmdREPORT},
        { "exit", cmdEXIT }
    };

    const std::map<std::string, FacingDirection> m_facingDirections = {
        { "north", fdNORTH },
        { "south", fdSOUTH },
        { "east", fdEAST },
        { "west", fdWEST },
        { "unknown", fdUNKNOWN }
    };

    std::map<FacingDirection, std::string> m_facingDirectionStrings;

    ToyRobot& m_robot;
    LoggerBase& m_logger;
};

class ConsoleCommander : public CommanderBase
{
public:
    ConsoleCommander(ToyRobot& robot, LoggerBase& logger)
        : CommanderBase(robot, logger)
    {}

    Command GetCommand(std::vector<std::string>& args) override;
};