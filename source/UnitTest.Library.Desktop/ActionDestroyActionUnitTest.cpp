#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(ActionDestroyActionUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		ActionDestroyAction dummyActionDestroyAction;
		World dummyWorld;
		Sector dummySector;
		Entity dummyEntity;
		ActionFoo dummyActionFoo;
		WorldState dummyWorldState;

	public:
		TEST_METHOD_INITIALIZE(ActionDestroyActionUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(ActionDestroyActionUnitTestCleanup)
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

		TEST_METHOD(ActionDestroyActionConstructorTest)
		{
			/*When I construct an ActionDestroyAction, I expect that its name
			and mAction strings are the ones passed in, and I expect that it
			have a prescribed attribute called "mAction".*/
			ActionDestroyAction ada("actionToDestroy"s, "name"s);
			Assert::AreEqual("name"s, ada.Name());
			Assert::AreEqual("actionToDestroy"s, ada.GetAction());
			Assert::IsTrue(ada.IsPrescribedAttribute("mAction"s));
		}

		TEST_METHOD(ActionDestroyActionUpdateTest)
		{
			/*When I update an ActionDestroyAction, I expect that it remove the
			specified Action from its container, as well as itself. I then
			expect that another call to the World's update method will cause
			both of these two Actions to be deallocated.*/
			World world("world"s);
			GameTime gameTime;
			WorldState worldState(gameTime);
			Sector* sector = new Sector("sector"s, &world);
			Entity* entity = new Entity("entity"s, sector);
			ActionFoo* actionFoo = new ActionFoo("actionFoo"s);
			ActionDestroyAction* actionDestroyAction = new ActionDestroyAction("actionFoo"s, "actionDestroyAction");
			entity->Adopt("actionFoo"s, *actionFoo);
			entity->Adopt("actionDestroyAction"s, *actionDestroyAction);

			world.Update(worldState);
			world.Update(worldState);

			Datum& actionsDatum = entity->Actions();
			uint32_t size = actionsDatum.Size();
			for (uint32_t i = 0u; i < size; ++i)
			{
				Scope& currentScope = actionsDatum.GetScope(i);
				if (&currentScope == actionFoo || &currentScope == actionDestroyAction)
				{
					Assert::Fail();
				}
			}


		}
	};
}