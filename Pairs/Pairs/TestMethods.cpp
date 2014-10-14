#include "stdafx.h"
#include "TestMethods.h"
#include <iostream>

// PUBLIC methods
void CTest::PrintMenu()
{
	int nn = 0;

	for (auto var : CTest::Spp)
	{
		std::cout << nn << ": " << std::get<0>(var) << std::endl;
		nn++;
	}
}

int CTest::InvokeTest(int choice)
{
	return std::get<1>(CTest::Spp.at(choice))();
}

// PRIVATE static reference value to the single object instance
std::unique_ptr<CTest> CTest::m_Instance;
// PRIVATE static once-flag value to control initialization of this instance
std::once_flag CTest::m_Once;

//------------------------------------------------------------------------------
/// @brief  Instance - get a reference to the object instance (create if needed)
/// @param  None
/// @return Reference to the single object instance
//------------------------------------------------------------------------------
CTest& CTest::Instance()
{
	std::call_once(m_Once, []{ m_Instance.reset(new CTest); });

	return *m_Instance.get();
}

// Private default constructor
CTest::CTest()
{
	strAmessage = "This is the message";
}

// PRIVATE STATIC test methods
int CTest::Test1()
{
	std::cout << "In method Test1()" << std::endl;
	std::cout << CTest::Instance().strAmessage << std::endl;
	return 0;
}

int CTest::Test2()
{
	std::cout << "In method Test2()" << std::endl;
	return -1;
}

int CTest::Test3()
{
	std::cout << "In method Test3()" << std::endl;
	return -2;
}

int CTest::Test4()
{
	std::cout << "In method Test4()" << std::endl;
	return -3;
}

int CTest::Test5()
{
	std::cout << "In method Test5()" << std::endl;
	return -4;
}
