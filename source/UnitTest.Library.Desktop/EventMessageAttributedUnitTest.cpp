#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(EventMessageAttributedUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		EventMessageAttributed dummyEventMessageAttributed;
		World dummyWorld;
	public:
		TEST_METHOD_INITIALIZE(EventMessageAttributedUnitTestInit)
		{
#ifdef _DEBUG

			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(EventMessageAttributedUnitTestCleanup)
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

		TEST_METHOD(EventMessageAttributedConstructorTest)
		{
			World world;
			EventMessageAttributed message("insertnamehere"s, &world);
			Assert::AreEqual("insertnamehere"s, message.GetSubtype());
			Assert::IsTrue(message.GetWorld() == &world);
			Assert::IsTrue(message.IsPrescribedAttribute("mSubtype"s));
			Assert::IsTrue(message.IsPrescribedAttribute("this"s));
			Assert::IsTrue(message.IsPrescribedAttribute("mWorld"s));
		}

		TEST_METHOD(EventMessageAttributedMoveConstructorTest)
		{
			World world;
			EventMessageAttributed rhs("Alan Smithee"s, &world);
			EventMessageAttributed lhs = move(rhs);
			Assert::IsTrue(lhs.GetWorld() == &world);
			Assert::AreEqual("Alan Smithee"s, lhs.GetSubtype());
			Assert::IsTrue(rhs.GetSubtype().empty());
			Assert::IsNull(rhs.GetWorld());
		}

		TEST_METHOD(EventMessageAttributedMoveAssignmentOperatorTest)
		{
			World world;
			EventMessageAttributed rhs("Alan Smithee"s, &world);
			EventMessageAttributed lhs;
			lhs = move(rhs);
			Assert::IsTrue(lhs.GetWorld() == &world);
			Assert::AreEqual("Alan Smithee"s, lhs.GetSubtype());
			Assert::IsTrue(rhs.GetSubtype().empty());
			Assert::IsNull(rhs.GetWorld());
		}

		TEST_METHOD(EventMessageAttributedSetSubtypeUnitTest)
		{
			EventMessageAttributed ema;
			ema.SetSubtype("whatever"s);
			Assert::AreEqual("whatever"s, ema.GetSubtype());
		}
	};
}