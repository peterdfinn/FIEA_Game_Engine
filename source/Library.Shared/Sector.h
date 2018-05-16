#pragma once
#include "RTTI.h"
#include "Factory.h"
namespace LibraryShared
{
	class World;

	/// <summary>
	/// This is a Sector. It is an organizational container to be used in
	/// creating a game's world. It is contained within a World, and it
	/// may contain any number of Entities. A Sector may also contain Actions.
	/// </summary>
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed);
	public:
		/// <summary>
		/// This the constructor for Sector. It takes a name for this Sector and
		/// a pointer to the World that should own this Sector.
		/// </summary>
		/// <param name="name">- The name this Sector should have.</param>
		/// <param name="world">- A pointer to the World to which this Sector
		/// should belong.</param>
		explicit Sector(std::string name = "", World* world = nullptr);

		/// <summary>
		/// This is the copy constructor for Sector. It has been deleted.
		/// </summary>
		/// <param name="other"></param>
		Sector(const Sector& other) = delete;

		/// <summary>
		/// This is the copy assignment operator for Sector. It has been
		/// deleted.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		Sector& operator=(const Sector& other) = delete;

		/// <summary>
		/// This is the move constructor for Sector. It is designed to supplant
		/// 'other' with this Sector; the parent of 'other' will now point to
		/// this object instead of 'other', and all children of 'other' will be
		/// children of this object instead. Note that this means the World
		/// relinquishes ownership of 'other', placing its ownership de facto in
		/// the hands of the caller.
		/// </summary>
		/// <param name="other">- The Sector to be moved.</param>
		Sector(Sector&& other);

		/// <summary>
		/// This is the move assignment operator for Sector. It has all the
		/// functionality of the move constructor, so see that for info about
		/// this method. Furthermore, this method will also cause this object
		/// to relinquish ownership of its child Scopes, taking on 'other's
		/// children instead.
		/// </summary>
		/// <param name="other">- The Sector to move.</param>
		/// <returns>A reference to this Sector.</returns>
		Sector& operator=(Sector&& other);

		/// <summary>
		/// This is the destructor for Sector. It has been defaulted.
		/// </summary>
		virtual ~Sector() override = default;

		/// <summary>
		/// Accessor for this Sector's name.
		/// </summary>
		/// <returns>This Sector's name.</returns>
		const std::string& Name() const;

		/// <summary>
		/// Mutator for this Sector's name.
		/// </summary>
		/// <param name="name">- The new name this Sector should have.</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Mutator for the World that owns this Sector. This method ensures
		/// that the new World points to this Sector as a child, that the old
		/// World no longer considers this Sector a child, and that this
		/// Sector's pointer to its parent is updated.
		/// </summary>
		/// <param name="world">- The world that should adopt this Sector as its
		/// child.</param>
		void SetWorld(World& world);

		/// <summary>
		/// Accessor for the World that owns this Sector.
		/// </summary>
		/// <returns>A pointer to the World that owns this Sector.</returns>
		World* GetWorld() const;

		/// <summary>
		/// This method returns a reference to the Datum in this Sector that
		/// contains pointers to all of the Entities that are children of this
		/// Sector.
		/// </summary>
		/// <returns>The Datum in this Sector that contains all the Entities in
		/// this Sector.</returns>
		Datum& Entities() const;

		/// <summary>
		/// This method returns a reference to the Datum in this Sector that
		/// contains pointers to all of the Actions that are children of this
		/// Sector.
		/// </summary>
		/// <returns>The Datum in this Sector that contains all the Actions in
		/// this Sector.</returns>
		Datum& Actions() const;

		/// <summary>
		/// This method returns a const pointer to a heap-allocated object of a
		/// derived class of Entity. 'className' should be the name of the class
		/// to instantiate, and 'instanceName' should be the name of the
		/// instance created. The newly created Entity is then adopted into this
		/// Sector.
		/// </summary>
		/// <param name="className">- The name of the derived class you wish to
		/// instantiate.</param>
		/// <param name="instanceName">- The name the returned instance will
		/// have.</param>
		/// <returns>- A pointer to a dynamically allocated Entity (or, rather,
		/// derived class thereof).</returns>
		Entity* const CreateEntity(const std::string& className,
			const std::string& instanceName);

		/// <summary>
		/// Update each Entity in this Sector.
		/// </summary>
		/// <param name="worldState">- Metadata to pass through the Update tree.
		/// </param>
		void Update(WorldState& worldState) const;

	private:
		std::string mName;
	};
}