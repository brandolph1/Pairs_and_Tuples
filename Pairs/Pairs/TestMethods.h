#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <array>

class CTest
{
private:
	// SINGLETON definitions:
	// PRIVATE static unique pointer to the single object
	static std::unique_ptr<CTest> m_Instance;
	// PRIVATE initialization flag
	static std::once_flag m_Once;
	// PRIVATE copy constructor
	CTest(const CTest&);
	// PRIVATE assignment operator
	CTest& operator= (const CTest&);
	// PRIVATE default constructor
	CTest();

public:
	// PUBLIC destructor (std::unique_ptr is responsible for the deletion of the object)
	virtual ~CTest() {}
	// Get a reference to this instance
	static CTest& Instance();

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

	MenuArray Spp { {
		CTest::StrPtrPair("Test1", Test1),
		CTest::StrPtrPair("Test2", Test2),
		CTest::StrPtrPair("Test3", Test3),
		CTest::StrPtrPair("Test4", Test4),
		CTest::StrPtrPair("Test5", Test5)
	} };

	std::string strAmessage;
};
