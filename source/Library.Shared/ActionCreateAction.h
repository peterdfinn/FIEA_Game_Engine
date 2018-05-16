#pragma once
#include "Action.h"
namespace LibraryShared
{
	/// <summary>
	/// This class is a concrete Action. The action it takes in its Update
	/// method is to instantiate an Action of a specified Action subclass, add
	/// it to this Action's containing list of Actions, and then remove itself
	/// from that list.
	/// </summary>
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action);
	public:
		/// <summary>
		/// This is the constructor for ActionCreateAction. It takes three
		/// arguments: the name this Action should have, the name of the class
		/// that this Action should instantiate, and the name of the
		/// aforementioned instantiated instance.
		/// </summary>
		/// <param name="nameOfThis">- The name this Action should have.</param>
		/// <param name="prototype">- The name of the derived class of Action to
		/// instantiate.</param>
		/// <param name="instanceName">- The name the newly created instance
		/// should have.</param>
		explicit ActionCreateAction(
			const std::string& nameOfThis = std::string(),
			const std::string& prototype = std::string(),
			const std::string& instanceName = std::string());

		/// <summary>
		/// The copy constructor for ActionCreateAction has been deleted.
		/// </summary>
		/// <param name=""></param>
		ActionCreateAction(const ActionCreateAction&) = delete;

		/// <summary>
		/// The copy assignment operator for ActionCreateAction has been
		/// deleted.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ActionCreateAction& operator=(const ActionCreateAction&) = delete;

		/// <summary>
		/// The move constructor for ActionCreateAction calls the move
		/// constructor for Action, and then it moves its two string members.
		/// </summary>
		/// <param name="other">- The ActionCreateAction to move from.</param>
		ActionCreateAction(ActionCreateAction&& other);

		/// <summary>
		/// The move assignment operator for ActionCreateAction calls the
		/// move assignment operator of Action, and then it moves its two string
		/// members.
		/// </summary>
		/// <param name="other">- The ActionCreateAction to move from.</param>
		/// <returns>A reference to this ActionCreateAction, post-move.
		/// </returns>
		ActionCreateAction& operator=(ActionCreateAction&& other);

		/// <summary>
		/// The destructor for ActionCreateAction has been defauted.
		/// </summary>
		virtual ~ActionCreateAction() override = default;

		/// <summary>
		/// Accessor for the name of the class that is to be instantiated.
		/// </summary>
		/// <returns>The name of the class this Action is to instantiate.
		/// </returns>
		const std::string& Prototype() const;

		/// <summary>
		/// Mutator for the name of the class that is to be instantiated.
		/// </summary>
		/// <param name="prototype">- The new name of the class that is to be
		/// instantiated.</param>
		void SetPrototype(const std::string& prototype);

		/// <summary>
		/// Accessor for the name of the instance that is to be created.
		/// </summary>
		/// <returns>The name of the instance that will be created by this
		/// Action.</returns>
		const std::string& InstanceName() const;

		/// <summary>
		/// Mutator for the name of the instance this Action creates.
		/// </summary>
		/// <param name="instanceName">- The name this Action will ascribe to
		/// the Action created.</param>
		void SetInstanceName(const std::string& instanceName);

		/// <summary>
		/// This Update method creates a new Action of the appropriate class
		/// name and adds it to the list of Actions that contains this Action.
		/// It then marks itself for removal in the next frame.
		/// </summary>
		/// <param name="worldState">- The state of the World.</param>
		virtual void Update(WorldState& worldState) override;

	private:
		std::string mPrototype;
		std::string mInstanceName;
	};
}