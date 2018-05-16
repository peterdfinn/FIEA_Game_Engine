#pragma once
#include "Factory.h"
#include "Attributed.h"
namespace LibraryShared
{
	class Action;
	class Sector;
	class WorldState;

	/// <summary>
	/// The Entity class is the base class from which all objects in a level may
	/// inherit (it's analogous to a GameObject in Unity or an Actor in UE4).
	/// </summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed);
	public:
		/// <summary>
		/// This is the standard constructor for an Entity. It takes a string
		/// that shall represent the name of the constructed Entity and a
		/// pointer to the Sector that should contain this Entity.
		/// </summary>
		/// <param name="name">- The name of the newly constructed Entity.
		/// </param>
		/// <param name="sector">- A pointer to the Sector that should contain
		/// this Entity.</param>
		explicit Entity(std::string name = "", Sector* sector = nullptr);

		/// <summary>
		/// This is the copy constructor for Entity. It calls the copy
		/// constructor of Attributed and then copies the name and the pointer
		/// to the Sector that contains this Entity.
		/// </summary>
		/// <param name="other">- A reference to the Entity to copy.</param>
		Entity(const Entity& other) = delete;

		/// <summary>
		/// This is the copy assignment operator for Entity. It calls the copy
		/// assignment operator for Attributed, and then it copies Entity's
		/// only member variable: the Entity's name. Finally, it adopts this
		/// Entity into the Sector that owns 'other'.
		/// </summary>
		/// <param name="other">- The Entity to copy.</param>
		/// <returns>A reference to this Entity.</returns>
		Entity& operator=(const Entity& other) = delete;

		/// <summary>
		/// This is the move constructor for Entity. It calls the move
		/// constructor of Attributed and then moves the name of this Entity and
		/// copies the Sector pointer, setting the Sector pointer in 'other' to
		/// nullptr.
		/// </summary>
		/// <param name="other">- The Entity to move.</param>
		Entity(Entity&& other);

		/// <summary>
		/// This is the move assignment operator for Entity. It calls the move
		/// assignment operator for Attributed, and then it moves Entity's
		/// member variables: the Entity's name and a pointer to the Entity's
		/// owning Sector.
		/// </summary>
		/// <param name="other">- The Entity to move.</param>
		/// <returns>A reference to this Entity.</returns>
		Entity& operator=(Entity&& other);

		/// <summary>
		/// The destructor for Entity has been defaulted.
		/// </summary>
		virtual ~Entity() override = default;

		/// <summary>
		/// This method returns this Entity's name.
		/// </summary>
		/// <returns>This Entity's name.</returns>
		std::string Name() const;

		/// <summary>
		/// This method sets the name of this Entity.
		/// </summary>
		/// <param name="name">- The new name this Entity should have.</param>
		/// <returns>The new name.</returns>
		std::string SetName(const std::string& name);

		/// <summary>
		/// This method returns the address of the Sector that owns this Entity.
		/// There is also a const version of this method that returns a const
		/// pointer, as opposed to this method, which returns a non-const
		/// pointer.
		/// </summary>
		/// <returns>The address of the Sector that owns this Entity.</returns>
		Sector* GetSector() const;

		/// <summary>
		/// This method marks this Entity as being owned by the Sector
		/// referenced by 'newSector'.
		/// </summary>
		/// <param name="newSector">- A reference to the Sector to which this
		/// Entity shall now belong.</param>
		void SetSector(Sector& newSector);

		/// <summary>
		/// This method creates a new Action of the derived class denoted by
		/// 'className' and adopts it into this Entity with name 'instanceName'.
		/// </summary>
		/// <param name="className">- The name of the derived class of Action
		/// that the new Action should be an object of.</param>
		/// <param name="instanceName">- The name this new Action should have.
		/// </param>
		/// <returns></returns>
		Action* const CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// This method returns the Datum that contains all Actions owned by
		/// this Entity.
		/// </summary>
		/// <returns>The Datum that contains all Actions owned by this Entity.
		/// </returns>
		Datum& Actions() const;

		/// <summary>
		/// This method is called every frame of the game.
		/// </summary>
		/// <param name="worldState">- The state information of the World.
		/// </param>
		void Update(WorldState& worldState);

	private:
		std::string mName;
	};

	ConcreteFactory(Entity, Entity);
}