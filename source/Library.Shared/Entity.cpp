#include "pch.h"
using namespace std;
namespace LibraryShared
{
	RTTI_DEFINITIONS(Entity);

	Entity::Entity(string name, Sector* sector) : mName(name)
	{
		if (sector != nullptr)
		{
			SetSector(*sector);
		}
		const uint64_t entityId = Entity::TypeIdClass();
		RegisterAttribute(Signature("this", DatumType::POINTER, 1u, 0u),
			entityId);
		RegisterAttribute(Signature("mName"s, DatumType::STRING, 1u,
			offsetof(Entity, mName)), entityId);
		RegisterAttribute(Signature("Actions"s, DatumType::TABLE, 0u, 0u),
			entityId, true, false);

		RegisterBaseType(entityId, Attributed::TypeIdClass());
	}

	Entity::Entity(Entity&& other) : Attributed(move(other)), mName(move(other.mName))
	{}

	Entity& Entity::operator=(Entity&& other)
	{
		if (&other != this)
		{
			Attributed::operator=(move(other));
			mName = move(other.mName);
		}
		return *this;
	}

	string Entity::Name() const
	{
		return mName;
	}

	string Entity::SetName(const string& name)
	{
		mName = name;
		return mName;
	}

	Sector* Entity::GetSector() const
	{
		Scope* parent = GetParent();
		if (parent == nullptr)
		{
			return nullptr;
		}
		assert(parent->Is(Sector::TypeIdClass()));
		return parent->As<Sector>();
	}

	void Entity::SetSector(Sector& newSector)
	{
		/*In SetSector, we need to make sure the old Sector's list of Entities
		no longer contains this Entity.*/
		Sector* const oldSector = GetSector();
		if (oldSector != nullptr)
		{
			Datum* const datumOfEntities = oldSector->Find("Entities");
			assert(datumOfEntities != nullptr);
			datumOfEntities->Remove(*this);
		}
		/*We also need to make sure the new Sector's list does contain this
		Entity and that the Scope tree is properly updated. This can be done
		with a call to Adopt.*/
		newSector.Adopt(mName, *this);
	}

	Datum& Entity::Actions() const
	{
		Datum* const actionsDatumPtr = Find("Actions"s);
		if (actionsDatumPtr == nullptr)
		{
			throw exception("This Entity has no Actions container! Did you\
				move it recently?");
		}
		return *actionsDatumPtr;
	}

	Action* const Entity::CreateAction(const string& className,
		const string& instanceName)
	{
		Action* const actionPtr = Factory<Action>::Create(className);
		if (actionPtr == nullptr)
		{
			throw exception("There is no factory for Actions of the requested\
				derived class!");
		}
		actionPtr->SetName(instanceName);
		actionPtr->SetEntity(*this);
		return actionPtr;
	}

	void Entity::Update(WorldState& worldState)
	{
		worldState.SetEntity(*this);
		Datum& actionsDatum = Actions();
		uint32_t actionsDatumSize = actionsDatum.Size();
		for (uint32_t i = 0u; i < actionsDatumSize; ++i)
		{
			actionsDatum.GetScope(i).As<Action>()->Update(worldState);
		}
	}
}