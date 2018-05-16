#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(JsonScopeSharedDataUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
	public:
		TEST_METHOD_INITIALIZE(JSONScopeSharedDataUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(JSONScopeSharedDataUnitTestCleanup)
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

		TEST_METHOD(JsonScopeSharedDataGetCurrentScopeTest)
		{
			JsonScopeSharedData d;
			Scope decoy;
			Assert::IsTrue(d.GetCurrentScope() == &d.GetScope());
			d.SetCurrentScope(decoy);
			Assert::IsTrue(d.GetCurrentScope() == &decoy);

			const JsonScopeSharedData dConst;
			Assert::IsTrue(dConst.GetCurrentScope() == &dConst.GetScope());
		}

		TEST_METHOD(JsonScopeSharedDataIsStackEmptyTest)
		{
			JsonScopeSharedData d;
			Assert::IsTrue(d.IsStackEmpty());
			d.PushStack();
			Assert::IsFalse(d.IsStackEmpty());
		}

		TEST_METHOD(JsonScopeSharedDataPopStackTest)
		{
			JsonScopeSharedData d;
			d.PushStack();
			d.PopStack();
			auto lambda = [&d]() { d.PopStack(); };
			Assert::ExpectException<exception>(lambda);
		}
	};
}