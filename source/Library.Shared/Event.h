#pragma once

#include <set>
#include <mutex>

#include "EventPublisher.h"
#include "EventArgs.h"

namespace LibraryShared
{
	class EventSubscriber;

	/// <summary>
	/// This class template represents an event in the event-driven system this
	/// engine uses. It has a payload (a.k.a. message) of type T.
	/// </summary>
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<T>, EventPublisher);
	public:
		/// <summary>
		/// This is the only constructor of Event<T>. It takes a raw message of
		/// type T and stores it in the EventArgs object this object has.
		/// </summary>
		/// <param name="rawMessage">- The raw message to be delivered to all
		/// subscribers upon publication.</param>
		explicit Event(const T& rawMessage);

		/// <summary>
		/// The destructor for Event has been defaulted.
		/// </summary>
		virtual ~Event() override = default;

		/// <summary>
		/// The copy constructor for Event has been defaulted.
		/// </summary>
		/// <param name="other">- The Event to copy.</param>
		Event(const Event& other) = default;

		/// <summary>
		/// The copy assignment operator for Event has been defaulted.
		/// </summary>
		/// <param name="other">- The Event to copy.</param>
		/// <returns>A reference to this Event post-copy.</returns>
		Event& operator=(const Event& other) = default;

		/// <summary>
		/// The move constructor for Event has been defaulted.
		/// </summary>
		/// <param name="other">- The Event to move.</param>
		Event(Event&& other) = default;

		/// <summary>
		/// The move assignment operator for Event has been defaulted.
		/// </summary>
		/// <param name="other">- The Event to move.</param>
		/// <returns>A reference to this Event post-move.</returns>
		Event& operator=(Event&& other) = default;

		/// <summary>
		/// Accessor for the raw message, contained within the EventArgs in this
		/// Event.
		/// </summary>
		/// <returns>The raw message this Event contains in its EventArgs.
		/// </returns>
		const T& Message() const;

		/// <summary>
		/// This static method subscribes an input EventSubscriber to all Events
		/// of type T, where T is the class of the template Event<T>. When an
		/// Event<T> is delivered, a subscribed EventSubscriber's Notify method
		/// is called, storing a pointer to the Event in that object.
		/// </summary>
		/// <param name="subscriber">- The new subscriber to Events of type T.
		/// </param>
		static void Subscribe(EventSubscriber& subscriber);
		
		/// <summary>
		/// This static method unsubscribes an EventSubscriber from Events of
		/// type T.
		/// </summary>
		/// <param name="subscriber">- A reference to the subscriber to
		/// unsubscribe from Events.</param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// This method unsubscribes all current subscribers from Events of type
		/// T.
		/// </summary>
		static void UnsubscribeAll();

	private:
		T mMessage;
		static std::mutex staticSubscribersMutex;
		static std::set<EventSubscriber*> staticSubscribers;
	};
}

#include "Event.inl"