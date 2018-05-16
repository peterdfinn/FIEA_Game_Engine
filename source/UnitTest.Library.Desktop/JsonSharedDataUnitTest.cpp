#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(JsonSharedDataUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
	public:
		TEST_METHOD_INITIALIZE(JsonSharedDataUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(JsonSharedDataUnitTestCleanup)
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

		TEST_METHOD(JsonSharedDataDefaultConstructorTest)
		{
			/*I expect the JsonSharedData default constructor to set depth to 0 and
			mParseMaster to nullptr.*/
			JsonSharedData sharedData;
			Assert::IsTrue(nullptr == sharedData.GetParseMaster());
			Assert::AreEqual(0u, sharedData.Depth());
		}

		TEST_METHOD(JsonSharedDataConstructorTest)
		{
			/*When a JsonSharedData is constructed with one argument, I expect
			mParseMaster to be set to that value, which should represent a
			pointer to a ParseMaster. I also expect depth to be 0.*/
			JsonParseMaster master;
			JsonSharedData sharedData(&master);
			Assert::IsTrue(&master == sharedData.GetParseMaster());
			Assert::AreEqual(0u, sharedData.Depth());
		}

		TEST_METHOD(JsonSharedDataCopyConstructorTest)
		{
			/*When a JsonSharedData is constructed with the copy constructor, I
			expect both the copy and the original to point to the same
			JsonParseMaster, and I expect both to have the same depth.*/
			JsonParseMaster master;
			JsonSharedData original(&master);
			original.IncrementDepth();
			const JsonSharedData copy(original);
			Assert::AreEqual(original.Depth(), copy.Depth());
			Assert::IsTrue(copy.GetParseMaster() == &master);
		}

		TEST_METHOD(JsonSharedDataCopyAssignmentOperatorTest)
		{
			/*When a JsonSharedData is assigned with the copy assignment
			operator, I expect both the copy and the original to point to the
			same JsonParseMaster, and I expect both to have the same depth.*/
			JsonParseMaster master1, master2;
			JsonSharedData sharedData1(&master1), sharedData2(&master2);
			sharedData1.IncrementDepth();
			sharedData1 = sharedData2;
			Assert::AreEqual(0u, sharedData1.Depth());
			Assert::IsTrue(sharedData1.GetParseMaster() == &master2);
		}

		TEST_METHOD(JsonSharedDataMoveConstructorTest)
		{
			/*When a JsonSharedData is constructed with the move constructor, I
			expect the new object to have its depth and its pointer to a
			JsonParseMaster copied from the original object, and I expect the
			original object to have its depth set to 0 and its pointer to a
			JsonParseMaster set to nullptr.*/
			JsonParseMaster master;
			JsonSharedData original(&master);
			original.IncrementDepth();
			JsonSharedData copy(move(original));
			Assert::AreEqual(1u, copy.Depth());
			Assert::IsTrue(copy.GetParseMaster() == &master);
			Assert::AreEqual(0u, original.Depth());
			Assert::IsTrue(original.GetParseMaster() == nullptr);
		}

		TEST_METHOD(JsonSharedDataMoveAssignmentOperatorTest)
		{
			/*When a JsonSharedData is assigned with the move assignment
			operator, I expect the new object to have its depth and its pointer
			to a JsonParseMaster copied from the original object, and I expect
			the original object to have its depth set to 0 and its pointer to a
			JsonParseMaster set to nullptr.*/
			JsonParseMaster master;
			JsonSharedData original(&master);
			original.IncrementDepth();
			JsonSharedData copy;
			copy = move(original);
			Assert::AreEqual(1u, copy.Depth());
			Assert::IsTrue(copy.GetParseMaster() == &master);
			Assert::AreEqual(0u, original.Depth());
			Assert::IsTrue(original.GetParseMaster() == nullptr);
		}

		TEST_METHOD(JsonSharedDataCloneTest)
		{
			/*I expect Clone to dynamically allocate a new JsonSharedData
			object, with its mParseMaster equal to the original object's
			mParseMaster, and return a pointer to it. Regardless of the state of
			the original JsonSharedData object, the clone should be ready to
			parse a new JSON file, which means its depth should be 0.*/

			//setting up stress conditions
			JsonParseMaster master;
			JsonSharedData sharedDataOriginal(&master);
			sharedDataOriginal.IncrementDepth();

			//testing
			const JsonSharedData* const clone = sharedDataOriginal.Clone();
			Assert::IsTrue(clone->GetParseMaster() == &master);
			Assert::AreEqual(0u, clone->Depth());
			delete clone;
		}

		TEST_METHOD(SharedDataIncrementDecrementDepthTest)
		{
			JsonSharedData d;
			Assert::AreEqual(1u, d.IncrementDepth());
			Assert::AreEqual(0u, d.DecrementDepth());
		}

		TEST_METHOD(SharedDataGetSetParseMasterTest)
		{
			/*I expect SetParseMaster to simply set the ParseMaster pointer
			within this object to the input pointer. I expect GetParseMaster to
			return this pointer.*/
			JsonParseMaster master1, master2;
			JsonSharedData shared(&master1);

			shared.SetParseMaster(&master2);
			Assert::IsTrue(&master2 == shared.GetParseMaster());

			shared.SetParseMaster(&master1);
			Assert::IsTrue(&master1 == shared.GetParseMaster());
		}

		TEST_METHOD(SharedDataInitializeTest)
		{
			/*I expect Initialize to set the depth to 0.*/
			JsonSharedData d;
			d.Initialize();
			Assert::AreEqual(0u, d.Depth());
		}
	};
}