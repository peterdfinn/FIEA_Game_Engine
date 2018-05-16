#include "pch.h"

using namespace std;

namespace LibraryShared
{
	RTTI_DEFINITIONS(EventPublisher);

	EventPublisher::EventPublisher(set<EventSubscriber*>& subs, mutex& subsMutex) :
		mSubscribers(&subs), mSubscribersMutex(&subsMutex)
	{}

	void EventPublisher::SetTime(const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		mDelay = delay;
		mTimeEnqueued = gameTime.CurrentTime();
	}

	const std::chrono::high_resolution_clock::time_point&
		EventPublisher::TimeEnqueued() const
	{
		return mTimeEnqueued;
	}

	const std::chrono::milliseconds& EventPublisher::Delay() const
	{
		return mDelay;
	}

	bool EventPublisher::IsExpired(const GameTime& gameTime) const
	{
		return gameTime.CurrentTime() > mTimeEnqueued + mDelay;
	}

	void EventPublisher::Deliver() const
	{
		assert(mSubscribers != nullptr);
		assert(mSubscribersMutex != nullptr);
		set<EventSubscriber*> localListOfSubscribers;
		{
			lock_guard<mutex> guard(*mSubscribersMutex);
			localListOfSubscribers = *mSubscribers;
		}
		vector<future<void>> futures;

		for (EventSubscriber* sub : localListOfSubscribers)
		{
			assert(sub != nullptr);
			futures.emplace_back(async([sub](const EventPublisher& e) {
				sub->Notify(e);
			}, cref(*this)));
		}

		for (auto& fut : futures)
		{
			fut.get();
		}
	}
}