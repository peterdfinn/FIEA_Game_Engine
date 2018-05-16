#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(ReactionAttributedUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		ReactionAttributed dummyReactionAttributed;
		World dummyWorld;
		EventMessageAttributed dummyEma;
	public:
		TEST_METHOD_INITIALIZE(ReactionAttributedUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(ReactionAttributedUnitTestCleanup)
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

		TEST_METHOD(ReactionAttributedConstructorTest)
		{
			ReactionAttributed reactionAttributed("type"s);
			Assert::AreEqual("type"s, reactionAttributed.GetSubtype());
			Assert::IsTrue(reactionAttributed.IsPrescribedAttribute("this"s));
			Assert::IsTrue(reactionAttributed.IsPrescribedAttribute("mEventMessage"s));
			Assert::IsTrue(reactionAttributed.IsPrescribedAttribute("mSubtype"s));
		}

		TEST_METHOD(ReactionAttributedMoveConstructorTest)
		{
			World world;
			ReactionAttributed rhs("type"s);
			EventMessageAttributed ema("type"s);
			Event<EventMessageAttributed> event(ema);
			Assert::IsTrue(event.Is(Event<EventMessageAttributed>::TypeIdClass()));
			event.Deliver();
			/*Now, 'rhs' should contain a subtype "type" and a pointer to 'ema'.
			If we move this, then the new ReactionAttributed should have subtype
			"type" and a pointer to 'ema', whereas these values in 'rhs' should
			have become "" and nullptr, respectively.*/
			ReactionAttributed lhs = move(rhs);
			Assert::IsNotNull(lhs.GetEventMessage());
			Assert::AreEqual("type"s, lhs.GetSubtype());
			Assert::IsNull(rhs.GetEventMessage());
			Assert::IsTrue(rhs.GetSubtype().empty());
		}

		TEST_METHOD(ReactionAttributedMoveAssignmentOperatorTest)
		{
			World world;
			ReactionAttributed rhs("type"s);
			EventMessageAttributed ema("type"s);
			Event<EventMessageAttributed> event(ema);
			event.Deliver();
			/*Now, 'rhs' should contain a subtype "type" and a pointer to 'ema'.
			If we move this, then the new ReactionAttributed should have subtype
			"type" and a pointer to 'ema', whereas these values in 'rhs' should
			have become "" and nullptr, respectively.*/
			ReactionAttributed lhs;
			lhs = move(rhs);
			Assert::IsNotNull(lhs.GetEventMessage());
			Assert::AreEqual("type"s, lhs.GetSubtype());
			Assert::IsNull(rhs.GetEventMessage());
			Assert::IsTrue(rhs.GetSubtype().empty());
		}

		TEST_METHOD(ReactionAttributedSetSubtypeTest)
		{
			ReactionAttributed ra;
			ra.SetSubtype("dab"s);
			Assert::AreEqual("dab"s, ra.GetSubtype());
		}
	};
}