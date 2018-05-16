#pragma once

#include "EventSubscriber.h"

namespace UnitTest
{
	class EventSubscriberUnsubscribe final : public LibraryShared::EventSubscriber
	{
	public:
		explicit EventSubscriberUnsubscribe(bool floatEvent);
		void SubscribeSelf();
		virtual void Notify(const LibraryShared::EventPublisher& event) override;
	private:
		bool mFloatEvent{ true };
	};
}