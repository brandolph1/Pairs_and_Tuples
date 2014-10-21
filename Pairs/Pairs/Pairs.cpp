// Pairs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <utility>
#include "TestMethods.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CInteractiveUnitTest * test = nullptr;
	int retVal = TEST_SUCCESS;

	try
	{
		test = new CInteractiveUnitTest;
	}
	catch (...)
	{
		std::cerr << "ERROR: Failed to instantiate CInteractiveUnitTest" << std::endl;
		return TEST_FAIL;
	}

	if (nullptr != test)
	{
		retVal = test->run();
		delete test;
	}

	return retVal;
}

