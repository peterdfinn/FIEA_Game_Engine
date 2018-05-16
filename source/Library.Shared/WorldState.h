#pragma once

#include "Vector.h"

namespace LibraryShared
{
	class Action;

	/// <summary>
	/// The WorldState class is used for storing metadata that is passed around
	/// to participants in the update tree in each frame of the game. A
	/// WorldState object contains pointers to the current GameTime object, the
	/// current World, the Sector within the World that is currently being
	/// updated, the Entity within the World that is currently being updated,
	/// and the Action within a World, Sector, or Entity that is currently being
	/// updated. The class also contains a static list of Scope objects that are
	/// to be destroyed because of the frame currently being generated.
	/// </summary>
	class WorldState final
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		WorldState() = default;

		/// <summary>
		/// This constructor allows a pointer to a GameTime to be set.
		/// </summary>
		/// <param name="time">- The GameTime object to be accessible from this
		/// WorldState object.</param>
		explicit WorldState(GameTime& time);

		/// <summary>
		/// The destructor for WorldState has been defaulted.
		/// </summary>
		~WorldState() = default;

		/// <summary>
		/// Accessor for GameTime pointer.
		/// </summary>
		/// <returns>A pointer to the GameTime associated with this WorldState
		/// object.</returns>
		GameTime* GetGameTime() const;

		/// <summary>
		/// Mutator for GameTime.
		/// </summary>
		/// <param name="time">- The GameTime object that should be associated
		/// with this WorldState object.</param>
		void SetGameTime(GameTime& time);

		/// <summary>
		/// Accessor for the World pointer.
		/// </summary>
		/// <returns>A pointer to the World currently being updated, or nullptr
		/// if the World has not yet been assigned.</returns>
		World* GetWorld() const;

		/// <summary>
		/// Mutator for the World.
		/// </summary>
		/// <param name="world">- The World within which a frame is being
		/// updated.</param>
		void SetWorld(World& world);

		/// <summary>
		/// Mutator for the Entity currently being updated.
		/// </summary>
		/// <param name="entity">- The Entity that is currently being updated.
		/// </param>
		void SetEntity(Entity& entity);

		/// <summary>
		/// Mutator for the Action currently being updated.
		/// </summary>
		/// <param name="action">- The Action that is currently being updated.
		/// </param>
		void SetAction(Action& action);

		/// <summary>
		/// This method adds a Scope object to the static list of objects to be
		/// deleted at the beginning of the next frame.
		/// </summary>
		/// <param name="toDelete">- The object to delete.</param>
		static void AddToDeleteList(Scope& toDelete);

		/// <summary>
		/// This method deletes all Scope objects currently referenced in the
		/// static delete list.
		/// </summary>
		static void DestroyScopes();

	private:
		static Vector<Scope*> mDeleteList;
		class GameTime* mGameTime{nullptr};
		class World* mWorld{nullptr};
		class Sector* mSector{nullptr};
		class Entity* mEntity{nullptr};
		Action* mAction{nullptr};
	};
}