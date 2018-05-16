#pragma once

#include <memory>

#include "Attributed.h"
#include "GameClock.h"
#include "EventQueue.h"

namespace LibraryShared
{
	class EventPublisher;

	template <typename T>
	class Event;

	/// <summary>
	/// The World class is the uppermost container for game objects. A World may
	/// contain multiple Sectors or Actions, a Sector may contain multiple
	/// Entities or Actions, and an Entity is an object in the world, which may
	/// contain multiple Actions.
	/// </summary>
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);
	public:
		/// <summary>
		/// This is the standard constructor for a World.
		/// </summary>
		/// <param name="name">- The name this World should have.</param>
		explicit World(const std::string& name = std::string());

		/// <summary>
		/// The copy constructor for World has been deleted.
		/// </summary>
		/// <param name="other"></param>
		World(const World& other) = delete;

		/// <summary>
		/// The copy assignment operator for World has been deleted.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		World& operator=(const World& other) = delete;

		/// <summary>
		/// The move constructor for World steals all of the other World's
		/// data, including its name and all of its attributes.
		/// </summary>
		/// <param name="other">- The World to move into this one.</param>
		World(World&& other);

		/// <summary>
		/// The move assignment operator for World steals all of the other
		/// World's data. Additionally, if this World has any Sectors already,
		/// this method makes this World relinquish owernship of them, meaning
		/// it is the responsibility of the user to ensure they are deleted.
		/// </summary>
		/// <param name="other">- The World whose data to steal.</param>
		/// <returns>A reference to this World.</returns>
		World& operator=(World&& other);

		/// <summary>
		/// The destructor for World has been defaulted.
		/// </summary>
		virtual ~World() override = default;

		/// <summary>
		/// Accessor for World name.
		/// </summary>
		/// <returns>The name of this World.</returns>
		const std::string& Name() const;

		/// <summary>
		/// Mutator for World name.
		/// </summary>
		/// <param name="name">- The new name this World shall have.</param>
		void SetName(const std::string& name);

		/// <summary>
		/// This method returns a reference to the Datum in this World's Scope
		/// that contains pointers to the Sectors this world contains. If there
		/// is no such Datum (which may happen if this World was moved into
		/// another one, thus stealing all Datums from it), an exception is
		/// thrown.
		/// </summary>
		/// <returns>The Datum in this World that contains pointers to all of
		/// the Sectors in this World.</returns>
		Datum& Sectors() const;

		/// <summary>
		/// This method returns a reference to the Datum in this World's Scope
		/// that contains pointers to the Action this world contains. If there
		/// is no such Datum (which may happen if this World was moved into
		/// another one, thus stealing all Datums from it), an exception is
		/// thrown.
		/// </summary>
		/// <returns>The Datum in this World that contains pointers to all of
		/// the Actions in this World.</returns>
		Datum& Actions() const;
		
		/// <summary>
		/// World-level method in Update tree.
		/// </summary>
		/// <param name="worldState">Metadata for the Update tree to use.
		/// </param>
		void Update(WorldState& worldState);

		/// <summary>
		/// This method returns a const pointer to a heap-allocated Sector. The
		/// newly created Sector is then adopted into this World.
		/// </summary>
		/// <param name="instanceName">- The name the returned Sector will have.
		/// </param>
		/// <returns>A pointer to a dynamically allocated Sector.</returns>
		Sector* const CreateSector(const std::string& instanceName);

		const EventQueue& GetEventQueue() const;
		EventQueue& GetEventQueue();

	private:
		EventQueue mEventQueue;
		GameClock mClock;
		std::string mName;
	};
}