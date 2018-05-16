#pragma once

#include "Attributed.h"

namespace LibraryShared
{
	class World;

	/// <summary>
	/// This class is a specific type of event message. It represents an event
	/// message that is an Attributed object.
	/// </summary>
	class EventMessageAttributed final : public Attributed
	{
	public:
		/// <summary>
		/// This is the constructor for EventMessageAttributed.
		/// </summary>
		/// <param name="subtype">- The name of the subtype that this Event
		/// message is to have. The default value is the empty string.</param>
		/// <param name="world">- A pointer to the World.</param>
		explicit EventMessageAttributed(
			const std::string& subtype = std::string(), World* world = nullptr);

		/// <summary>
		/// This is the copy constructor for EventMessageAttributed. It has been
		/// defaulted.
		/// </summary>
		/// <param name="other">- The EventMessageAttributed to copy.</param>
		EventMessageAttributed(const EventMessageAttributed& other) = default;

		/// <summary>
		/// This is the copy assignment operator for EventMessageAttributed. It
		/// has been defaulted.
		/// </summary>
		/// <param name="other">- The EventMessageAttributed to copy into this
		/// one.</param>
		/// <returns>A reference to this object post-copy.</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed& other)
			= default;

		/// <summary>
		/// This is the move constructor for EventMessageAttributed. It calls
		/// the move constructor of its base class, and then it copies the World
		/// pointer from 'other', sets 'other's World pointer to nullptr, and
		/// moves the subtype string.
		/// </summary>
		/// <param name="other">- The EventMessageAttributed to move.</param>
		EventMessageAttributed(EventMessageAttributed&& other);

		/// <summary>
		/// This is the move assignment operator for EventMessageAttributed. It
		/// calls the move assignment operator of its base class, and then it
		/// moves the subtype, moves the World pointer, and sets the world
		/// pointer in 'other' to be nullptr.
		/// </summary>
		/// <param name="other">- The EventMessageAttributed to move into this
		/// one.</param>
		/// <returns>A reference to this object post-move.</returns>
		EventMessageAttributed& operator=(EventMessageAttributed&& other);

		/// <summary>
		/// This is the destructor for EventMessageAttributed. It has been
		/// defaulted.
		/// </summary>
		virtual ~EventMessageAttributed() override = default;
		
		/// <summary>
		/// Accessor for the World pointer stored in this object.
		/// </summary>
		/// <returns>A pointer to the World in which the Event that contains
		/// this object is to be published.</returns>
		World* GetWorld() const;

		/// <summary>
		/// Mutator for the World pointer stored in this object.
		/// </summary>
		/// <param name="world">- The World pointer to set this object's World
		/// pointer to.</param>
		void SetWorld(World* world);

		/// <summary>
		/// Accessor for the string representing the subtype of this
		/// EventMessageAttributed.
		/// </summary>
		/// <returns>The subtype of this EventMessageAttributed.</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Mutator for the string representing the subtype of this
		/// EventMessageAttributed.
		/// </summary>
		/// <param name="subtype">- The new subtype this EventMessageAttributed
		/// should be.</param>
		void SetSubtype(const std::string& subtype);

	private:

		std::string mSubtype;

		World* mWorld;
	};
}