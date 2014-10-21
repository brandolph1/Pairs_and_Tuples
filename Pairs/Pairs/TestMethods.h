#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <array>
#include <atomic>
#include <condition_variable>
#include <new>

#define TEST_SUCCESS 0
#define TEST_FAIL 1

class CUnitTest
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

	typedef size_t EventHandle;
	typedef void(*ProviderMethodPtr)(EventHandle);

	void Test1_Provider(EventHandle);

	typedef std::pair<std::atomic<bool>, std::condition_variable> BoolCondPair;
	typedef std::array<BoolCondPair, 5> EventArray;
	typedef std::array<std::atomic<bool>, 5> AllocEventArray;

	EventArray EventFlags;
	AllocEventArray AllocFlags;
	std::mutex allocMutex;

	EventHandle AllocEvent()
	{
		EventHandle rv = 0U;
		bool allocOK = false;

		{
			std::unique_lock<std::mutex> ul(allocMutex);

			for (auto nn = 0U; nn < AllocFlags.size(); ++nn)
			{
				if (false == AllocFlags[nn])
				{
					AllocFlags[nn] = true;
					rv = nn;
					allocOK = true;
					break;
				}
			}
		} // lock is released here

		if (false == allocOK)
		{
			throw new(std::bad_alloc);
		}

		return rv;
	}

	void FreeEvent(EventHandle ev)
	{
		std::unique_lock<std::mutex> ul(allocMutex);
		AllocFlags.at(ev) = false;
	}

	std::atomic<bool>& EventFlag(size_t ii)
	{
		return std::get<0>(EventFlags.at(ii));
	}

	bool GetEventFlag(size_t ii)
	{
		return EventFlag(ii).load();
	}

	void ClearEventFlag(size_t ii)
	{
		EventFlag(ii).store(false);
	}

	void SetEventFlag(size_t ii)
	{
		EventFlag(ii).store(true);
	}

	std::condition_variable& EventCondition(size_t ii)
	{
		return std::get<1>(EventFlags.at(ii));
	}

	void RaiseEventCondition(size_t ii)
	{
		EventCondition(ii).notify_all();
	}

	bool WaitForEventCondition(size_t ii, int tt)
	{
		std::mutex readyMutex;
		std::unique_lock<std::mutex> ul(readyMutex);

		return EventCondition(ii).wait_for(ul, std::chrono::seconds(tt), [=]{ return GetEventFlag(ii); });
	} // lock is released here

	std::string strAmessage;
};

class CInteractiveUnitTest
{
public:
	int run();
};