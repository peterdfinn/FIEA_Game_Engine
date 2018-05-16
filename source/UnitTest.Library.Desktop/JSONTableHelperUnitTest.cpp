#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;
using namespace glm;

namespace UnitTest
{
	TEST_CLASS(JSONTableHelperUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;

	public:
		TEST_METHOD_INITIALIZE(DatumTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(DatumTestCleanup)
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

		TEST_METHOD(JSONTableHelperHandleTest)
		{
			/*Let's create a test file.*/
			ofstream testFileOut("TableHelperTestFile.txt");
			vec4 v1(-1.0f, -1.1f, -1.2f, -1.3f), v2(-2.0f, -2.1f, -2.2f, -2.3f);
			testFileOut << R"({"v1":{"type": "vector", "value": ")";
			testFileOut << to_string(v1);
			testFileOut << R"("},"child":{"type": "table", "value": {"v2":{"type":"vector","value": ")";
			testFileOut << to_string(v2);
			testFileOut << R"("}}}})";
			testFileOut.close();

			/*Now, let's parse the file.*/
			ifstream testFileIn("TableHelperTestFile.txt");
			JSONScopeSharedData sharedData;
			ParseMaster master(&sharedData);
			JSONTableHelper tableHelper;
			JSONVectorHelper vectorHelper;
			master.AddHelper(tableHelper);
			master.AddHelper(vectorHelper);
			bool streamIsOpen = testFileIn.is_open();
			streamIsOpen;
			master.Parse(testFileIn);

			/*Finally, let's see if the file was parsed in a way I expect.*/
			Scope& scopeAfterParse = sharedData.GetScope();
			Assert::IsFalse(scopeAfterParse.Find("v1") == nullptr);
		}
	};
}