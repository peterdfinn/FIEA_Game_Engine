#pragma once

#include "Reaction.h"
#include "Factory.h"

namespace LibraryShared
{
	/// <summary>
	/// A ReactionAttributed is a concrete Reaction that responds to Events with
	/// messages of type EventMessageAttributed.
	/// </summary>
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

	public:
		/// <summary>
		/// This is the constructor for ReactionAttributed. It takes one
		/// argument, the string representing the subtype of
		/// EventMessageAttributed this Reaction will accept.
		/// </summary>
		/// <param name="subtype">- The string representing the subtype of
		/// EventMessageAttributed this Reaction will accept; its default value
		/// is the empty string.</param>
		explicit ReactionAttributed(const std::string& subtype = std::string());

		/// <summary>
		/// This is the copy constructor for ReactionAttributed. It has been
		/// deleted.
		/// </summary>
		/// <param name=""></param>
		ReactionAttributed(const ReactionAttributed&) = delete;

		/// <summary>
		/// This is the copy assignment operator for ReactionAttributed. It has
		/// been deleted.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ReactionAttributed& operator=(const ReactionAttributed&) = delete;

		/// <summary>
		/// This is the move constructor for ReactionAttributed. It calls the
		/// move constructor of its base class, and then it moves the subtype
		/// string and the pointer to the EventMessageAttributed.
		/// </summary>
		/// <param name="other">- The ReactionAttributed to move.</param>
		ReactionAttributed(ReactionAttributed&& other);

		/// <summary>
		/// This is the move assignment operator for ReactionAttributed. It
		/// calls the move assignment operator of its base class, and then it
		/// moves the subtype string and the pointer to the
		/// EventMessageAttributed.
		/// </summary>
		/// <param name="other">- The ReactionAttributed to move.</param>
		/// <returns>A reference to this object post-move.</returns>
		ReactionAttributed& operator=(ReactionAttributed&& other);

		/// <summary>
		/// This is the destructor for ReactionAttributed. It unsubscribes this
		/// object from Event<EventMessageAttributed>.
		/// </summary>
		virtual ~ReactionAttributed() override;

		/// <summary>
		/// Mutator for the subtype of EventMessageAttributed this object is
		/// interested in.
		/// </summary>
		/// <param name="subtype">- The subtype of EventMessageAttributed this
		/// object should receive from Events.</param>
		void SetSubtype(const std::string& subtype);

		/// <summary>
		/// Accessor for the subtype of EventMessageAttributed this object is
		/// interested in.
		/// </summary>
		/// <returns>- The subtype of EventMessageAttributed this object is
		/// interested in.</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Accessor for the address of the EventMessageAttributed this Reaction
		/// has received. If this Reaction has never received an Event.
		/// </summary>
		/// <returns>If this Reaction has ever received an Event, the address of
		/// the contained EventMessageAttributed; otherwise, nullptr is
		/// returned.</returns>
		const EventMessageAttributed* GetEventMessage() const;

		/// <summary>
		/// This method checks to see if the input EventPublisher is an
		/// Event<EventMessageAttributed>, and if so, checks to see if the
		/// contained EventMessageAttributed is of the same subtype as this
		/// Reaction; if so, a pointer to this EventMessageAttributed is stored
		/// in this object.
		/// </summary>
		/// <param name="event">- The Event that gets delivered to this
		/// EventSubscriber.</param>
		virtual void Notify(const EventPublisher& event) override;

	private:

		std::string mSubtype;

		const EventMessageAttributed* mEventMessage{ nullptr };

		std::shared_ptr<const EventPublisher> mEventSharedPtr{ nullptr };

	};

	ConcreteFactory(ReactionAttributed, Reaction);
}