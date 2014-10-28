#include "stdafx.h"
#include "TestMethods.h"
#include <iostream>
#include <thread>

///////////////////////////////////////////////////////////////////////////////
// PUBLIC methods
// Interactive Test Run Method
void CInteractiveUnitTest::run()
{
	int choice = 0;

	do
	{
		CUnitTest::Instance().PrintMenu();
		std::cout << "Enter choice (-1 to exit): ";
		std::cin >> choice;

		if (-1 < choice)
		{
			try
			{
				int rv = CUnitTest::Instance().InvokeTest(choice);

				std::cout << "Result= " << ((rv==TEST_SUCCESS)?"PASS":"FAIL") << std::endl;
			}
			catch (std::out_of_range)
			{
				std::cerr << "Wrong choice, try again" << std::endl;
			}
		}
	}
	while (-1 < choice);
}

// Print the interactive menu, options 1..N
void CUnitTest::PrintMenu()
{
	for (auto nn = 0U; nn < Spp.size(); ++nn)
	{
		std::cout << nn+1 << ": " << TestName(nn) << std::endl;
	}
}

// Invoke the selected test method
int CUnitTest::InvokeTest(int choice)
{
	return TestPtr(choice-1)();
}


///////////////////////////////////////////////////////////////////////////////
// SINGLETON Items
// PRIVATE static reference value to the single object instance
std::unique_ptr<CUnitTest> CUnitTest::m_Instance;
// PRIVATE static once-flag value to control initialization of this instance
std::once_flag CUnitTest::m_Once;

//------------------------------------------------------------------------------
/// @brief  Instance - get a reference to the object instance (create if needed)
/// @param  None
/// @return Reference to the single object instance
//------------------------------------------------------------------------------
CUnitTest& CUnitTest::Instance()
{
	std::call_once(m_Once, []{ m_Instance.reset(new CUnitTest); });

	return *m_Instance.get();
}

// Private default constructor
CUnitTest::CUnitTest()
{
	strAmessage = "This is a test message";
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE Non-static members
// Unit test providers
void CUnitTest::Test1_Provider(EventHandle ev)
{
	std::cout << "In Test1_Provider()" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
#if 0
	std::cout << "Test1_Provider: mis-fire ..." << std::endl;
#else
	std::cout << "Test1_Provider: fire ***" << std::endl;
	SetEventFlag(ev);
	RaiseEventCondition(ev);
#endif
}

void CUnitTest::Test2_Provider(int id, EventHandle ev)
{
	std::cout << "In Test2_Provider(" << id << ")" << std::endl;
	(void) WaitForEventCondition(ev);
	std::cout << "Test2_Provider(" << id << "): fire ***" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE STATIC
// Unit test methods
int CUnitTest::Test1()
{
	EventHandle ev = 0U;
	bool bRv = false;

	std::cout << "In method Test1()" << std::endl;

	try
	{
		ev = CUnitTest::Instance().AllocEvent();
	}
	catch (std::bad_alloc)
	{
		std::cerr << "ERROR: Cannot execute test" << std::endl;
		return TEST_FAIL;
	}
	
	std::thread test_provider = std::thread([=]{ CUnitTest::Instance().Test1_Provider(ev); });

	std::cout << CUnitTest::Instance().strAmessage << std::endl;
	bRv = CUnitTest::Instance().WaitForEventCondition(ev, 15);
	test_provider.join();
	std::cout << "Test result= " << ((true == bRv) ? "True" : "False") << std::endl;
	CUnitTest::Instance().ClearEventFlag(ev);
	CUnitTest::Instance().FreeEvent(ev);

	return TEST_SUCCESS;
}

int CUnitTest::Test2()
{
	EventHandle ev = 0U;

	std::cout << "In method Test2()" << std::endl;

	try
	{
		ev = CUnitTest::Instance().AllocEvent();
	}
	catch (std::bad_alloc)
	{
		std::cerr << "ERROR: Cannot execute test" << std::endl;
		return TEST_FAIL;
	}

	const size_t NumProviders = 5;
	std::thread test_providers[NumProviders];

	for (size_t nn = 0; nn < NumProviders; ++nn)
	{
		test_providers[nn] = std::thread([=]{ CUnitTest::Instance().Test2_Provider(nn+1, ev); });
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(800));
	CUnitTest::Instance().SetEventFlag(ev);
	CUnitTest::Instance().RaiseEventCondition(ev);

	for (auto& tp : test_providers) tp.join();
	
	CUnitTest::Instance().ClearEventFlag(ev);
	CUnitTest::Instance().FreeEvent(ev);

	return TEST_SUCCESS;
}

int CUnitTest::Test3()
{
	std::cout << "In method Test3()" << std::endl;
	return TEST_SUCCESS;
}

int CUnitTest::Test4()
{
	std::cout << "In method Test4()" << std::endl;
	return TEST_SUCCESS;
}

int CUnitTest::Test5()
{
	std::cout << "In method Test5()" << std::endl;
	return TEST_SUCCESS;
}
