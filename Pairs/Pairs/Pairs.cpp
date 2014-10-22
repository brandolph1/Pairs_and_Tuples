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
		test->run();
		delete test;
	}

	return 0;
}

