#pragma once

#include "EventSubscriber.h"

namespace UnitTest
{
	class LibraryShared::EventPublisher;
	class FloatSubscriber final : public LibraryShared::EventSubscriber
	{
	public:
		bool Received() const;
		virtual ~FloatSubscriber() override = default;
		virtual void Notify(const LibraryShared::EventPublisher& event) override;
	private:
		bool mReceived{ false };
	};
}