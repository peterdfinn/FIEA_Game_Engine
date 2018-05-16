#include "pch.h"

using namespace std;
using namespace glm;
using namespace LibraryShared;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTest
{
	TEST_CLASS(AttributedUnitTest)
	{
	private:
		AttributedFoo dummy;
		int32_t testInt = -15;
		float testFloat = -15.55f;
		vec4 testVector = { 1.0f, 2.0f, 3.0f, 4.0f };
		mat4 testMatrix = { testVector, testVector, testVector, testVector };
		string testString = "test";
		RTTI* testPointer = &dummy;
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

		TEST_METHOD(AttributedDefaultConstructorTest)
		{
			/*I expect the HashMap to contain 7 Datums (Data?): a pointer to
			this, and the 6 member variables of AttributedFoo.*/
			AttributedFoo a(testInt, testFloat, testVector, testMatrix, testString, testPointer);
			Assert::AreEqual(7u, a.Size());

			Assert::AreEqual(testInt, a.Find("integerAttribute"s)->GetInt());
			Assert::AreEqual(testFloat, a.Find("floatAttribute"s)->GetFloat());
			Assert::IsTrue(testVector == a.Find("vectorAttribute"s)->GetVector());
			Assert::IsTrue(testMatrix == a.Find("matrixAttribute"s)->GetMatrix());
			Assert::IsTrue(testString == a.Find("stringAttribute"s)->GetString());
			Assert::IsTrue(testPointer == a.Find("pointerAttribute"s)->GetPointer());
		}

		TEST_METHOD(AttributedCopyConstructorTest)
		{
			/*I expect the HashMap to have been deep copied.*/
			AttributedFoo original;
			AttributedFoo copy(original);

			const Datum* foundOriginal_integer = original.Find("integerAttribute");
			const Datum* foundCopy_integer = copy.Find("integerAttribute");
			Assert::IsFalse(foundOriginal_integer == nullptr);
			Assert::IsFalse(foundCopy_integer == nullptr);
			Assert::IsFalse(foundCopy_integer == foundOriginal_integer);

			const Datum* foundOriginal_float = original.Find("floatAttribute");
			const Datum* foundCopy_float = copy.Find("floatAttribute");
			Assert::IsFalse(foundOriginal_float == nullptr);
			Assert::IsFalse(foundCopy_float == nullptr);
			Assert::IsFalse(foundCopy_float == foundOriginal_float);

			const Datum* foundOriginal_vector = original.Find("vectorAttribute");
			const Datum* foundCopy_vector = copy.Find("vectorAttribute");
			Assert::IsFalse(foundOriginal_vector == nullptr);
			Assert::IsFalse(foundCopy_vector == nullptr);
			Assert::IsFalse(foundCopy_vector == foundOriginal_vector);

			const Datum* foundOriginal_matrix = original.Find("matrixAttribute");
			const Datum* foundCopy_matrix = copy.Find("matrixAttribute");
			Assert::IsFalse(foundOriginal_matrix == nullptr);
			Assert::IsFalse(foundCopy_matrix == nullptr);
			Assert::IsFalse(foundCopy_matrix == foundOriginal_matrix);

			const Datum* foundOriginal_string = original.Find("stringAttribute");
			const Datum* foundCopy_string = copy.Find("stringAttribute");
			Assert::IsFalse(foundOriginal_string == nullptr);
			Assert::IsFalse(foundCopy_string == nullptr);
			Assert::IsFalse(foundCopy_string == foundOriginal_string);

			const Datum* foundOriginal_pointer = original.Find("pointerAttribute");
			const Datum* foundCopy_pointer = copy.Find("pointerAttribute");
			Assert::IsFalse(foundOriginal_pointer == nullptr);
			Assert::IsFalse(foundCopy_pointer == nullptr);
			Assert::IsFalse(foundCopy_pointer == foundOriginal_pointer);
		}

		TEST_METHOD(AttributedMoveConstructorTest)
		{
			/*I expect the HashMap to have been moved, resulting in an empty
			HashMap in the original.*/
			AttributedFoo original;
			AttributedFoo copy = move(original);

			const Datum* foundOriginal_integer = original.Find("integerAttribute");
			const Datum* foundCopy_integer = copy.Find("integerAttribute");
			Assert::IsTrue(foundOriginal_integer == nullptr);
			Assert::IsFalse(foundCopy_integer == nullptr);

			const Datum* foundOriginal_float = original.Find("floatAttribute");
			const Datum* foundCopy_float = copy.Find("floatAttribute");
			Assert::IsTrue(foundOriginal_float == nullptr);
			Assert::IsFalse(foundCopy_float == nullptr);

			const Datum* foundOriginal_vector = original.Find("vectorAttribute");
			const Datum* foundCopy_vector = copy.Find("vectorAttribute");
			Assert::IsTrue(foundOriginal_vector == nullptr);
			Assert::IsFalse(foundCopy_vector == nullptr);

			const Datum* foundOriginal_matrix = original.Find("matrixAttribute");
			const Datum* foundCopy_matrix = copy.Find("matrixAttribute");
			Assert::IsTrue(foundOriginal_matrix == nullptr);
			Assert::IsFalse(foundCopy_matrix == nullptr);

			const Datum* foundOriginal_string = original.Find("stringAttribute");
			const Datum* foundCopy_string = copy.Find("stringAttribute");
			Assert::IsTrue(foundOriginal_string == nullptr);
			Assert::IsFalse(foundCopy_string == nullptr);

			const Datum* foundOriginal_pointer = original.Find("pointerAttribute");
			const Datum* foundCopy_pointer = copy.Find("pointerAttribute");
			Assert::IsTrue(foundOriginal_pointer == nullptr);
			Assert::IsFalse(foundCopy_pointer == nullptr);
		}

		TEST_METHOD(AttributedCopyAssignmentOperatorTest)
		{
			/*I expect the HashMap to have been deep copied.*/
			AttributedFoo original, copy;
			copy = original;

			const Datum* foundOriginal_integer = original.Find("integerAttribute");
			const Datum* foundCopy_integer = copy.Find("integerAttribute");
			Assert::IsFalse(foundOriginal_integer == nullptr);
			Assert::IsFalse(foundCopy_integer == nullptr);
			Assert::IsFalse(foundCopy_integer == foundOriginal_integer);

			const Datum* foundOriginal_float = original.Find("floatAttribute");
			const Datum* foundCopy_float = copy.Find("floatAttribute");
			Assert::IsFalse(foundOriginal_float == nullptr);
			Assert::IsFalse(foundCopy_float == nullptr);
			Assert::IsFalse(foundCopy_float == foundOriginal_float);

			const Datum* foundOriginal_vector = original.Find("vectorAttribute");
			const Datum* foundCopy_vector = copy.Find("vectorAttribute");
			Assert::IsFalse(foundOriginal_vector == nullptr);
			Assert::IsFalse(foundCopy_vector == nullptr);
			Assert::IsFalse(foundCopy_vector == foundOriginal_vector);

			const Datum* foundOriginal_matrix = original.Find("matrixAttribute");
			const Datum* foundCopy_matrix = copy.Find("matrixAttribute");
			Assert::IsFalse(foundOriginal_matrix == nullptr);
			Assert::IsFalse(foundCopy_matrix == nullptr);
			Assert::IsFalse(foundCopy_matrix == foundOriginal_matrix);

			const Datum* foundOriginal_string = original.Find("stringAttribute");
			const Datum* foundCopy_string = copy.Find("stringAttribute");
			Assert::IsFalse(foundOriginal_string == nullptr);
			Assert::IsFalse(foundCopy_string == nullptr);
			Assert::IsFalse(foundCopy_string == foundOriginal_string);

			const Datum* foundOriginal_pointer = original.Find("pointerAttribute");
			const Datum* foundCopy_pointer = copy.Find("pointerAttribute");
			Assert::IsFalse(foundOriginal_pointer == nullptr);
			Assert::IsFalse(foundCopy_pointer == nullptr);
			Assert::IsFalse(foundCopy_pointer == foundOriginal_pointer);
		}

		TEST_METHOD(AttributedMoveAssignmentOperatorTest)
		{
			/*I expect the HashMap to have been moved, resulting in an empty
			HashMap in the original.*/
			AttributedFoo original;
			AttributedFoo copy;
			copy = move(original);

			const Datum* foundOriginal_integer = original.Find("integerAttribute");
			const Datum* foundCopy_integer = copy.Find("integerAttribute");
			Assert::IsTrue(foundOriginal_integer == nullptr);
			Assert::IsFalse(foundCopy_integer == nullptr);

			const Datum* foundOriginal_float = original.Find("floatAttribute");
			const Datum* foundCopy_float = copy.Find("floatAttribute");
			Assert::IsTrue(foundOriginal_float == nullptr);
			Assert::IsFalse(foundCopy_float == nullptr);

			const Datum* foundOriginal_vector = original.Find("vectorAttribute");
			const Datum* foundCopy_vector = copy.Find("vectorAttribute");
			Assert::IsTrue(foundOriginal_vector == nullptr);
			Assert::IsFalse(foundCopy_vector == nullptr);

			const Datum* foundOriginal_matrix = original.Find("matrixAttribute");
			const Datum* foundCopy_matrix = copy.Find("matrixAttribute");
			Assert::IsTrue(foundOriginal_matrix == nullptr);
			Assert::IsFalse(foundCopy_matrix == nullptr);

			const Datum* foundOriginal_string = original.Find("stringAttribute");
			const Datum* foundCopy_string = copy.Find("stringAttribute");
			Assert::IsTrue(foundOriginal_string == nullptr);
			Assert::IsFalse(foundCopy_string == nullptr);

			const Datum* foundOriginal_pointer = original.Find("pointerAttribute");
			const Datum* foundCopy_pointer = copy.Find("pointerAttribute");
			Assert::IsTrue(foundOriginal_pointer == nullptr);
			Assert::IsFalse(foundCopy_pointer == nullptr);
		}

		TEST_METHOD(AttributedIsAttributeTest)
		{
			AttributedFoo f;
			Assert::IsTrue(f.IsAttribute("this"));
			Assert::IsTrue(f.IsAttribute("integerAttribute"));
			Assert::IsTrue(f.IsAttribute("floatAttribute"));
			Assert::IsTrue(f.IsAttribute("vectorAttribute"));
			Assert::IsTrue(f.IsAttribute("matrixAttribute"));
			Assert::IsTrue(f.IsAttribute("pointerAttribute"));
			Assert::IsTrue(f.IsAttribute("stringAttribute"));
			Assert::IsFalse(f.IsAttribute("integerAttribut"));
		}

		TEST_METHOD(AttributedIsPrescribedAttributeTest)
		{
			AttributedFoo f;
			Assert::IsTrue(f.IsPrescribedAttribute("integerAttribute"));
			Assert::IsTrue(f.IsPrescribedAttribute("floatAttribute"));
			Assert::IsTrue(f.IsPrescribedAttribute("vectorAttribute"));
			Assert::IsTrue(f.IsPrescribedAttribute("matrixAttribute"));
			Assert::IsTrue(f.IsPrescribedAttribute("pointerAttribute"));
			Assert::IsTrue(f.IsPrescribedAttribute("stringAttribute"));
			Assert::IsFalse(f.IsPrescribedAttribute("integerAttribut"));
			Assert::IsTrue(f.IsPrescribedAttribute("this"));
		}

		TEST_METHOD(AttributedIsAuxiliaryAttributeTest)
		{
			AttributedFoo f;
			Assert::IsFalse(f.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(f.IsAuxiliaryAttribute("integerAttribute"));
			Assert::IsFalse(f.IsAuxiliaryAttribute("floatAttribute"));
			Assert::IsFalse(f.IsAuxiliaryAttribute("vectorAttribute"));
			Assert::IsFalse(f.IsAuxiliaryAttribute("matrixAttribute"));
			Assert::IsFalse(f.IsAuxiliaryAttribute("pointerAttribute"));
			Assert::IsFalse(f.IsAuxiliaryAttribute("stringAttribute"));
			Assert::IsFalse(f.IsAuxiliaryAttribute("integerAttribut"));
		}
	};
}