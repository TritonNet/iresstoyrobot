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
#include "Logger.h"
#include <fstream>

// Commander Base class. This class provide abstraction for console and file commanders.
class CommanderBase
{
public:
    CommanderBase(ToyRobot& robot, LoggerBase& logger);
    void Launch();

protected:
    /// <summary>
    /// Try to read a single line from the input stream.
    /// </summary>
    /// <param name="input">string reference for loading the input value</param>
    /// <returns>[true] Line is read sucessfully. [false] input stream is closed</returns>
    bool virtual TryReadLine(std::string& input) = 0;

private:
    /// <summary>
    /// Get a single command from user
    /// </summary>
    /// <param name="args">List of user provided input arguments.</param>
    /// <returns>The command</returns>
    Command GetCommand(std::vector<std::string>& args);

    /// <summary>
    /// Convey the place command to the robot.
    /// </summary>
    /// <param name="args">User arguments</param>
    void Place(std::vector<std::string>& args);

    /// <summary>
    /// Convey move command to the robot.
    /// </summary>
    void Move();

    /// <summary>
    /// Convey report command and print output on the user stream.
    /// </summary>
    void Report();

    /// <summary>
    /// Helper function for splitting string by given delimiter.
    /// </summary>
    std::vector<std::string> Split(const std::string& str, const std::string& delimiter);

    /// <summary>
    /// Helper function for converting string to lower case.
    /// </summary>
    /// <param name="str">Input string</param>
    /// <returns>Output string converted to lower-case</returns>
    std::string ToLower(std::string str);

    /// <summary>
    /// Helper function for converting string to upper case
    /// </summary>
    /// <param name="str">Input string</param>
    /// <returns>Output string converted to upper-case</returns>
    std::string ToUpper(std::string str);

    /// <summary>
    /// Try convert the provided string to an integer.
    /// </summary>
    /// <param name="str">Input string</param>
    /// <param name="num">result integer if conversion if sucess</param>
    /// <returns>[true] conversion sucess. [false] conversion failed.</returns>
    bool TryParseInt(std::string str, int& num);

    /// <summary>
    /// Mapping of commands and their internal representation.
    /// </summary>
    const std::map<std::string, Command> m_commands = {
        { "place", cmdPLACE },
        { "move", cmdMOVE },
        { "left", cmdTURN_LEFT },
        { "right", cmdTURN_RIGHT},
        { "report", cmdREPORT},
        { "exit", cmdEXIT }
    };

    /// <summary>
    /// Mapping of robot's facing directions and their internal representation.
    /// </summary>
    const std::map<std::string, FacingDirection> m_facingDirections = {
        { "north", fdNORTH },
        { "south", fdSOUTH },
        { "east", fdEAST },
        { "west", fdWEST },
        { "unknown", fdUNKNOWN }
    };

    /// <summary>
    /// Inverse mapping of robot's facing direction
    /// </summary>
    std::map<FacingDirection, std::string> m_facingDirectionStrings;

    ToyRobot& m_robot;
    LoggerBase& m_logger;
};

/// <summary>
/// Console commander which can get commands from console
/// </summary>
class ConsoleCommander : public CommanderBase
{
public:
    ConsoleCommander(ToyRobot& robot, LoggerBase& logger)
        : CommanderBase(robot, logger)
    {}

    /// <summary>
    /// Copy constructor is not allowed
    /// </summary>
    ConsoleCommander(const ConsoleCommander&) = delete;

protected:
    bool TryReadLine(std::string& input) override;
};

/// <summary>
/// File commander which can get commands from file
/// </summary>
class FileCommander : public CommanderBase
{
public:
    FileCommander(std::string path, ToyRobot& robot, LoggerBase& logger)
        : CommanderBase(robot, logger),
        m_filestream(path)
    { }

    /// <summary>
    /// Copy constructor is not allowed
    /// </summary>
    FileCommander(const FileCommander&) = delete;

    ~FileCommander()
    {
        m_filestream.close();
    }

protected:
    bool TryReadLine(std::string& input) override;

private:
    std::ifstream m_filestream;
};