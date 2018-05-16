#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(ActionListUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		ActionList dummyActionList{ "dummyActionList"s };
		ActionFoo dummyActionFoo{ "dummyActionFoo"s };

	public:
		TEST_METHOD_INITIALIZE(ActionListUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(ActionListUnitTestCleanup)
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

		TEST_METHOD(ActionListConstructorTest)
		{
			/*I expect that, after constructing an ActionList, the name be what
			was passed in, and that there be three prescribed attributes, with
			names "mName", "this" and "Actions".*/
			ActionList actionList("list"s);
			Assert::AreEqual("list"s, actionList.Name());
			Assert::IsTrue(actionList.IsPrescribedAttribute("this"s));
			Assert::IsTrue(actionList.IsPrescribedAttribute("Actions"s));
		}

		TEST_METHOD(ActionListActionsTest)
		{
			/*I expect that Actions return a valid Datum, unless the ActionList
			had been recently moved, in which case, I expect an exception to be
			thrown.*/
			ActionList list("list"s);
			list.Actions();
			ActionList movedInto(move(list));
			auto expectNoActionsDatum = [&list]() { list.Actions(); };
			Assert::ExpectException<exception>(expectNoActionsDatum);
		}

		TEST_METHOD(ActionListUpdateTest)
		{
			/*I expect that the Update method of ActionList call the Update
			method of each Action in this ActionList.*/

			ActionFoo::numberOfActions = 0u;
			ActionList actionList("actionList"s);
			Datum& actions = actionList.Actions();
			for (int i = 0; i < 10; ++i)
			{
				actions.PushBack(*(new ActionFoo("actionFoo"s)));
			}
			WorldState worldState;
			actionList.Update(worldState);
			Assert::AreEqual(10u, ActionFoo::numberOfActions);
		}
	};
}