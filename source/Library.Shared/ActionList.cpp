#include "pch.h"
using namespace std;
namespace LibraryShared
{
	RTTI_DEFINITIONS(ActionList);
	ActionList::ActionList(const string& name) : Action(name)
	{
		const uint64_t actionListId = ActionList::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			actionListId);
		RegisterAttribute(Signature("Actions"s, DatumType::TABLE, 0u, 0u),
			actionListId, true, false);

		RegisterBaseType(actionListId, Action::TypeIdClass());
	}

	ActionList::ActionList(ActionList&& other) : Action(move(other))
	{}

	ActionList& ActionList::operator=(ActionList&& other)
	{
		if (this != &other)
		{
			if (&other != this)
			{
				Action::operator=(move(other));
			}
		}
		return *this;
	}

	void ActionList::Update(WorldState& worldState)
	{
		Datum& actionsDatum = Actions();
		const uint32_t numberOfActions = actionsDatum.Size();
		for (uint32_t i = 0u; i < numberOfActions; ++i)
		{
			Scope& actionScope = actionsDatum.GetScope(i);
			assert(actionScope.Is(Action::TypeIdClass()));
			actionScope.As<Action>()->Update(worldState);
		}
	}

	Datum& ActionList::Actions() const
	{
		Datum* const actionsDatumPtr = Find("Actions"s);
		if (actionsDatumPtr == nullptr)
		{
			throw exception("This ActionList has no Actions container! Did you\
				move it recently?");
		}
		return *actionsDatumPtr;
	}
}