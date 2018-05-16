#pragma once

#include "EventPublisher.h"
namespace LibraryShared
{
	/// <summary>
	/// This is the interface for any and all classes that want to receive
	/// events.
	/// </summary>
	class EventSubscriber
	{
	public:
		/// <summary>
		/// This method passes an Event to a subscriber and allows the
		/// subscriber access to its message.
		/// </summary>
		/// <param name="event">- The Event this subscriber subscribed to.
		/// </param>
		virtual void Notify(const EventPublisher& event) = 0;

		/// <summary>
		/// The destructor for EventSubscriber has been defaulted.
		/// </summary>
		virtual ~EventSubscriber() = default;
	};
}