#pragma once
#include "Action.h"
namespace LibraryShared
{
	/// <summary>
	/// This class is a list of Actions, which is itself an Action, as it
	/// inherits from Action. Because an Action can now be either a singular
	/// Action or an ActionList, this class constitutes an example of the
	/// Composite design pattern.
	/// </summary>
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);
	public:
		/// <summary>
		/// This is the constructor for ActionList. It registers a prescribed
		/// attribute called "Actions", which will store the Actions that this
		/// list contains. The input string is passed to the Action constructor.
		/// </summary>
		/// <param name="name">- The name this Action should have, passed to the
		/// Action constructor, which is called before this constructor.</param>
		explicit ActionList(const std::string& name = std::string());

		/// <summary>
		/// The copy constructor for ActionList has been deleted.
		/// </summary>
		/// <param name=""></param>
		ActionList(const ActionList&) = delete;

		/// <summary>
		/// The copy assignment operator for ActionList has been deleted.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ActionList& operator=(const ActionList&) = delete;

		/// <summary>
		/// The move constructor for ActionList just calls the move constructor
		/// of Action.
		/// </summary>
		/// <param name="other">- The ActionList to move from.</param>
		ActionList(ActionList&& other);

		/// <summary>
		/// The move assignment operator for ActionList simply calls the move
		/// assignment operator of Action.
		/// </summary>
		/// <param name="other">- The ActionList to move from.</param>
		/// <returns>A reference to this ActionList, post-move.</returns>
		ActionList& operator=(ActionList&& other);

		/// <summary>
		/// The destructor for ActionList has been defaulted.
		/// </summary>
		virtual ~ActionList() override = default;

		/// <summary>
		/// This concrete implementation of Update, inherited as a pure virtual
		/// method from Action, calls the Update method of each Action contained
		/// in this ActionList.
		/// </summary>
		/// <param name="worldState">- The state of the World.</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// This method returns a reference to the Datum in this ActionList that
		/// contains all the Actions in this ActionList.
		/// </summary>
		/// <returns>A reference to the Datum that contains all the Actions
		/// contained in this ActionList.</returns>
		Datum& Actions() const;
	};
}