# IRessToyRobot
IRESS - Toy Robot Code Challenge

##### Build Requirements
1. Visual Studio 2019
2. Platform Toolset v142
3. Windows SDK 10
4. C++ 14

##### Build Instructions

1. Open the solution file
2. Restore nuget packages
3. Build the solution

##### Unittests
Unit tests can be executed to ensure the correctness of the functionality.
Please find the unit tests in `ToyRobot.Test` project.

##### Run instruction

This toy robot can be run via the commands provided either through the console winodw, or input file.

1. Run the `>toyrobot.exe` to launch for console mode.
2. Run `>toyrobot.exe commands.txt output.txt` for file commands.
	The `commands.txt` file should contain the commands in the order that they should execute.
	The `output.txt` file will be created if not exists and the output logs will be appended to the file.

The robot commands are as per the [instruction.pdf](docs/instructions.pdf) file.
