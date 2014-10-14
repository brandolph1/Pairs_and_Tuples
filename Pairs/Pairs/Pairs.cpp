// Pairs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <utility>
#include "TestMethods.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int choice = 0;

	do
	{
		CTest::Instance().PrintMenu();
		std::cout << "Enter choice (-1 to exit): ";
		std::cin >> choice;

		if (-1 < choice)
		{
			try
			{
				int rv = CTest::Instance().InvokeTest(choice);

				std::cout << "Result= " << rv << std::endl;
			}
			catch (std::out_of_range)
			{
				std::cerr << "Wrong choice, try again" << std::endl;
			}
		}
	}
	while (-1 < choice);

	return 0;
}

