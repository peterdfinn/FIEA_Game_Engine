#pragma once

#include <queue>
#include <memory>
#include <algorithm>
#include <mutex>

#include "EventPublisher.h"
#include "Event.h"

namespace LibraryShared
{

	/// <summary>
	/// This class is the Event Queue used by games made using this engine.
	/// </summary>
	class EventQueue final
	{
	public:
		/// <summary>
		/// This method enqueues an Event to be fired off after a delay. The
		/// default delay is 0 milliseconds.
		/// </summary>
		/// <param name="event">- A reference to the Event to enqueue.</param>
		/// <param name="gameTime">- The GameTime representing the current time
		/// information in the context of the game.</param>
		/// <param name="delay">- The delay after being enqueued at which point
		/// the event should be published, in milliseconds. Default value is 0
		/// milliseconds, or no delay.</param>
		/// <returns>A shared_ptr to the heap-allocated Event that was enqueued.
		/// </returns>
		template <typename T>
		std::shared_ptr<const EventPublisher> Enqueue(const Event<T>& event,
			const GameTime& gameTime,
			std::chrono::milliseconds delay = std::chrono::milliseconds(0));
		
		/// <summary>
		/// This method partitions all events in the queue into those that have
		/// expired and those that haven't. Then, all expired events are
		/// published and removed from the queue.
		/// </summary>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// This method can be called to publish an event.
		/// </summary>
		/// <param name="event">- The event to publish.</param>
		static void Send(const EventPublisher& event);

		/// <summary>
		/// Clears the event queue. None of the events are published.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns the number of Events currently in the queue.
		/// </summary>
		/// <returns>- The number of Events currently in the queue.</returns>
		size_t Size() const;

		/// <summary>
		/// Reserve enough room for a specified number of entries in the Event
		/// Queue.
		/// </summary>
		/// <param name="capacity">- The number of entries for which the Event
		/// Queue should have allocated memory.</param>
		void Reserve(size_t capacity);

		/// <summary>
		/// Returns the number of entries in the Event Queue for which there is
		/// currently memory allocated.
		/// </summary>
		/// <returns>The number of entries in the Event Queue for which there is
		/// currently memory allocated.</returns>
		size_t Capacity() const;

		/// <summary>
		/// Returns a bool indicating whether the event queue is empty.
		/// </summary>
		/// <returns>True if the queue is empty; false otherwise.</returns>
		bool IsEmpty() const;

	private:

		std::vector<std::shared_ptr<const EventPublisher>> mQueue;
		std::vector<std::shared_ptr<const EventPublisher>> mExpiredQueue;
		mutable std::mutex mQueueMutex;
		mutable std::mutex mExpiredQueueMutex;
	};
}

#include "EventQueue.inl"