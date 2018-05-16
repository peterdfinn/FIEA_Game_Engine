#pragma once

#include <chrono>
#include <set>

#include "RTTI.h"

#include "GameTime.h"
namespace LibraryShared
{
	class EventSubscriber;

	/// <summary>
	/// This class is the base interface from which all Event<T> classes
	/// inherit. The Event Queue is composed of EventPublishers.
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);
	public:
		/// <summary>
		/// This is the constructor for an EventPublisher. It should only be
		/// called by an Event constructor. It assigns the address of the input
		/// std::vector to mSubscribers.
		/// </summary>
		EventPublisher(std::set<EventSubscriber*>& subs, std::mutex& subsMutex);
	
		/// <summary>
		/// This is the copy constructor for EventPublisher. It has been
		/// defaulted.
		/// </summary>
		/// <param name="other">- The EventPublisher to copy.</param>
		EventPublisher(const EventPublisher& other) = default;

		/// <summary>
		/// This is the copy assignment operator for EventPublisher. It has been
		/// defaulted.
		/// </summary>
		/// <param name="other">- The EventPublisher to copy.</param>
		/// <returns>A reference to this EventPublisher.</returns>
		EventPublisher& operator=(const EventPublisher& other) = default;

		/// <summary>
		/// This is the move constructor for EventPublisher. It has been
		/// defaulted.
		/// </summary>
		/// <param name="other">- The EventPublisher to move from.</param>
		EventPublisher(EventPublisher&& other) = default;

		/// <summary>
		/// This is the move assignment operator for EventPublisher. It has been
		/// defaulted.
		/// </summary>
		/// <param name="other">- The EventPublisher to move from.</param>
		/// <returns>A reference to this EventPublisher.</returns>
		EventPublisher& operator=(EventPublisher&& other) = default;

		/// <summary>
		/// This is the destructor for EventPublisher. It has been defaulted.
		/// </summary>
		virtual ~EventPublisher() override = default;

		/// <summary>
		/// Mutator for the time members of this class. The delay is set to the
		/// input argument, and the time enqueued is set to the current time.
		/// </summary>
		/// <param name="gameTime">- A reference to the GameTime being passed
		/// through the Update tree when this EventPublisher is enqueued.
		/// </param>
		/// <param name="delay">- The delay after enqueuing this EventPublisher
		/// at which this object should be published.</param>
		void SetTime(const GameTime& gameTime, std::chrono::milliseconds delay =
			std::chrono::milliseconds(0));

		/// <summary>
		/// Accessor for the time at which this EventPublisher was enqueued. If
		/// this EventPublisher was never enqueued, then this value has no
		/// defined meaning.
		/// </summary>
		/// <returns>- The time at which this EventPublisher was enqueued, if it
		/// was ever enqueued.</returns>
		const std::chrono::high_resolution_clock::time_point& TimeEnqueued()
			const;

		/// <summary>
		/// Accessor for the number of milliseconds in the delay between this
		/// EventPublisher being enqueued and this EventPublisher being
		/// published.
		/// </summary>
		/// <returns>- The number of milliseconds after enqueuing this
		/// EventPublisher at which this EventPublisher should be published.
		/// </returns>
		const std::chrono::milliseconds& Delay() const;

		/// <summary>
		/// When called on an enqueued EventPublisher, this method determines
		/// whether this EventPublisher's delay has expired.
		/// </summary>
		/// <param name="gameTime">- The GameTime at which this method is being
		/// called.</param>
		/// <returns>True if this enqueued EventPublisher's delay has expired;
		/// false otherwise. If this EventPublisher is not currently enqueued,
		/// the return value is undefined.</returns>
		bool IsExpired(const GameTime& gameTime) const;

		/// <summary>
		/// This method delivers this object to all subscribers.
		/// </summary>
		void Deliver() const;

	private:

		std::chrono::high_resolution_clock::time_point mTimeEnqueued;
		std::chrono::milliseconds mDelay;

		/*This uses pointers instead of
		references because Vector cannot use references.*/
		std::set<EventSubscriber*>* mSubscribers{nullptr};
		
		std::mutex* mSubscribersMutex{ nullptr };
	};
}