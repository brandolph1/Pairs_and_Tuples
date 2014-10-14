// Pairs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <utility>
#include <array>
#include "TestMethods.h"

typedef std::pair<std::string, TestMethodPtr> StrPtrPair;

std::array<StrPtrPair, 5> Spp = {
	StrPtrPair("Test1", Test1),
	StrPtrPair("Test2", Test2),
	StrPtrPair("Test3", Test3),
	StrPtrPair("Test4", Test4),
	StrPtrPair("Test5", Test5)
};

int _tmain(int argc, _TCHAR* argv[])
{
	int nn = 0;
	int choice = 0;

	do
	{
		nn = 0;

		for (auto var : Spp)
		{
			std::cout << nn << ": " << std::get<0>(var) << std::endl;
			nn++;
		}

		std::cout << "Enter choice (-1 to exit): ";
		std::cin >> choice;

		if (-1 < choice)
		{
			try
			{
				int rv = std::get<1>(Spp.at(choice))();

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

