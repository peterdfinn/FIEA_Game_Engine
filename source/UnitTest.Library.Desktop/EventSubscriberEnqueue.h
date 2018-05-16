#pragma once
#include "EventSubscriber.h"

namespace UnitTest
{
	class EventSubscriberEnqueue final : public LibraryShared::EventSubscriber
	{
	public:
		EventSubscriberEnqueue(LibraryShared::GameTime* gameTime = nullptr,
			LibraryShared::EventQueue* q = nullptr);
		virtual void Notify(const LibraryShared::EventPublisher& event) override;
	private:
		LibraryShared::EventQueue* mEventQueue;
		LibraryShared::GameTime* mGameTime;
	};
}