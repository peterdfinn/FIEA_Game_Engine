#include "pch.h"
#include "EventSubscriberEnqueue.h"

using namespace LibraryShared;

namespace UnitTest
{
	EventSubscriberEnqueue::EventSubscriberEnqueue(GameTime* gameTime, EventQueue* q) :
		mEventQueue(q), mGameTime(gameTime)
	{}

	void EventSubscriberEnqueue::Notify(const EventPublisher& event)
	{
		UNREFERENCED_PARAMETER(event);
		if (mEventQueue != nullptr && mGameTime != nullptr)
		{
			Event<float> e1(1.0f);
			mEventQueue->Enqueue(e1, *mGameTime);
		}
	}
}