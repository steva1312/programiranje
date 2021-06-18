#include <iostream>

void Log(const char* str, bool endline)
{	
	if (endline) {
		std::cout << str << std::endl;
	}
	else {
		std::cout << str;
	}
}

void LogSolution(int x, int y, std::string operation)
{
	int solution;
	bool valid = true;
	if (operation == "+") {
		solution = x + y;
	} else if (operation == "-") {
		solution = x - y;
	} else if (operation == "*") {
		solution = x * y;
	} else if (operation == "/") {
		solution = x / y;
	} else {
		Log("Invalid operation.", true);
		valid = false;
	}

	if (valid) {
		std::cout << solution << std::endl;
	}
}

void Operation()
{
	int x, y;
	std::string operation;

	Log("Enter first number: ", false);
	std::cin >> x;
	Log("Enter second number: ", false);
	std::cin >> y;
	Log("Enter operation (+, -, *, /): ", false);
	std::cin >> operation;

	LogSolution(x, y, operation);
}

int main()
{
	while (true) {
		Operation();
	}
}