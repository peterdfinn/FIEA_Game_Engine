#include "pch.h"

using namespace LibraryShared;

namespace UnitTest
{
	EventSubscriberUnsubscribe::EventSubscriberUnsubscribe(bool floatEvent)
		: mFloatEvent(floatEvent)
	{}

	void EventSubscriberUnsubscribe::Notify(const EventPublisher& event)
	{
		UNREFERENCED_PARAMETER(event);
		if (mFloatEvent)
		{
			Event<float>::Unsubscribe(*this);
		}
		else
		{
			Event<int32_t>::Unsubscribe(*this);
		}
	}

	void EventSubscriberUnsubscribe::SubscribeSelf()
	{
		if (mFloatEvent)
		{
			Event<float>::Subscribe(*this);
		}
		else
		{
			Event<int32_t>::Subscribe(*this);
		}
	}
}