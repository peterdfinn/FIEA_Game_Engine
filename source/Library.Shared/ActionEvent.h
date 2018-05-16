#pragma once

#include <chrono>
#include <memory>

#include "Action.h"
#include "Factory.h"

namespace LibraryShared
{
	/// <summary>
	/// This concrete subclass of Action, as its action, enqueues an object of
	/// class Event<EventMessageAttributed>. The desired subtype will be a user-
	/// defined string, and the delay the event should have when it's enqueued
	/// is a member variable of this class. Both of these can be set with a
	/// constructor call.
	/// </summary>
	class ActionEvent final : public Action
	{
	public:
		/// <summary>
		/// This is the constructor for ActionEvent. It calls the constructor
		/// for Action, and then it assigns the member variables mDelay and
		/// mSubtype based on the input arguments.
		/// </summary>
		/// <param name="subtype">- The string representing the subtype the
		/// Event created by this Action should have.</param>
		/// <param name="delay">- The number of milliseconds after which the
		/// generated Event is enqueued at which point the Event should be
		/// published.</param>
		/// <param name="name">- The name this Action should have.</param>
		explicit ActionEvent(const std::string& subtype = std::string(),
			std::chrono::milliseconds delay = std::chrono::milliseconds(0),
			const std::string& name = std::string());

		/// <summary>
		/// This is the copy constructor for ActionEvent. It has been deleted.
		/// </summary>
		/// <param name=""></param>
		ActionEvent(const ActionEvent&) = delete;

		/// <summary>
		/// This is the copy assignment operator for ActionEvent. It has been
		/// deleted.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ActionEvent& operator=(const ActionEvent&) = delete;

		/// <summary>
		/// This is the move constructor for ActionEvent. It has been defaulted.
		/// </summary>
		/// <param name="other">- The ActionEvent to move.</param>
		ActionEvent(ActionEvent&& other) = default;

		/// <summary>
		/// This is the move assignment operator for ActionEvent. It has been
		/// defaulted.
		/// </summary>
		/// <param name="other">- The ActionEvent to move.</param>
		/// <returns>A reference to this ActionEvent, post-move.</returns>
		ActionEvent& operator=(ActionEvent&& other) = default;

		/// <summary>
		/// This is the destructor for ActionEvent. It has been defaulted.
		/// </summary>
		virtual ~ActionEvent() override = default;

		/// <summary>
		/// Accessor for the delay to be assigned into the Event created by this
		/// Action.
		/// </summary>
		/// <returns>The number of milliseconds after which the Event created by
		/// this Action is to be published.</returns>
		std::chrono::milliseconds GetDelay() const;

		/// <summary>
		/// Mutator for the delay to be assigned into the Event created by this
		/// Action.
		/// </summary>
		/// <param name="delay">The number of milliseconds after which the Event
		/// created by this Action should be published.</param>
		void SetDelay(std::chrono::milliseconds delay);

		/// <summary>
		/// Accessor for the subtype the Event's message should be.
		/// </summary>
		/// <returns>- The subtype of the Event's message.</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Mutator for the subtype of the Event's message.
		/// </summary>
		/// <param name="subtype">- The subtype the Event's message should be.
		/// </param>
		void SetSubtype(const std::string& subtype);
		
		/// <summary>
		/// The Update method for an ActionEvent will create an
		/// Event<EventMessageAttributed>, assign it values for its subtype and
		/// delay, copy all auxiliary attributes from this Attributed, and
		/// enqueue the Event.
		/// </summary>
		/// <param name="worldState">- A reference to the WorldState metadata
		/// being passed around in the Update tree.</param>
		virtual void Update(WorldState& worldState) override;

	private:
		std::string mSubtype;
		std::chrono::milliseconds mDelay;
		std::shared_ptr<const EventPublisher> mEvent{ nullptr };
	};

	ConcreteFactory(ActionEvent, Action);
}