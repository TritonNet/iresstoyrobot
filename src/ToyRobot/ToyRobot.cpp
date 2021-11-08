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

#include "ToyRobot.h"
#include "Logger.h"

ToyRobot::ToyRobot(LoggerBase& logger):
	m_logger(logger)
{
}

bool ToyRobot::TryPlace(uint8_t x, uint8_t y, FacingDirection facingDirection)
{
	if (m_placed)
	{
		m_logger.Warn("Robot is already placed. Ignoring the command");
		return false;
	}

	if (x < 0 || x > m_xmax)
	{
		m_logger.Error("Invalid x coordinate. X should be in between 0-" + std::to_string(m_xmax));
		return false;
	}

	if (y < 0 || y > m_ymax)
	{
		m_logger.Error("Invalid y coordinate. Y should be in between 0-" + std::to_string(m_ymax));
		return false;
	}

	m_x = x;
	m_y = y;
	m_facingDirection = facingDirection;
	m_placed = true;

	return true;
}

bool ToyRobot::TryMove()
{
	if (!m_placed)
	{
		m_logger.Error("Robot is not placed. Please place the robot before moving.");
		return false;
	}

	switch (m_facingDirection)
	{	
	case fdNORTH:
		if (m_y + 1 > m_ymax)
		{
			m_logger.Warn("Robot going to move over the north edge. Command is ignored for safety.");
			return false;
		}
		m_y++;
		return true;
	case fdSOUTH:
		if (m_y - 1 < 0)
		{
			m_logger.Warn("Robot going to move over the south edge. Command is ignored for safety.");
			return false;
		}
		m_y--;
		return true;
	case fdEAST:
		if (m_x + 1 > m_xmax)
		{
			m_logger.Warn("Robot going to move over the east edge. Command is ignored for safety.");
			return false;
		}
		m_x++;
		return true;
	case fdWEST:
		if (m_x - 1 < 0)
		{
			m_logger.Warn("Robot going to move over the west edge. Command is ignored for safety.");
			return false;
		}
		m_x--;
		return true;
	case fdUNKNOWN:
	default:
		m_logger.Error("Robot is facing an unknown direction.");
		return false;
	}
}

bool ToyRobot::TryTurnLeft()
{
	if (!m_placed)
	{
		m_logger.Error("Robot is not placed.");
		return false;
	}

	switch (m_facingDirection)
	{
	case fdNORTH:
		m_facingDirection = fdWEST;
		m_logger.Info("Robot is now facing WEST");
		return true;
	case fdSOUTH:
		m_facingDirection = fdEAST;
		m_logger.Info("Robot is now facing EAST");
		return true;
	case fdEAST:
		m_facingDirection = fdNORTH;
		m_logger.Info("Robot is now facing NORTH");
		return true;
	case fdWEST:
		m_facingDirection = fdSOUTH;
		m_logger.Info("Robot is now facing SOUTH");
		return true;
	case fdUNKNOWN:
	default:
		m_logger.Error("Robot is now facing an unknown direction.");
		return false;
	}
}

bool ToyRobot::TryTurnRight()
{
	if (!m_placed)
	{
		m_logger.Error("Robot is not placed.");
		return false;
	}

	switch (m_facingDirection)
	{
	case fdNORTH:
		m_facingDirection = fdEAST;
		m_logger.Info("Robot is now facing EAST");
		return true;
	case fdSOUTH:
		m_facingDirection = fdWEST;
		m_logger.Info("Robot is now facing WEST");
		return true;
	case fdEAST:
		m_facingDirection = fdSOUTH;
		m_logger.Info("Robot is now facing SOUTH");
		return true;
	case fdWEST:
		m_facingDirection = fdNORTH;
		m_logger.Info("Robot is now facing NORTH");
		return true;
	case fdUNKNOWN:
	default:
		m_logger.Error("Robot is now facing an unknown direction.");
		return false;
	}
}

void ToyRobot::Report(uint8_t& x, uint8_t& y, FacingDirection& facingDirection)
{
	x = m_x;
	y = m_y;
	facingDirection = m_facingDirection;
}
