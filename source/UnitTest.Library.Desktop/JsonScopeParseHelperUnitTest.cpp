#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(JsonScopeParseHelperUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
	public:
		TEST_METHOD_INITIALIZE(JsonScopeParseHelperUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(JsonScopeParseHelperUnitTestCleanup)
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

		TEST_METHOD(JsonScopeParseHelperDefaultConstructorTest)
		{
			/*When I default construct a JsonScopeParseHelper, I expect its
			pointer to a JsonParseMaster to be nullptr.*/
			JsonScopeParseHelper helper;
			Assert::IsTrue(helper.GetParseMaster() == nullptr);
		}

		TEST_METHOD(JsonScopeParseHelperCopyConstructorTest)
		{
			/*I expect the copy constructor of JsonScopeParseHelper to copy the
			pointer to the master.*/
			JsonParseMaster master;
			JsonScopeParseHelper helper;
			master.AddHelper(helper);
			JsonScopeParseHelper copy(helper);
			Assert::IsTrue(&master == copy.GetParseMaster());
		}

		TEST_METHOD(JsonScopeParseHelperCopyAssignmentOperatorTest)
		{
			/*I expect the copy assignment operator of JsonScopeParseHelper to
			copy the pointer to the master.*/
			JsonParseMaster master;
			JsonScopeParseHelper helper;
			master.AddHelper(helper);
			JsonScopeParseHelper copy;
			copy = helper;
			Assert::IsTrue(&master == copy.GetParseMaster());
		}

		TEST_METHOD(JsonScopeParseHelperMoveConstructorTest)
		{
			/*I expect the move constructor of JsonScopeParseHelper to copy the
			pointer to the master, and I expect the pointer in the original to
			be set to nullptr.*/
			JsonParseMaster master;
			JsonScopeParseHelper helper;
			master.AddHelper(helper);
			JsonScopeParseHelper copy(move(helper));
			Assert::IsTrue(&master == copy.GetParseMaster());
			Assert::IsTrue(helper.GetParseMaster() == nullptr);
		}

		TEST_METHOD(JsonScopeParseHelperMoveAssignmentOperatorTest)
		{
			/*I expect the move assignment operator of JsonScopeParseHelper to
			copy the pointer to the master, and I expect the pointer in the
			original to be set to nullptr.*/
			JsonParseMaster master;
			JsonScopeParseHelper helper;
			master.AddHelper(helper);
			JsonScopeParseHelper copy;
			copy = move(helper);
			Assert::IsTrue(&master == copy.GetParseMaster());
			Assert::IsTrue(helper.GetParseMaster() == nullptr);
		}

		TEST_METHOD(JsonScopeParseHelperCreateTest)
		{
			/*I expect Create to dynamically allocate a new JsonScopeParseHelper
			and return a pointer to it. The dynamically allocated
			JsonScopeParseHelper should have its pointer to a master be
			nullptr.*/
			JsonParseMaster master;
			JsonScopeParseHelper helper;
			helper.SetParseMaster(&master);
			const IJsonParseHelper* const created = helper.Create();
			Assert::IsTrue(created->Is(JsonScopeParseHelper::TypeIdClass()));
			Assert::IsTrue(created->GetParseMaster() == nullptr);
			delete created;
		}

		TEST_METHOD(JsonScopeParseHelperHandleBeginningTest)
		{
			
			JsonSharedData data;
			JsonSharedData* addrOfData = &data;
			UNREFERENCED_PARAMETER(addrOfData);
			JsonParseMaster master(&data);
			JsonParseMaster* addrOfMaster = &master;
			UNREFERENCED_PARAMETER(addrOfMaster);
			JsonScopeParseHelper helper;
			master.AddHelper(helper);
			Assert::IsFalse(helper.HandleBeginning("name"));
		}

		TEST_METHOD(JsonScopeParseHelperHandleTest)
		{
			JsonSharedData data1;
			JsonParseMaster master1(&data1);
			JsonScopeParseHelper helper1;
			master1.AddHelper(helper1);
			Json::Value value1;
			Assert::IsFalse(helper1.Handle("type", value1));

			JsonScopeSharedData data2;
			JsonParseMaster master2(&data2);
			JsonScopeParseHelper helper2;
			master2.AddHelper(helper2);
			Json::Value value2;
			Assert::IsFalse(helper2.Handle("type", value2));

			data2.PushStack();
			stringstream("{\"type\": \"str\"}") >> value2;
			Assert::IsTrue(value2["type"].isString());
			data2.SetTopOfStackType(DatumType::FLOAT);
			Assert::IsFalse(helper2.Handle("type", value2["type"]));

			Json::Value value3;
			data2.SetTopOfStackValue(&value3);
			helper2.Handle("value", value3);

			Assert::IsFalse(helper2.Handle("", value2["type"]));

			helper2.Handle("fake", value3);
		}
	};
}