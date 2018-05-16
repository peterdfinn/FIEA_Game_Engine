#pragma once
#include <memory>
#include "EventSubscriber.h"

namespace UnitTest
{
	class LibraryShared::EventPublisher;

	class DatumSubscriber final : public LibraryShared::EventSubscriber
	{
	public:
		virtual void Notify(const LibraryShared::EventPublisher& event) override;
		bool TypeCheck() const;
		const LibraryShared::EventPublisher* GetEvent() const;
	private:
		const LibraryShared::EventPublisher* mEvent{ nullptr };
	};
}