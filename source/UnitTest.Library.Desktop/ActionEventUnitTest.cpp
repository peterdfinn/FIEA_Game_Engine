#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;
using namespace std::chrono;

namespace UnitTest
{
	TEST_CLASS(ActionEventUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		ActionEvent dummyActionEvent;
		ReactionAttributed dummyReactionAttributed;
		EventMessageAttributed dummyEventMessageAttributed;
		World dummyWorld;
	public:
		TEST_METHOD_INITIALIZE(ActionEventUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(ActionEventUnitTestCleanup)
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

		TEST_METHOD(ActionEventConstructorTest)
		{
			ActionEvent ae("type"s, 40ms, "name"s);
			Assert::IsTrue(40ms == ae.GetDelay());
			Assert::AreEqual("type"s, ae.GetSubtype());
			Assert::IsTrue(ae.IsPrescribedAttribute("this"s));
			Assert::IsTrue(ae.IsPrescribedAttribute("mDelay"s));
			Assert::IsTrue(ae.IsPrescribedAttribute("mSubtype"s));
		}

		TEST_METHOD(ActionEventUpdateTest)
		{
			/*When I call Update of ActionEvent, I expect that the desired Event
			has been created and enqueued with the appropriate delay and
			message. I also expect that any auxiliary attributes possessed by
			the ActionEvent be passed on to the EventMessageAttributed.*/
			World world;
			WorldState worldState;
			GameTime gameTime;
			worldState.SetGameTime(gameTime);
			worldState.SetWorld(world);
			GameClock clock;
			clock.UpdateGameTime(gameTime);
			ActionEvent ae("type"s);
			ae.AppendAuxiliaryAttribute("aux"s);
			ae.Update(worldState);
			clock.UpdateGameTime(gameTime);
			Assert::IsTrue(1u == world.GetEventQueue().Size());
			ReactionAttributed reaction("type"s);
			Assert::IsNull(reaction.GetEventMessage());
			world.GetEventQueue().Update(gameTime);

			const auto* msg = reaction.GetEventMessage();
			Assert::IsNotNull(msg);
			Assert::IsTrue(msg->IsAuxiliaryAttribute("aux"s));
		}

		TEST_METHOD(ActionEventSetSubtypeTest)
		{
			ActionEvent ae("old"s);
			ae.SetSubtype("new"s);
			Assert::AreEqual("new"s, ae.GetSubtype());
		}
		
		TEST_METHOD(ActionEventSetDelayTest)
		{
			ActionEvent ae("type"s, 100ms);
			ae.SetDelay(101ms);
			Assert::IsTrue(101ms == ae.GetDelay());
		}
	};
}