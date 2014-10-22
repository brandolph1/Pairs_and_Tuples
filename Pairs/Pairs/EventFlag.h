#pragma once
//
// Base class for allocating, registering, and alerting condition events
//
// Typical use:
//  in main thread--
//   1. EventHandle ev = AllocEvent();
//   2. start worker thread, passing 'ev'
//   3. flag = WaitForEventCondition(ev, t-duration);
//   4. join worker thread
//   5. report/test flag, consume data from worker thread
//   6. ClearEventFlag(ev)
//   7. FreeEvent(ev)
//
//  in worker thread--
//   1. Accept event handle 'ev' as a parameter
//   2. Do some work, create data for consumption by main thread
//   3. SetEventFlag(ev)
//   4. RaiseEventCondition(ev)
//
#include <memory>
#include <array>
#include <atomic>
#include <condition_variable>
#include <new>

class CEventFlag
{
private:
	typedef std::pair<std::atomic<bool>, std::condition_variable> BoolCondPair;
	typedef std::array<BoolCondPair, 5> EventArray;
	typedef std::array<std::atomic<bool>, 5> AllocEventArray;

	EventArray EventFlags;
	AllocEventArray AllocFlags;
	std::mutex allocMutex;

public:
	CEventFlag()
	{
		for (auto nn = 0U; nn < NumEventFlags(); ++nn)
		{
			std::atomic_init(&EventFlag(nn), false);
		}

		for (auto nn = 0U; nn < NumAllocFlags(); ++nn)
		{
			std::atomic_init(&AllocFlag(nn), false);
		}
	}

	typedef size_t EventHandle;

	size_t NumEventFlags() { return EventFlags.size(); }
	size_t NumAllocFlags() { return AllocFlags.size(); }

	std::atomic<bool>& AllocFlag(size_t ii)
	{
		return AllocFlags.at(ii);
	}

	EventHandle AllocEvent()
	{
		EventHandle rv = 0U;
		bool allocOK = false;

		{
			std::unique_lock<std::mutex> ul(allocMutex);

			for (auto nn = 0U; nn < NumAllocFlags(); ++nn)
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
		AllocFlag(ev) = false;
	} // lock is released here

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
};