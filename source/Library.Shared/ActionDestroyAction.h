#pragma once

#include "Action.h"

namespace LibraryShared
{
	/// <summary>
	/// This is a concrete subclass of Action. Its action in its Update method
	/// is to remove an Action instance of a desired name from this Action's
	/// containing list of Actions.
	/// </summary>
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action);
	public:
		/// <summary>
		/// This is the only constructor of ActionDestroyAction. It takes two
		/// optional arguments; the second sets the name of this Action, and the
		/// first specifies the name of the Action this ActionDestroyAction
		/// should destroy. 
		/// </summary>
		/// <param name="actionToDestroy">- The name of the Action to destroy.
		/// </param>
		/// <param name="nameOfThis">- The name of this Action.</param>
		explicit ActionDestroyAction(
			const std::string& actionToDestroy = std::string(),
			const std::string& nameOfThis = std::string());

		/// <summary>
		/// The copy constructor for ActionDestroyAction has been deleted.
		/// </summary>
		/// <param name=""></param>
		ActionDestroyAction(const ActionDestroyAction&) = delete;

		/// <summary>
		/// The copy assignment operator for ActionDestroyAction has been
		/// deleted.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ActionDestroyAction& operator=(const ActionDestroyAction&) = delete;

		/// <summary>
		/// The move constructor for ActionDestroyAction calls the move
		/// constructor for Action and then moves the name of the Action that
		/// this Action destroys.
		/// </summary>
		/// <param name="other">- The ActionDestroyAction to move from.</param>
		ActionDestroyAction(ActionDestroyAction&& other);

		/// <summary>
		/// The move assignment operator for ActionDestroyAction calls the move
		/// assignment operator for Action and then moves the name of the Action
		/// that this Action destroys.
		/// </summary>
		/// <param name="other">- The ActionDestroyAction to move from.</param>
		/// <returns>A reference to this object.</returns>
		ActionDestroyAction& operator=(ActionDestroyAction&& other);

		/// <summary>
		/// The destructor for ActionDestroyAction has been defaulted.
		/// </summary>
		virtual ~ActionDestroyAction() override = default;

		/// <summary>
		/// This is the Update method for ActionDestroyAction. It stores
		/// pointers to itself and the specified Action to destroy in the delete
		/// queue, so they will be destroyed at the beginning of the next frame.
		/// </summary>
		/// <param name="worldState">- The WorldState being used in this Update
		/// tree.</param>
		virtual void Update(WorldState& worldState) override;
		
		/// <summary>
		/// Accessor for mAction, the name of the Action that is to be destroyed
		/// by this Action.
		/// </summary>
		/// <returns>The name of the Action that is to be destroyed by this
		/// Action.</returns>
		const std::string& GetAction() const;

		/// <summary>
		/// Mutator for mAction, the name of the Action that is to be destroyed
		/// by this Action.
		/// </summary>
		/// <param name="action">- The name of the Action that is to be
		/// destroyed by this Action.</param>
		void SetAction(const std::string& action);

	private:
		std::string mAction;
	};
}