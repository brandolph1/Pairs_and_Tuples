#pragma once

#include "EventFlag.h"
#include <string>

#define TEST_SUCCESS 0
#define TEST_FAIL 1

class CUnitTest : public CEventFlag
{
private:
	// SINGLETON definitions:
	// PRIVATE static unique pointer to the single object
	static std::unique_ptr<CUnitTest> m_Instance;
	// PRIVATE initialization flag
	static std::once_flag m_Once;
	// PRIVATE copy constructor
	CUnitTest(const CUnitTest&);
	// PRIVATE assignment operator
	CUnitTest& operator= (const CUnitTest&);
	// PRIVATE default constructor
	CUnitTest();

public:
	// PUBLIC destructor (std::unique_ptr is responsible for the deletion of the object)
	virtual ~CUnitTest() {}
	// Get a reference to this instance
	static CUnitTest& Instance();

	void PrintMenu();
	int InvokeTest(int /*choice*/);

private:
	static int Test1();
	static int Test2();
	static int Test3();
	static int Test4();
	static int Test5();

	typedef int(*TestMethodPtr)(void);
	typedef std::pair<std::string, TestMethodPtr> StrPtrPair;
	typedef std::array<StrPtrPair, 5> MenuArray;

	MenuArray Spp{ {
		StrPtrPair("Test1", Test1),
		StrPtrPair("Test2", Test2),
		StrPtrPair("Test3", Test3),
		StrPtrPair("Test4", Test4),
		StrPtrPair("Test5", Test5)
	} };

	std::string& TestName(size_t ii)
	{
		return std::get<0>(Spp.at(ii));
	}

	TestMethodPtr TestPtr(size_t ii)
	{
		return std::get<1>(Spp.at(ii));
	}

	void Test1_Provider(EventHandle);

	std::string strAmessage;
};

class CInteractiveUnitTest
{
public:
	void run();
};