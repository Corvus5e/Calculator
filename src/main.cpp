
#include "Calculator.h"

#include <iostream>

using namespace std;

void PrintHelp();

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		PrintHelp();
	}

	try
	{
		cout << Calculator::CalculateExpression(argv[1]) << endl;
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

void PrintHelp()
{
	cout << "TODO: help message here" << endl;
}