#pragma once
#include "Action.h"
#include "Factory.h"
namespace UnitTest
{
	class ActionFoo : public LibraryShared::Action
	{
		RTTI_DECLARATIONS(ActionFoo, LibraryShared::Action);
	public:
		static uint32_t numberOfActions;
		explicit ActionFoo(const std::string& name = std::string());
		virtual void ActionFoo::Update(WorldState& worldState) override;
	};

	ConcreteFactory(ActionFoo, LibraryShared::Action)
}