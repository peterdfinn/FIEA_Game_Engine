#pragma once
#include "RTTI.h"

namespace LibraryShared
{
	template <typename T>
	class Event;

	template <typename T>
	class EventArgs final : public RTTI
	{
		RTTI_DECLARATIONS(EventArgs<T>, RTTI);
		friend class Event<T>;
	private:
		explicit EventArgs(const T& rawMessage);
		virtual ~EventArgs() override = default;
		EventArgs(const EventArgs&) = default;
		EventArgs& operator=(const EventArgs&) = default;
		EventArgs(EventArgs&&) = default;
		EventArgs& operator=(EventArgs&&) = default;
		T mRawMessage;
	};
}

#include "EventArgs.inl"