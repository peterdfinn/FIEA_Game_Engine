#include "pch.h"
using namespace std;
namespace LibraryShared
{
	RTTI_DEFINITIONS(World);
	World::World(const string& name) : mName(name)
	{
		const uint64_t worldId = World::TypeIdClass();
		RegisterAttribute(Signature("this", DatumType::POINTER, 1u, 0u),
			worldId);
		RegisterAttribute(Signature("mName"s, DatumType::STRING, 1u,
			offsetof(World, mName)), worldId);
		RegisterAttribute(Signature("Sectors"s, DatumType::TABLE, 0u, 0u),
			worldId, true, false);
		RegisterAttribute(Signature("Actions"s, DatumType::TABLE, 0u, 0u),
			worldId, true, false);

		RegisterBaseType(worldId, Attributed::TypeIdClass());
	}

	World::World(World&& other) : Attributed(move(other)), mName(move(other.mName))
	{}

	World& World::operator=(World&& other)
	{
		if (&other != this)
		{
			Attributed::operator=(move(other));
			mName = move(other.mName);
		}
		return *this;
	}

	const string& World::Name() const
	{
		return mName;
	}

	void World::SetName(const string& name)
	{
		mName = name;
	}

	Datum& World::Sectors() const
	{
		Datum* sectors = Find("Sectors"s);
		if (sectors == nullptr)
		{
			throw exception("This World has no Sectors container! Did you move\
				it recently?");
		}
		return *sectors;
	}

	Datum& World::Actions() const
	{
		Datum* const actionsDatumPtr = Find("Actions"s);
		if (actionsDatumPtr == nullptr)
		{
			throw exception("This Entity has no Actions container! Did you\
				move it recently?");
		}
		return *actionsDatumPtr;
	}

	void World::Update(WorldState& worldState)
	{
		WorldState::DestroyScopes();
		worldState.SetWorld(*this);
		const Datum& sectors = Sectors();
		for (uint32_t i = 0u; i < sectors.Size(); ++i)
		{
			sectors.GetScope(i).As<Sector>()->Update(worldState);
		}
		mClock.UpdateGameTime(*worldState.GetGameTime());
	}

	Sector* const World::CreateSector(const string& instanceName)
	{
		return new Sector(instanceName, this);
	}

	const EventQueue& World::GetEventQueue() const
	{
		return mEventQueue;
	}

	EventQueue& World::GetEventQueue()
	{
		return mEventQueue;
	}
}