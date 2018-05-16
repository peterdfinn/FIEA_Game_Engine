#pragma once
#include "Attributed.h"

namespace LibraryShared
{
	class Entity;

	/// <summary>
	/// This class encapsulates an action as an object. This class should never
	/// be directly instantiated; rather, it acts as a virtual base from which
	/// concrete Actions inherit. Any inheriting classes must override the
	/// 'Update' method.
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);
	public:
		/// <summary>
		/// This is the constructor for Action. It isn't used to instantiate
		/// the Action class, as this class is pure virtual; rather, it exists
		/// so that it can be called by constructors of concrete derived
		/// classes. It sets the name and owning Entity of this Action according
		/// to its arguments.
		/// </summary>
		/// <param name="name">- The name this Action should have. Default value
		/// is an empty string.</param>
		/// <param name="entity">- A pointer to the Entity that should own this
		/// Action. Default value is nullptr.</param>
		explicit Action(const std::string& name = std::string());

		/// <summary>
		/// The copy constructor for Action has been deleted.
		/// </summary>
		/// <param name=""></param>
		Action(const Action&) = delete;

		/// <summary>
		/// The copy assignment operator for Action has been deleted.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		Action& operator=(const Action&) = delete;

		/// <summary>
		/// The move constructor for Action steals the data from 'other'.
		/// </summary>
		/// <param name="other">- The Action from which to steal data.</param>
		Action(Action&& other);

		/// <summary>
		/// The move assignment operator of Action steals the data of 'other'
		/// and relinquishes ownership of its children.
		/// </summary>
		/// <param name="other">- The Action from which to steal data.</param>
		/// <returns>A reference to this Action.</returns>
		Action& operator=(Action&& other);

		/// <summary>
		/// The destructor for Action has been defaulted.
		/// </summary>
		virtual ~Action() override = default;

		/// <summary>
		/// This is a pure virtual method that exists entirely to be overridden
		/// by derived classes. This method should contain all actions that
		/// this Action is to represent.
		/// </summary>
		/// <param name="state">- The state of the World.</param>
		virtual void Update(WorldState& state) = 0;

		/// <summary>
		/// Accessor for this Action's name.
		/// </summary>
		/// <returns>The name of this Action.</returns>
		const std::string& Name() const;

		/// <summary>
		/// Mutator for this Action's name.
		/// </summary>
		/// <param name="name"></param>
		void SetName(const std::string& name);

		/// <summary>
		/// Mutator for this Action's owning Entity.
		/// </summary>
		/// <param name="entity">- The Entity that should own this Action.
		/// </param>
		void SetEntity(Entity& entity);

		/// <summary>
		/// Accessor for this Action's owning Entity.
		/// </summary>
		/// <returns>The Entity that owns this Action, if there is one; nullptr
		/// otherwise.</returns>
		Entity* GetEntity() const;
	
	private:

		std::string mName;
	};
}