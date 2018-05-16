#include "pch.h"
using namespace std;
namespace LibraryShared
{
	RTTI_DEFINITIONS(Sector);

	Sector::Sector(string name, World* world) : mName(name)
	{
		if (world != nullptr)
		{
			SetWorld(*world);
		}

		uint64_t sectorId = Sector::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u), sectorId);
		RegisterAttribute(Signature("mName"s, DatumType::STRING, 1u, offsetof(Sector, mName)), sectorId);
		RegisterAttribute(Signature("Entities"s, DatumType::TABLE, 0u, 0u), sectorId, true, false);
		RegisterAttribute(Signature("Actions"s, DatumType::TABLE, 0u, 0u), sectorId, true, false);

		RegisterBaseType(sectorId, Attributed::TypeIdClass());
	}

	Sector::Sector(Sector&& other) : Attributed(move(other)), mName(move(other.mName))
	{}

	Sector& Sector::operator=(Sector&& other)
	{
		if (&other != this)
		{
			Attributed::operator=(move(other));
			mName = move(other.mName);
		}
		return *this;
	}

	const string& Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const string& name)
	{
		mName = name;
	}

	void Sector::SetWorld(World& world)
	{
		/*In SetWorld, we need to make sure the old World's list of Sectors no
		longer contains this Sector.*/
		const World* const oldWorld = GetWorld();
		if (oldWorld != nullptr)
		{
			Datum* const datumOfSectors = oldWorld->Find("Sectors"s);
			assert(datumOfSectors != nullptr);
			datumOfSectors->Remove(*this);
		}
		/*We also need to make sure the new World's list does contain this
		Sector and that the Scope tree is properly updated. This can be done
		with a call to Adopt.*/
		world.Adopt(mName, *this);
	}

	World* Sector::GetWorld() const
	{
		Scope* const parent = GetParent();
		if (parent == nullptr)
		{
			return nullptr;
		}
		assert(parent->Is(World::TypeIdClass()));
		return parent->As<World>();
	}

	Datum& Sector::Entities() const
	{
		Datum* const entitiesDatumPtr = Find("Entities"s);
		if (entitiesDatumPtr == nullptr)
		{
			throw exception("This Sector has no Entities container! Did you\
				move it recently?");
		}
		return *entitiesDatumPtr;
	}

	Datum& Sector::Actions() const
	{
		Datum* const actionsDatumPtr = Find("Actions"s);
		if (actionsDatumPtr == nullptr)
		{
			throw exception("This Entity has no Actions container! Did you\
				move it recently?");
		}
		return *actionsDatumPtr;
	}

	Entity* const Sector::CreateEntity(const string& className,
		const string& instanceName)
	{
		Entity* const entityPtr = Factory<Entity>::Create(className);
		entityPtr->SetName(instanceName);
		entityPtr->SetSector(*this);
		return entityPtr;
	}

	void Sector::Update(WorldState& worldState) const
	{
		Datum& entities = Entities();
		for (uint32_t i = 0u; i < entities.Size(); ++i)
		{
			entities.GetScope(i).As<Entity>()->Update(worldState);
		}
	}
}