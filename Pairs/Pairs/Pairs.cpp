// Pairs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestMethods.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	CInteractiveUnitTest * test = nullptr;

	try
	{
		test = new CInteractiveUnitTest;
	}
	catch (...)
	{
		std::cerr << "ERROR: Failed to instantiate CInteractiveUnitTest" << std::endl;
		return 255;
	}

	if (nullptr != test)
	{
		size_t num_vals = CUnitTest::Instance().NumAllocFlags();
		bool rv = false;

		for (auto nn = 0U; nn < num_vals; ++nn)
		{
			std::cout << nn << ": ";
			rv = CUnitTest::Instance().AllocFlag(nn).is_lock_free();
			std::cout << "Is lock free= " << ((rv == true) ? "True" : "False") << ", ";
			rv = CUnitTest::Instance().AllocFlag(nn).load();
			std::cout << "Value= " << ((rv == true) ? "True" : "False");
			std::cout << std::endl;
		}

		test->run();
		delete test;
	}

	return 0;
}

