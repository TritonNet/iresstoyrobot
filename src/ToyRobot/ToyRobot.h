/*
 *	The MIT License (MIT)
 *	Copyright � 2021 Kushan Fernando (info@kushan.me)
 *	
 *	Permission is hereby granted, free of charge, to any person obtaining 
 *  a copy of this software and associated documentation files (the �Software�), 
 *  to deal in the Software without restriction, including without limitation 
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 *  sell copies of the Software, and to permit persons to whom the Software is furnished 
 *  to do so, subject to the following conditions:
 *	
 *	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *	
 *	THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 *  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 *  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 *  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <stdint.h>
#include "FacingDirection.h"
#include "Logger.h"

class ToyRobot
{
public:
	ToyRobot(LoggerBase& logger);
	bool TryPlace(uint8_t x, uint8_t y, FacingDirection facingDirection);
	bool TryMove();
	bool TryTurnRight();
	bool TryTurnLeft();
	void Report(uint8_t& x, uint8_t& y, FacingDirection& facingDirection);

private:
	uint8_t m_x = 0;
	uint8_t m_y = 0;

	const uint8_t m_xmax = 5;
	const uint8_t m_ymax = 5;

	FacingDirection m_facingDirection = fdUNKNOWN;
	bool m_placed = false;

	LoggerBase& m_logger;
};
