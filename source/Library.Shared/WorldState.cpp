#include "pch.h"

namespace LibraryShared
{
	WorldState::WorldState(GameTime& time) : mGameTime(&time)
	{}

	GameTime* WorldState::GetGameTime() const
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(GameTime& time)
	{
		mGameTime = &time;
	}

	World* WorldState::GetWorld() const
	{
		return mWorld;
	}

	void WorldState::SetWorld(World& world)
	{
		mWorld = &world;
	}

	void WorldState::AddToDeleteList(Scope& toDelete)
	{
		mDeleteList.PushBack(&toDelete);
	}

	void WorldState::DestroyScopes()
	{
		for (Scope* scope : mDeleteList)
		{
			scope->DetachFromParent();
			delete scope;
		}
		mDeleteList.Clear();
	}

	void WorldState::SetEntity(Entity& entity)
	{
		mEntity = &entity;
	}

	void WorldState::SetAction(Action& action)
	{
		mAction = &action;
	}

	Vector<Scope*> WorldState::mDeleteList;
}