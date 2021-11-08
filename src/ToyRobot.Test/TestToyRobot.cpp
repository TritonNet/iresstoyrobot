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

#include "gtest/gtest.h"
#include "ToyRobot.h"

class MockLogger : public LoggerBase
{
protected:
	void Print(std::string /*msgType*/, std::string /*msg*/, std::string /*end*/) override
	{
		// Just ignore any log messages here.
	}
};

class TestToyRobot : public testing::Test
{
public:
	void SetUp() override
	{
		m_logger = std::make_unique<MockLogger>();
		m_robot = std::make_unique<ToyRobot>(*m_logger);
	}

	void ExpectRobotCoordinates(uint8_t x, uint8_t y, FacingDirection facingDirection)
	{
		uint8_t report_x;
		uint8_t report_y;
		FacingDirection report_facingDirection;

		m_robot->Report(report_x, report_y, report_facingDirection);

		EXPECT_EQ(report_x, x);
		EXPECT_EQ(report_y, y);
		EXPECT_EQ(report_facingDirection, facingDirection);
	}

	bool TryPlace(uint8_t x, uint8_t y, FacingDirection facingDirection)
	{
		return m_robot->TryPlace(x, y, facingDirection);
	}

	bool TryMove()
	{
		return m_robot->TryMove();
	}

	bool TryTurnRight()
	{
		return m_robot->TryTurnRight();
	}

	bool TryTurnLeft()
	{
		return m_robot->TryTurnLeft();
	}

private:
	std::unique_ptr<ToyRobot> m_robot;
	std::unique_ptr<LoggerBase> m_logger;
};

TEST_F(TestToyRobot, TestInitialization)
{
	ExpectRobotCoordinates(0,0, fdUNKNOWN);
}

TEST_F(TestToyRobot, TestInitialPlaceCommand)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdSOUTH;

	const auto sucess = TryPlace(cmd_x, cmd_y, cmd_facingDirection);
	EXPECT_TRUE(sucess);

	ExpectRobotCoordinates(cmd_x, cmd_y, cmd_facingDirection);
}

TEST_F(TestToyRobot, TestSecondPlaceCommand)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdSOUTH;

	auto sucess = TryPlace(cmd_x, cmd_y, cmd_facingDirection);
	EXPECT_TRUE(sucess);

	sucess = TryPlace(4, 4, fdNORTH);
	EXPECT_FALSE(sucess);

	ExpectRobotCoordinates(cmd_x, cmd_y, cmd_facingDirection);
}

TEST_F(TestToyRobot, TestMoveBeforePlace)
{
	auto sucess = TryMove();
	EXPECT_FALSE(sucess);

	ExpectRobotCoordinates(0, 0, fdUNKNOWN);
}

TEST_F(TestToyRobot, TestTurnRightFullCircle)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdSOUTH;

	EXPECT_TRUE(TryPlace(cmd_x, cmd_y, cmd_facingDirection));

	EXPECT_TRUE(TryTurnRight());
	ExpectRobotCoordinates(cmd_x, cmd_y, fdWEST);

	EXPECT_TRUE(TryTurnRight());
	ExpectRobotCoordinates(cmd_x, cmd_y, fdNORTH);

	EXPECT_TRUE(TryTurnRight());
	ExpectRobotCoordinates(cmd_x, cmd_y, fdEAST);

	EXPECT_TRUE(TryTurnRight());
	ExpectRobotCoordinates(cmd_x, cmd_y, fdSOUTH);
}

TEST_F(TestToyRobot, TestTurnLeftFullCircle)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdSOUTH;

	EXPECT_TRUE(TryPlace(cmd_x, cmd_y, cmd_facingDirection));

	EXPECT_TRUE(TryTurnLeft());
	ExpectRobotCoordinates(cmd_x, cmd_y, fdEAST);

	EXPECT_TRUE(TryTurnLeft());
	ExpectRobotCoordinates(cmd_x, cmd_y, fdNORTH);

	EXPECT_TRUE(TryTurnLeft());
	ExpectRobotCoordinates(cmd_x, cmd_y, fdWEST);

	EXPECT_TRUE(TryTurnLeft());
	ExpectRobotCoordinates(cmd_x, cmd_y, fdSOUTH);
}

TEST_F(TestToyRobot, TestMoveOnce)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdSOUTH;

	auto sucess = TryPlace(cmd_x, cmd_y, cmd_facingDirection);
	EXPECT_TRUE(sucess);

	sucess = TryMove();
	EXPECT_TRUE(sucess);

	ExpectRobotCoordinates(cmd_x, cmd_y - 1, cmd_facingDirection);
}

TEST_F(TestToyRobot, TestMoveOverSouthEdge)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdSOUTH;

	EXPECT_TRUE(TryPlace(cmd_x, cmd_y, cmd_facingDirection));
	
	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(cmd_x, 1, cmd_facingDirection);

	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(cmd_x, 0, cmd_facingDirection);

	EXPECT_FALSE(TryMove());
	ExpectRobotCoordinates(cmd_x, 0, cmd_facingDirection);
}

TEST_F(TestToyRobot, TestMoveOverEastEdge)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdEAST;

	EXPECT_TRUE(TryPlace(cmd_x, cmd_y, cmd_facingDirection));
	
	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(4, cmd_y, cmd_facingDirection);

	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(5, cmd_y, cmd_facingDirection);

	EXPECT_FALSE(TryMove());
	ExpectRobotCoordinates(5, cmd_y, cmd_facingDirection);
}

TEST_F(TestToyRobot, TestMoveOverNorthEdge)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdNORTH;

	EXPECT_TRUE(TryPlace(cmd_x, cmd_y, cmd_facingDirection));

	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(cmd_x, 3, cmd_facingDirection);

	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(cmd_x, 4, cmd_facingDirection);

	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(cmd_x, 5, cmd_facingDirection);

	EXPECT_FALSE(TryMove());
	ExpectRobotCoordinates(cmd_x, 5, cmd_facingDirection);
}

TEST_F(TestToyRobot, TestMoveOverWestEdge)
{
	auto cmd_x = 3;
	auto cmd_y = 2;
	auto cmd_facingDirection = fdWEST;

	EXPECT_TRUE(TryPlace(cmd_x, cmd_y, cmd_facingDirection));

	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(2, cmd_y, cmd_facingDirection);

	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(1, cmd_y, cmd_facingDirection);

	EXPECT_TRUE(TryMove());
	ExpectRobotCoordinates(0, cmd_y, cmd_facingDirection);

	EXPECT_FALSE(TryMove());
	ExpectRobotCoordinates(0, cmd_y, cmd_facingDirection);
}
