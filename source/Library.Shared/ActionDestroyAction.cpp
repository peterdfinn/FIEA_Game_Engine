#include "pch.h"

using namespace std;

namespace LibraryShared
{
	RTTI_DEFINITIONS(ActionDestroyAction);
	ActionDestroyAction::ActionDestroyAction(const string& actionToDestroy,
		const string& nameOfThis) : Action(nameOfThis),
		mAction(move(actionToDestroy))
	{
		const uint64_t actionDestroyActionId =
			ActionDestroyAction::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			actionDestroyActionId);
		RegisterAttribute(Signature("mAction"s, DatumType::STRING, 1u,
			offsetof(ActionDestroyAction, mAction)), actionDestroyActionId);

		RegisterBaseType(actionDestroyActionId, Action::TypeIdClass());
	}

	ActionDestroyAction::ActionDestroyAction(ActionDestroyAction&& other) :
		Action(move(other))
	{
		mAction = move(other.mAction);
	}

	ActionDestroyAction& ActionDestroyAction::operator=(
		ActionDestroyAction&& other)
	{
		if (this != &other)
		{
			Action::operator=(move(other));
			mAction = move(other.mAction);
		}
		return *this;
	}

	void ActionDestroyAction::Update(WorldState& worldState)
	{
		worldState.SetAction(*this);

		/*This method simply stores its address and the address of the action to
		be destroyed into the delete queue.*/
		const Scope* const parent = GetParent();
		if (parent == nullptr)
		{
			throw exception("Actions must have parents before they can be\
				used!");
		}
		Datum* const actionsContainer = parent->Find("Actions"s);
		assert(actionsContainer != nullptr);
		uint32_t size = actionsContainer->Size();
		for (uint32_t i = 0u; i < size; ++i)
		{
			Action* const currentAction = actionsContainer->GetScope(i).As<Action>();
			assert(currentAction != nullptr);
			if (currentAction->Name() == mAction)
			{
				WorldState::AddToDeleteList(*currentAction);
				break;
			}
		}
		WorldState::AddToDeleteList(*this);
	}

	const string& ActionDestroyAction::GetAction() const
	{
		return mAction;
	}

	void ActionDestroyAction::SetAction(const string& action)
	{
		mAction = action;
	}
}