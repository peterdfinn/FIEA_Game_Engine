#include "pch.h"
#include "DatumSubscriber.h"

using namespace std;
using namespace LibraryShared;

namespace UnitTest
{
	void DatumSubscriber::Notify(const EventPublisher& event)
	{
		mEvent = &event;
	}

	bool DatumSubscriber::TypeCheck() const
	{
		if (mEvent == nullptr)
		{
			return false;
		}

		return mEvent->Is(Event<Datum>::TypeIdClass());
	}

	const EventPublisher* DatumSubscriber::GetEvent() const
	{
		return mEvent;
	}
}
