#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(ActionCreateActionUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		World dummyWorld{ "dummyWorld"s };
		Sector dummySector{ "dummySector"s };
		Entity dummyEntity{ "dummyEntity"s };
		ActionCreateAction dummyActionCreateAction;
		ActionFoo dummyActionFoo;
		WorldState dummyWorldState;

	public:
		TEST_METHOD_INITIALIZE(ActionCreateActionUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(ActionCreateActionUnitTestCleanup)
		{
#ifdef _DEBUG
			_CrtMemState finalMemoryState, memoryStateDiff;
			_CrtMemCheckpoint(&finalMemoryState);
			if (_CrtMemDifference(&memoryStateDiff, &initialMemoryState, &finalMemoryState))
			{
				Assert::Fail(L"Memory is leaking!");
			}
#endif
		}

		TEST_METHOD(ActionCreateActionConstructorTest)
		{
			ActionCreateAction action("name"s, "prototype"s, "instanceName"s);
			Assert::AreEqual("name"s, action.Name());
			Assert::AreEqual("prototype"s, action.Prototype());
			Assert::AreEqual("instanceName"s, action.InstanceName());
		}

		TEST_METHOD(ActionCreateActionUpdateTest)
		{
			ActionFooFactory aff;
			Factory<Action>::Add(aff);
			World world("world"s);
			Sector* sector = new Sector("sector"s, &world);
			Entity* entity = new Entity("entity"s, sector);
			GameTime gameTime;
			WorldState worldState(gameTime);
			string aca = "actionCreateAction"s;
			ActionCreateAction* actionCreateAction = new ActionCreateAction(
				aca, "ActionFoo"s,"newInstanceName"s);
			entity->Adopt(aca, *actionCreateAction);

			/*By this point, we should now have one world, which contains one
			Sector, which contains one Entity, which contains one Action. If we
			call the World's Update method, this should cause a cascading effect
			wherein the Update method of ActionCreateAction will be called.*/
			world.Update(worldState);

			/*At this point, I expect that the Entity's Actions Datum contain
			exactly one ActionFoo.*/
			Datum& actionsDatum = entity->Actions();
			uint32_t actionsDatumSize = actionsDatum.Size();
			uint32_t actionFoos = 0u;
			for (uint32_t i = 0u; i < actionsDatumSize; ++i)
			{
				if (actionsDatum.GetScope(i).Is(ActionFoo::TypeIdClass()))
				{
					++actionFoos;
				}
			}
			Assert::AreEqual(1u, actionFoos);

			/*Finally, in order to not leak memory, we must call Update again.
			This should deallocate the ActionCreateAction, and I should expect
			that a reference to it no longer be found in the container of
			Actions.*/
			world.Update(worldState);
			actionsDatumSize = actionsDatum.Size();
			for (uint32_t i = 0u; i < actionsDatumSize; ++i)
			{
				if (&actionsDatum.GetScope(i) == actionCreateAction)
				{
					Assert::Fail();
				}
			}
			Factory<Action>::Remove(aff);
		}
	};
}