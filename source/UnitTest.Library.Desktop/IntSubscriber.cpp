#include "pch.h"
#include "IntSubscriber.h"

using namespace LibraryShared;

namespace UnitTest
{
	bool IntSubscriber::Received() const
	{
		return mReceived;
	}

	void IntSubscriber::Notify(const EventPublisher& event)
	{
		if (event.Is(Event<int>::TypeIdClass()))
		{
			mReceived = true;
		}
	}
}