#include "pch.h"

#include "SimpleSharedData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;
using namespace glm;

namespace UnitTest
{
	TEST_CLASS(SimpleParseHelperUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
	public:
		TEST_METHOD_INITIALIZE(AttributedTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(AttributedTestCleanup)
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

		TEST_METHOD(SimpleParseHelperConstructorTest)
		{
			/*I expect the constructor to default-construct all of the Datum
			member variables, meaning that they all have type UNKNOWN.*/
			SimpleSharedData sph;
			Assert::AreEqual(0, sph.dummy);
		}
	};
}