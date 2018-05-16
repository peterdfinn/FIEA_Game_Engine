#include "pch.h"
#include "JsonParseMaster.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;
using namespace glm;
using namespace Json;

namespace UnitTest
{
	TEST_CLASS(JsonParseMasterUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
	public:
		TEST_METHOD_INITIALIZE(JsonParseMasterUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(JsonParseMasterUnitTestCleanup)
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

		TEST_METHOD(JsonParseMasterConstructorTest)
		{
			/*When default constructing a JsonParseMaster, I expect an empty
			Vector of helpers to have been allocated, and I expect the pointer
			to the SharedData to be nullptr. I also expect IsClone() to return
			false.*/
			JsonParseMaster master1;
			Assert::IsTrue(master1.GetParseHelpers().IsEmpty());
			Assert::IsTrue(master1.GetSharedData() == nullptr);
			Assert::IsFalse(master1.IsClone());

			/*When constructing a ParseMaster with one parameter, I expect an
			empty Vector of helpers to have been allocated, and I expect the
			pointer to the SharedData to have been set to the input parameter.*/
			JsonSharedData sharedData;
			JsonParseMaster master2(&sharedData);
			Assert::IsTrue(&sharedData == master2.GetSharedData());
			Assert::IsTrue(master2.GetParseHelpers().IsEmpty());
			Assert::IsFalse(master2.IsClone());
		}
		
		TEST_METHOD(JsonParseMasterDestructorTest)
		{
			/*When destructing a ParseMaster, I expect that it should not cause
			memory leaks, even if it has been cloned, in which case it is
			responsible for deallocating its helpers.*/
			JsonScopeSharedData sharedData;
			JsonScopeParseHelper helper;
			JsonParseMaster master(&sharedData);
			master.AddHelper(helper);

			const JsonParseMaster* const clone = master.Clone();
			delete clone;
		}

		TEST_METHOD(JsonParseMasterMoveConstructorTest)
		{
			/*I expect the move constructor to leave the object moved from in a
			"nullified" state, which in this case means that its list of helpers
			is empty, its JsonSharedData pointer is nullptr, and it's not marked
			as a clone. I also expect the data in the new JsonParseHelper to be
			the same as the one in the original before it was moved.*/
			JsonScopeSharedData sharedData;
			JsonParseMaster masterOriginal(&sharedData);
			JsonScopeParseHelper helper;
			masterOriginal.AddHelper(helper);

			JsonParseMaster masterMove(move(masterOriginal));
			Assert::IsTrue(masterMove.GetSharedData() == &sharedData);
			Assert::AreEqual(1u, masterMove.GetParseHelpers().Size());
			Assert::IsTrue(&helper == masterMove.GetParseHelpers().At(0u));
			Assert::IsFalse(masterMove.IsClone());

			Assert::IsTrue(masterOriginal.GetSharedData() == nullptr);
			Assert::IsTrue(masterOriginal.GetParseHelpers().IsEmpty());
			Assert::IsFalse(masterOriginal.IsClone());
		}
		
		TEST_METHOD(JsonParseMasterMoveConstructorCloneTest)
		{
			/*I expect the move constructor to leave the object moved from in a
			"nullified" state, which in this case means that its list of helpers
			is empty, its JsonSharedData pointer is nullptr, and it's not marked
			as a clone. I also expect the data in the new JsonParseHelper to be
			the same as the one in the original before it was moved.*/
			JsonScopeSharedData sharedData;
			JsonParseMaster masterOriginal(&sharedData);
			JsonScopeParseHelper helper;
			masterOriginal.AddHelper(helper);
			JsonParseMaster* const clone = masterOriginal.Clone();
			const JsonSharedData* const cloneSharedData = clone->GetSharedData();
			const IJsonParseHelper* const cloneHelper = clone->GetParseHelpers().At(0u);

			JsonParseMaster masterMove(move(*clone));
			Assert::IsTrue(masterMove.GetSharedData() == cloneSharedData);
			Assert::AreEqual(1u, masterMove.GetParseHelpers().Size());
			Assert::IsTrue(cloneHelper == masterMove.GetParseHelpers().At(0u));
			Assert::IsTrue(masterMove.IsClone());

			Assert::IsTrue(clone->GetSharedData() == nullptr);
			Assert::IsTrue(clone->GetParseHelpers().IsEmpty());
			Assert::IsFalse(clone->IsClone());

			delete clone;
		}

		TEST_METHOD(JsonParseMasterMoveOriginalToOriginalTest)
		{
			/*This is a test of the move assignment operator when the lhs is not
			a clone and the rhs is not a clone.*/
			/*I expect the move assignment operator to leave the object moved
			from in a "nullified" state, which in this case means that its list
			of helpers is empty, its JsonSharedData pointer is nullptr, and it's
			not marked as a clone. I also expect the data in the new
			JsonParseHelper to be the same as the one in the original before it
			was moved.*/
			JsonScopeSharedData sharedDataLhs;
			JsonParseMaster masterLhs(&sharedDataLhs);
			JsonScopeParseHelper helperLhs;
			masterLhs.AddHelper(helperLhs);

			JsonScopeSharedData sharedDataRhs;
			JsonParseMaster masterRhs(&sharedDataRhs);
			JsonScopeParseHelper helperRhs;
			masterRhs.AddHelper(helperRhs);

			masterLhs = move(masterRhs);

			Assert::IsTrue(&sharedDataRhs == masterLhs.GetSharedData());
			Assert::AreEqual(1u, masterLhs.GetParseHelpers().Size());
			Assert::IsTrue(&helperRhs == masterLhs.GetParseHelpers().At(0u));
			Assert::IsFalse(masterLhs.IsClone());

			Assert::IsTrue(masterRhs.GetSharedData() == nullptr);
			Assert::IsTrue(masterRhs.GetParseHelpers().IsEmpty());
			Assert::IsFalse(masterRhs.IsClone());
		}

		TEST_METHOD(JsonParseMasterMoveOriginalToCloneTest)
		{
			/*This is a test of the move assignment operator when the lhs is a
			clone and the rhs is not a clone.*/
			/*I expect the move assignment operator to leave the object moved
			from in a "nullified" state, which in this case means that its list
			of helpers is empty, its JsonSharedData pointer is nullptr, and it's
			not marked as a clone. I also expect the data in the new
			JsonParseHelper to be the same as the one in the original before it
			was moved.*/
			JsonScopeSharedData sharedDataLhs;
			JsonParseMaster masterLhsOriginal(&sharedDataLhs);
			JsonScopeParseHelper helperLhs;
			masterLhsOriginal.AddHelper(helperLhs);
			JsonParseMaster* const masterLhs = masterLhsOriginal.Clone();

			JsonScopeSharedData sharedDataRhs;
			JsonParseMaster masterRhs(&sharedDataRhs);
			JsonScopeParseHelper helperRhs;
			masterRhs.AddHelper(helperRhs);

			*masterLhs = move(masterRhs);

			Assert::IsTrue(&sharedDataRhs == masterLhs->GetSharedData());
			Assert::AreEqual(1u, masterLhs->GetParseHelpers().Size());
			Assert::IsTrue(&helperRhs == masterLhs->GetParseHelpers().At(0u));
			Assert::IsFalse(masterLhs->IsClone());

			Assert::IsTrue(masterRhs.GetSharedData() == nullptr);
			Assert::IsTrue(masterRhs.GetParseHelpers().IsEmpty());
			Assert::IsFalse(masterRhs.IsClone());

			delete masterLhs;
		}

		TEST_METHOD(JsonParseMasterMoveCloneToOriginalTest)
		{
			/*This is a test of the move assignment operator when the lhs is not
			a clone and the rhs is a clone.*/
			/*I expect the move assignment operator to leave the object moved
			from in a "nullified" state, which in this case means that its list
			of helpers is empty, its JsonSharedData pointer is nullptr, and it's
			not marked as a clone. I also expect the data in the new
			JsonParseHelper to be the same as the one in the original before it
			was moved.*/
			JsonScopeSharedData sharedDataLhs;
			JsonParseMaster masterLhs(&sharedDataLhs);
			JsonScopeParseHelper helperLhs;
			masterLhs.AddHelper(helperLhs);

			JsonScopeSharedData sharedDataRhs;
			JsonParseMaster masterRhsOriginal(&sharedDataRhs);
			JsonScopeParseHelper helperRhs;
			masterRhsOriginal.AddHelper(helperRhs);
			JsonParseMaster* const masterRhs = masterRhsOriginal.Clone();
			const JsonSharedData* const masterRhsSharedData = masterRhs->GetSharedData();
			const IJsonParseHelper* const masterRhsHelper = masterRhs->GetParseHelpers().At(0u);

			masterLhs = move(*masterRhs);

			Assert::IsTrue(masterRhsSharedData == masterLhs.GetSharedData());
			Assert::AreEqual(1u, masterLhs.GetParseHelpers().Size());
			Assert::IsTrue(masterRhsHelper == masterLhs.GetParseHelpers().At(0u));
			Assert::IsTrue(masterLhs.IsClone());

			Assert::IsTrue(masterRhs->GetSharedData() == nullptr);
			Assert::IsTrue(masterRhs->GetParseHelpers().IsEmpty());
			Assert::IsFalse(masterRhs->IsClone());

			delete masterRhs;
		}

		TEST_METHOD(JsonParseMasterMoveCloneToCloneTest)
		{
			/*This is a test of the move assignment operator when the lhs is a
			clone and the rhs is a clone.*/
			/*I expect the move assignment operator to leave the object moved
			from in a "nullified" state, which in this case means that its list
			of helpers is empty, its JsonSharedData pointer is nullptr, and it's
			not marked as a clone. I also expect the data in the new
			JsonParseHelper to be the same as the one in the original before it
			was moved.*/
			JsonScopeSharedData sharedDataLhs;
			JsonParseMaster masterLhsOriginal(&sharedDataLhs);
			JsonScopeParseHelper helperLhs;
			masterLhsOriginal.AddHelper(helperLhs);
			JsonParseMaster* const masterLhs = masterLhsOriginal.Clone();

			JsonScopeSharedData sharedDataRhs;
			JsonParseMaster masterRhsOriginal(&sharedDataRhs);
			JsonScopeParseHelper helperRhs;
			masterRhsOriginal.AddHelper(helperRhs);
			JsonParseMaster* const masterRhs = masterRhsOriginal.Clone();
			const JsonSharedData* const masterRhsSharedData = masterRhs->GetSharedData();
			const IJsonParseHelper* const masterRhsHelper = masterRhs->GetParseHelpers().At(0u);

			*masterLhs = move(*masterRhs);

			Assert::IsTrue(masterRhsSharedData == masterLhs->GetSharedData());
			Assert::AreEqual(1u, masterLhs->GetParseHelpers().Size());
			Assert::IsTrue(masterRhsHelper == masterLhs->GetParseHelpers().At(0u));
			Assert::IsTrue(masterLhs->IsClone());

			Assert::IsTrue(masterRhs->GetSharedData() == nullptr);
			Assert::IsTrue(masterRhs->GetParseHelpers().IsEmpty());
			Assert::IsFalse(masterRhs->IsClone());

			delete masterRhs;
			delete masterLhs;
		}

		TEST_METHOD(JsonParseMasterCloneTest)
		{
			/*I expect Clone to not leak memory, I expect their helpers to be
			of the same derived class, and I expect their SharedData members to
			be of the same derived class.*/

			JsonScopeParseHelper helper;
			JsonScopeSharedData sharedData;
			JsonParseMaster master(&sharedData);
			master.AddHelper(helper);
			const JsonParseMaster* const clone = master.Clone();

			Assert::IsTrue(clone->IsClone());
			Assert::AreEqual(1u, clone->GetParseHelpers().Size());
			Assert::IsTrue(clone->GetParseHelpers().At(0u)->Is(JsonScopeParseHelper::TypeIdClass()));
			Assert::IsTrue(clone->GetSharedData()->Is(JsonScopeSharedData::TypeIdClass()));

			delete clone;
		}

		TEST_METHOD(JsonParseMasterAddHelperTest)
		{
			/*When I add helpers to a JsonParseMaster, I expect one of two
			things to happen. If the JsonParseMaster is a clone, I expect that
			the helper not be added and that AddHelper return false. If the
			JsonParseMaster is not a clone, I expect AddHelper to return true,
			and I expect a pointer to that helper to now exist in the master's
			list of helpers. Also, when I successfully add a helper to a
			master's list, I expect that helper to recognize its new master.*/
			JsonParseMaster master;
			JsonScopeParseHelper helper1;
			Assert::IsTrue(master.AddHelper(helper1));
			Assert::AreEqual(1u, master.GetParseHelpers().Size());
			Assert::IsTrue(master.GetParseHelpers().At(0u) == &helper1);
			Assert::IsTrue(helper1.GetParseMaster() == &master);

			JsonParseMaster* const clone = master.Clone();
			JsonScopeParseHelper helper2;
			Assert::IsFalse(clone->AddHelper(helper2));
			Assert::AreEqual(1u, clone->GetParseHelpers().Size());

			delete clone;
		}

		TEST_METHOD(JsonParseMasterRemoveHelperTest)
		{
			/*I expect a removed helper to no longer be in the JsonParseMaster's
			vector of pointers to helpers (assuming, of course, it wasn't in
			there twice for some reason), and I expect RemoveHelper to return
			true in the event a helper was successfully removed and the master
			was not a clone, and false otherwise. I also expect the removed
			helper to no longer point to its former master.*/

			JsonParseMaster master;
			JsonScopeParseHelper helper1, helper2;
			master.AddHelper(helper1);
			JsonParseMaster* const clone = master.Clone();

			Assert::IsFalse(clone->RemoveHelper(helper1));

			delete clone;

			Assert::IsFalse(master.RemoveHelper(helper2));
			Assert::IsTrue(master.RemoveHelper(helper1));
			Assert::IsTrue(master.GetParseHelpers().IsEmpty());
			Assert::IsTrue(helper1.GetParseMaster() == nullptr);
		}

		TEST_METHOD(JsonParseMasterParseTest)
		{
			/*I expect the ParseMaster to parse the JSON file into a
			SimpleSharedData and populate the Datums in the SimpleSharedData
			object according to the contents of the file.*/

			ifstream fileStream("Content\\TableHelperTestFile.txt");
			JsonScopeSharedData sharedData;
			JsonParseMaster master(&sharedData);
			JsonScopeParseHelper helper;
			master.AddHelper(helper);

			master.Parse(fileStream);

			fileStream.close();

			Scope& populatedScope = sharedData.GetScope();
			Assert::AreEqual(11u, populatedScope.Size());

			Datum* testInt = populatedScope.Find("testInt");
			Assert::IsNotNull(testInt);
			Assert::AreEqual(1u, testInt->Size());
			Assert::AreEqual(4, testInt->GetInt());

			Datum* testIntArray = populatedScope.Find("testIntArray");
			Assert::IsNotNull(testIntArray);
			Assert::AreEqual(3u, testIntArray->Size());
			for (uint32_t i = 0u; i < 3u; ++i)
			{
				Assert::AreEqual(i + 1u, static_cast<uint32_t>(testIntArray->GetInt(i)));
			}

			Datum* testFloat = populatedScope.Find("testFloat");
			Assert::IsNotNull(testFloat);
			Assert::AreEqual(1u, testFloat->Size());
			Assert::AreEqual(-9.99f, testFloat->GetFloat());

			Datum* testFloatArray = populatedScope.Find("testFloatArray");
			Assert::IsNotNull(testFloatArray);
			Assert::AreEqual(2u, testFloatArray->Size());
			Assert::AreEqual(0.1f, testFloatArray->GetFloat(0u));
			Assert::AreEqual(0.2f, testFloatArray->GetFloat(1u));

			Datum* testVector = populatedScope.Find("testVector");
			Assert::IsNotNull(testVector);
			Assert::AreEqual(1u, testVector->Size());
			vec4 v1(2.0f, 2.0f, 2.0f, 2.0f);
			Assert::IsTrue(v1 == testVector->GetVector());

			Datum* testVectorArray = populatedScope.Find("testVectorArray");
			Assert::IsNotNull(testVectorArray);
			Assert::AreEqual(2u, testVectorArray->Size());
			vec4 v2(1.1f, 1.1f, 1.1f, 1.1f), v3(2.2f, 2.2f, 2.2f, 2.2f);
			Assert::IsTrue(v2 == testVectorArray->GetVector(0u));
			Assert::IsTrue(v3 == testVectorArray->GetVector(1u));

			Datum* testMatrix = populatedScope.Find("testMatrix");
			Assert::IsNotNull(testMatrix);
			Assert::AreEqual(1u, testMatrix->Size());
			mat4 m1(0.0f, 0.1f, 0.2f, 0.3f, 1.0f, 1.1f, 1.2f, 1.3f, 2.0f, 2.1f, 2.2f, 2.3f, 3.0f, 3.1f, 3.2f, 3.3f);
			Assert::IsTrue(m1 == testMatrix->GetMatrix());

			Datum* testMatrixArray = populatedScope.Find("testMatrixArray");
			Assert::IsNotNull(testMatrixArray);
			Assert::AreEqual(2u, testMatrixArray->Size());
			Assert::IsTrue(m1 == testMatrixArray->GetMatrix(0u));
			Assert::IsTrue(m1 == testMatrixArray->GetMatrix(1u));

			Datum* testString = populatedScope.Find("testString");
			Assert::IsNotNull(testString);
			Assert::AreEqual(1u, testString->Size());
			Assert::IsTrue("Testing" == testString->GetString());

			Datum* testStringArray = populatedScope.Find("testStringArray");
			Assert::IsNotNull(testStringArray);
			Assert::AreEqual(2u, testStringArray->Size());
			Assert::IsTrue("hello" == testStringArray->GetString(0u));
			Assert::IsTrue("world" == testStringArray->GetString(1u));

			Datum* testScope = populatedScope.Find("testScope");
			Assert::IsNotNull(testScope);
			Assert::AreEqual(1u, testScope->Size());
			Scope* childScope = &testScope->GetScope();
			Assert::IsNotNull(childScope);
			Assert::AreEqual(1u, childScope->Size());
			
			Datum* childString = childScope->Find("childString");
			Assert::IsNotNull(childString);
			Assert::AreEqual(1u, childString->Size());
			Assert::IsTrue("Hello world" == childString->GetString());
		}
	};
}