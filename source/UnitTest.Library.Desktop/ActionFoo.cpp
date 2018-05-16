#include "pch.h"
using namespace LibraryShared;
using namespace std;
namespace UnitTest
{
	RTTI_DEFINITIONS(ActionFoo);

	uint32_t ActionFoo::numberOfActions{ 0u };

	ActionFoo::ActionFoo(const string& name) : Action(name) {}

	void ActionFoo::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		++numberOfActions;
	}
}