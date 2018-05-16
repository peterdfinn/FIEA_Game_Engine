#include "pch.h"

using namespace LibraryShared;

namespace UnitTest
{
	bool FloatSubscriber::Received() const
	{
		return mReceived;
	}

	void FloatSubscriber::Notify(const EventPublisher& event)
	{
		if (event.Is(Event<float>::TypeIdClass()))
		{
			mReceived = true;
		}
	}
}