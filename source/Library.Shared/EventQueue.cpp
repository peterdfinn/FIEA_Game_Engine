#include "pch.h"

using namespace std;

namespace LibraryShared
{
	void EventQueue::Update(const GameTime& gameTime)
	{
		vector<future<void>> futures;

		// locking unexpired queue and moving 
		{
			lock_guard<mutex> queueLock(mQueueMutex);
			vector<shared_ptr<const EventPublisher>>::iterator part =
				partition(mQueue.begin(), mQueue.end(), [&gameTime](const shared_ptr<const EventPublisher>& e)
			{
				return !e->IsExpired(gameTime);
			});
			for (auto it = part; it != mQueue.end(); ++it)
			{
				const shared_ptr<const EventPublisher>& sharedptr = *it;
				futures.emplace_back(async([&sharedptr, this]
				{
					lock_guard<mutex> expiredQueueLock(mExpiredQueueMutex);
					mExpiredQueue.push_back(sharedptr);
				}));
			}
			for (future<void>& fut : futures)
			{
				fut.get();
			}
			futures.clear();

			mQueue.erase(part, mQueue.end());
		}
		{
			lock_guard<mutex> expiredQueueLock(mExpiredQueueMutex);
			for (const shared_ptr<const EventPublisher>& expiredEvent : mExpiredQueue)
			{
				futures.emplace_back(async([&expiredEvent]
				{
					expiredEvent->Deliver();
				}));
			}
			for (future<void>& fut : futures)
			{
				fut.get();
			}
			futures.clear();
			mExpiredQueue.clear();
		}
	}

	bool EventQueue::IsEmpty() const
	{
		return mQueue.empty() && mExpiredQueue.empty();
	}

	size_t EventQueue::Size() const
	{
		return mQueue.size() + mExpiredQueue.size();
	}

	size_t EventQueue::Capacity() const
	{
		return mQueue.capacity();
	}

	void EventQueue::Reserve(size_t capacity)
	{
		{
			lock_guard<mutex> lockGuard(mQueueMutex);
			mQueue.reserve(capacity);
		}
		{
			lock_guard<mutex> lockGuard(mExpiredQueueMutex);
			mExpiredQueue.reserve(capacity);
		}
	}

	void EventQueue::Clear()
	{
		{
			lock_guard<mutex> lockGuard(mQueueMutex);
			mQueue.clear();
		}
		{
			lock_guard<mutex> lockGuard(mExpiredQueueMutex);
			mExpiredQueue.clear();
		}
	}

	void EventQueue::Send(const EventPublisher& event)
	{
		event.Deliver();
	}
}