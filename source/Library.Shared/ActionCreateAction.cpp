#include "pch.h"

using namespace std;

namespace LibraryShared
{
	RTTI_DEFINITIONS(ActionCreateAction);
	ActionCreateAction::ActionCreateAction(const string& nameOfThis,
		const string& prototype, const string& instanceName) :
		Action(nameOfThis), mPrototype(move(prototype)),
		mInstanceName(move(instanceName))
	{
		const uint64_t actionCreateActionId = ActionCreateAction::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			actionCreateActionId);
		RegisterAttribute(Signature("mPrototype"s, DatumType::STRING, 1u, 
			offsetof(ActionCreateAction, mPrototype)), actionCreateActionId);
		RegisterAttribute(Signature("mInstanceName"s, DatumType::STRING, 1u,
			offsetof(ActionCreateAction, mInstanceName)), actionCreateActionId);

		RegisterBaseType(actionCreateActionId, Action::TypeIdClass());
	}

	ActionCreateAction::ActionCreateAction(ActionCreateAction&& other) :
		Action(move(other)), mPrototype(move(other.mPrototype)),
		mInstanceName(move(other.mInstanceName))
	{}

	ActionCreateAction& ActionCreateAction::operator=
		(ActionCreateAction&& other)
	{
		if (this != &other)
		{
			Action::operator=(move(other));
			mInstanceName = move(other.mInstanceName);
			mPrototype = move(other.mPrototype);
		}
		return *this;
	}

	const string& ActionCreateAction::Prototype() const
	{
		return mPrototype;
	}

	const string& ActionCreateAction::InstanceName() const
	{
		return mInstanceName;
	}

	void ActionCreateAction::SetPrototype(const string& prototype)
	{
		mPrototype = move(prototype);
	}

	void ActionCreateAction::SetInstanceName(const string& instanceName)
	{
		mInstanceName = move(instanceName);
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		worldState.SetAction(*this);

		/*First, I must create the new Action, using mPrototype and
		mInstanceName appropriately.*/
		Action* const newAction = Factory<Action>::Create(mPrototype);
		newAction->SetName(mInstanceName);

		/*Next, I must have the parent adopt the new Action into its container
		of Actions. This can be done with call to Adopt on the parent.*/
		Scope* const parent = GetParent();
		assert(parent != nullptr);
		parent->Adopt(mInstanceName, *newAction);

		/*Finally, I need to add this object to the list of Scopes to be deleted
		in the next frame.*/
		WorldState::AddToDeleteList(*this);
	}
}