#include "pch.h"
using namespace std;
namespace LibraryShared
{
	RTTI_DEFINITIONS(Action)

	Action::Action(const string& name) : mName(move(name))
	{
		uint64_t actionId = Action::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			actionId);
		RegisterAttribute(Signature("mName"s, DatumType::STRING, 1u,
			offsetof(Action, mName)), actionId);

		RegisterBaseType(actionId, Attributed::TypeIdClass());
	}

	Action::Action(Action&& other) : Attributed(move(other)),
		mName(move(other.mName))
	{}

	Action& Action::operator=(Action&& other)
	{
		if (&other != this)
		{
			Attributed::operator=(move(other));
			mName = move(other.mName);
		}
		return *this;
	}

	void Action::SetName(const string& name)
	{
		mName = name;
	}
	
	const string& Action::Name() const
	{
		return mName;
	}

	Entity* Action::GetEntity() const
	{
		Scope* parent = GetParent();
		if (parent == nullptr)
		{
			return nullptr;
		}
		if (!parent->Is(Entity::TypeIdClass()))
		{
			return nullptr;
		}
		return parent->As<Entity>();
	}

	void Action::SetEntity(Entity& entity)
	{
		/*In SetEntity, we need to make sure the old Entity's list of Actions no
		longer contains this Action.*/
		Scope* const oldParent = GetParent();
		if (oldParent != nullptr)
		{
			Datum* const datumOfActions = oldParent->Find("Actions"s);
			assert(datumOfActions != nullptr);
			datumOfActions->Remove(*this);
		}
		/*We also need to make sure the new Sector's list does contain this
		Entity and that the Scope tree is properly updated. This can be done
		with a call to Adopt.*/
		entity.Adopt(mName, *this);
	}
}