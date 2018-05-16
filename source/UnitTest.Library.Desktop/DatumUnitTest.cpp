#include "pch.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_CXX98
#include "Datum.h"
#include "glm\gtx\string_cast.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;
using namespace glm;

namespace UnitTest
{
	TEST_CLASS(DatumUnitTest)
	{
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

		TEST_METHOD(DatumDefaultConstructorTest)
		{
			const Datum d;
			Assert::AreEqual(0u, d.Size());
			Assert::AreEqual(0u, d.Capacity());
			Assert::IsTrue(d.Type() == DatumType::UNKNOWN);
		}

		TEST_METHOD(DatumCopyConstructorTest)
		{
			//testing for copy of empty Datum
			const Datum d;
			const Datum copy(d);
			Assert::IsTrue(d == copy);
			Assert::AreEqual(0u, copy.Size());
			auto lambda1 = [&copy]() { copy.GetInt(); };
			Assert::ExpectException<exception>(lambda1);

			//testing for copy of internal ints
			Datum internalInt = { 6, 5, 4, 3, 2, 1 };
			Datum internalIntCopy(internalInt);
			internalInt = 7; //shouldn't throw exception
			internalIntCopy = 7; //shouldn't throw exception
			internalIntCopy.Reserve(100u); //shouldn't throw exception
			Assert::IsTrue(internalInt == internalIntCopy);

			//testing for copy of external ints
			Datum externalInt = { 3, 2, 1 };
			Datum internalInt2 = std::move(externalInt);
			Datum externalIntCopy(externalInt);
			auto lambda2 = [&externalIntCopy]() { externalIntCopy.Reserve(100u); };
			Assert::ExpectException<exception>(lambda2);
			Assert::IsTrue(externalInt == externalIntCopy);

			//testing for copy of internal floats
			Datum internalFloat = { 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };
			Datum internalFloatCopy(internalFloat);
			internalFloat = 7.0f; //shouldn't throw exception
			internalFloatCopy = 7.0f; //shouldn't throw exception
			internalFloatCopy.Reserve(200u); //shouldn't throw exception
			Assert::IsTrue(internalFloat == internalFloatCopy);

			//testing for copy of external floats
			Datum externalFloat = { 3.0f, 2.0f, 1.0f };
			Datum internalFloat2 = std::move(externalFloat);
			Datum externalFloatCopy(externalFloat);
			auto lambda3 = [&externalFloatCopy]() { externalFloatCopy.Reserve(100u); };
			Assert::ExpectException<exception>(lambda3);
			Assert::IsTrue(externalFloat == externalFloatCopy);

			//testing for copy of internal vectors
			glm::vec4 vectors[4];
			for (int i = 0; i < 4; ++i)
			{
				vectors[i] = glm::vec4(i, i, i, i);
			}
			Datum internalVector = { vectors[0], vectors[1], vectors[2], vectors[3] };
			Datum internalVectorCopy(internalVector);
			internalVector = vectors[3]; //shouldn't throw exception
			internalVectorCopy = vectors[3]; //shouldn't throw exception
			internalVectorCopy.Reserve(20u); //shouldn't throw exception
			Assert::IsTrue(internalVector == internalVectorCopy);

			//testing for copy of external vectors
			Datum externalVector = { vectors[3], vectors[2], vectors[1] };
			Datum internalVector2 = std::move(externalVector);
			Datum externalVectorCopy(externalVector);
			auto lambda4 = [&externalVectorCopy]() { externalVectorCopy.Reserve(100u); };
			Assert::ExpectException<exception>(lambda3);
			Assert::IsTrue(externalVector == externalVectorCopy);

			//testing for copy of internal matrices
			glm::mat4 matrices[4];
			for (int i = 0; i < 4; ++i)
			{
				matrices[i] = glm::mat4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i);
			}
			Datum internalMatrix = { matrices[0], matrices[1], matrices[2], matrices[3] };
			Datum internalMatrixCopy(internalMatrix);
			internalMatrix = matrices[3]; //shouldn't throw exception
			internalMatrixCopy = matrices[3]; //shouldn't throw exception
			internalMatrixCopy.Reserve(12u); //shouldn't throw excpetion
			Assert::IsTrue(internalMatrix == internalMatrixCopy);

			//testing for copy of external matrices
			Datum externalMatrix = { matrices[3], matrices[2], matrices[1] };
			Datum internalMatrix2 = std::move(externalMatrix);
			Datum externalMatrixCopy(externalMatrix);
			auto lambda5 = [&externalMatrixCopy]() { externalMatrixCopy.Reserve(100u); };
			Assert::ExpectException<exception>(lambda3);
			Assert::IsTrue(externalMatrix == externalMatrixCopy);

			//testing for copy of internal strings
			Datum internalString = { "Viva", "Pinata:", "Filled", "with", "Fun!" };
			Datum internalStringCopy(internalString);
			internalString = "blah"; //shouldn't throw exception
			internalStringCopy = "blah"; //shouldn't throw exception
			internalStringCopy.Reserve(39u); //shouldn't throw exception
			Assert::IsTrue(internalString == internalStringCopy);

			//testing for copy of external strings
			Datum externalString = { "So", "what" };
			Datum internalString2 = std::move(externalString);
			Datum externalStringCopy(externalString);
			auto lambda6 = [&externalStringCopy]() { externalStringCopy.Reserve(100u); };
			Assert::ExpectException<exception>(lambda6);
			Assert::IsTrue(externalString == externalStringCopy);
			
			//testing for copy of internal pointers
			Foo foos[4];
			for (int i = 0; i < 4; ++i)
			{
				Foo foo(i);
				foos[i] = foo;
			}
			Datum internalPointer = { &foos[0], &foos[1], &foos[2], &foos[3] };
			Datum internalPointerCopy(internalPointer);
			internalPointer = &foos[3]; //shouldn't throw exception
			internalPointerCopy = &foos[3]; //shouldn't throw exception
			internalPointerCopy.Reserve(12u); //shouldn't throw excpetion
			Assert::IsTrue(internalPointer == internalPointerCopy);

			//testing for copy of external pointers
			Datum externalPointer = { &foos[3], &foos[2], &foos[1] };
			Datum internalPointer2 = std::move(externalPointer);
			Datum externalPointerCopy(externalPointer);
			auto lambda7 = [&externalPointerCopy]() { externalPointerCopy.Reserve(100u); };
			Assert::ExpectException<exception>(lambda7);
			Assert::IsTrue(externalPointer == externalPointerCopy);
		}
		
		TEST_METHOD(DatumMoveConstructorTest)
		{
			//testing for move of empty Datum
			Datum d;
			Datum dMove = move(d);
			Assert::IsTrue(d == dMove);
			Assert::AreEqual(0u, dMove.Size());
			auto lambda0 = [&dMove]() { dMove.GetInt(); };
			Assert::ExpectException<exception>(lambda0);

			//testing for move of internal ints
			Datum internalInt = { 6, 5, 4, 3, 2, 1 };
			Datum internalIntMove = move(internalInt);
			auto lambda1 = [&internalInt]() {internalInt.Reserve(10u); };
			Assert::ExpectException<exception>(lambda1);
			Assert::IsTrue(internalInt == internalIntMove);
			internalIntMove.Reserve(10u); //shouldn't cause an exception

			//testing for move of external ints
			Datum externalInt = { 3, 2, 1 };
			Datum internalInt2 = std::move(externalInt); //making externalInt external
			Datum externalIntMove(externalInt);
			auto lambda2 = [&externalIntMove]() { externalIntMove.Reserve(100u); };
			auto lambda3 = [&externalInt]() { externalInt.Reserve(10u); };
			Assert::ExpectException<exception>(lambda2);
			Assert::ExpectException<exception>(lambda3);
			Assert::IsTrue(externalInt == externalIntMove);
			externalInt = 4;
			Assert::AreEqual(4, externalIntMove.GetInt());

			//testing for move of internal floats
			Datum internalFloat = { 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };
			Datum internalFloatMove = move(internalFloat);
			auto lambda4 = [&internalFloat]() { internalFloat.Reserve(10u); };
			Assert::ExpectException<exception>(lambda4);
			internalFloatMove.Reserve(10u); //shouldn't cause an exception
			Assert::IsTrue(internalFloat == internalFloatMove);
											
			//testing for move of external floats
			Datum externalFloat = { 3.0f, 2.0f, 1.0f };
			Datum internalFloat2 = std::move(externalFloat); //making externalFloat external
			Datum externalFloatMove(externalFloat);
			auto lambda5 = [&externalFloatMove]() { externalFloatMove.Reserve(10u); };
			auto lambda6 = [&externalFloat]() { externalFloat.Reserve(10u); };
			Assert::ExpectException<exception>(lambda5);
			Assert::ExpectException<exception>(lambda6);
			Assert::IsTrue(externalFloat == externalFloatMove);
			externalFloat = 4.0f;
			Assert::AreEqual(4.0f, externalFloatMove.GetFloat());

			//testing for move of internal vectors
			glm::vec4 vectors[4];
			for (int i = 0; i < 4; ++i)
			{
				vectors[i] = glm::vec4(i, i, i, i);
			}
			Datum internalVector = { vectors[0], vectors[1], vectors[2], vectors[3] };
			Datum internalVectorMove = move(internalVector);
			auto lambda7 = [&internalVector]() { internalVector.Reserve(10u); };
			Assert::ExpectException<exception>(lambda7);
			Assert::IsTrue(internalVector == internalVectorMove);
			internalVectorMove.Reserve(10u); //shouldn't cause an exception

			//testing for move of external vectors
			Datum externalVector = { vectors[3], vectors[2], vectors[1] };
			Datum internalVector2 = std::move(externalVector); //making externalVector external
			Datum externalVectorMove(externalVector);
			auto lambda8 = [&externalVectorMove]() { externalVectorMove.Reserve(10u); };
			auto lambda9 = [&externalVector]() { externalVector.Reserve(10u); };
			Assert::ExpectException<exception>(lambda8);
			Assert::ExpectException<exception>(lambda9);
			Assert::IsTrue(externalVector == externalVectorMove);
			externalVector = vectors[0];
			Assert::IsTrue(vectors[0] == externalVectorMove.GetVector());

			//testing for move of internal matrices
			glm::mat4 matrices[4];
			for (int i = 0; i < 4; ++i)
			{
				matrices[i] = glm::mat4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i);
			}
			Datum internalMatrix = { matrices[0], matrices[1], matrices[2], matrices[3] };
			Datum internalMatrixMove = move(internalMatrix);
			auto lambda10 = [&internalMatrix]() { internalMatrix.Reserve(10u); };
			Assert::ExpectException<exception>(lambda10);
			Assert::IsTrue(internalMatrix == internalMatrixMove);
			internalMatrixMove.Reserve(10u); //shouldn't cause an exception

			//testing for move of external matrices
			Datum externalMatrix = { matrices[3], matrices[2], matrices[1] };
			Datum internalMatrix2 = std::move(externalMatrix); //making externalMatrix external
			Datum externalMatrixMove(externalMatrix);
			auto lambda11 = [&externalMatrixMove]() { externalMatrixMove.Reserve(10u); };
			auto lambda12 = [&externalMatrix]() { externalMatrix.Reserve(10u); };
			Assert::ExpectException<exception>(lambda11);
			Assert::ExpectException<exception>(lambda12);
			Assert::IsTrue(externalMatrix == externalMatrixMove);
			externalMatrix = matrices[0];
			Assert::IsTrue(matrices[0] == externalMatrixMove.GetMatrix());

			//testing for move of internal strings
			Datum internalString = { "Mustard", "on", "tha", "beat", "ho!" };
			Datum internalStringMove = move(internalString);
			auto lambda13 = [&internalString]() { internalString.Reserve(10u); };
			Assert::ExpectException<exception>(lambda13);
			Assert::IsTrue(internalString == internalStringMove);
			internalStringMove.Reserve(10u); //shouldn't cause an exception

			//testing for move of external strings
			Datum externalString = { "Thank", "U", "India" };
			Datum internalString2 = std::move(externalString); //making externalString external
			Datum externalStringMove(externalString);
			auto lambda14 = [&externalStringMove]() { externalStringMove.Reserve(10u); };
			auto lambda15 = [&externalString]() { externalString.Reserve(10u); };
			Assert::ExpectException<exception>(lambda14);
			Assert::ExpectException<exception>(lambda15);
			Assert::IsTrue(externalString == externalStringMove);
			internalString2 = "ummm";
			Assert::IsTrue("ummm" == externalStringMove.GetString());

			//testing for move of internal matrices
			Foo foos[4];
			for (int i = 0; i < 4; ++i)
			{
				Foo f(i * 2000001);
				foos[i] = f;
			}
			Datum internalPointer = { &foos[0], &foos[1], &foos[2], &foos[3] };
			Datum internalPointerMove = move(internalPointer);
			auto lambda16 = [&internalPointer]() { internalPointer.Reserve(10u); };
			Assert::ExpectException<exception>(lambda16);
			Assert::IsTrue(internalPointer == internalPointerMove);
			internalPointerMove.Reserve(10u); //shouldn't cause an exception

			//testing for move of external &foos
			Datum externalPointer = { &foos[3], &foos[2], &foos[1] };
			Datum internalPointer2 = std::move(externalPointer); //making externalPointer external
			Datum externalPointerMove(externalPointer);
			auto lambda17 = [&externalPointerMove]() { externalPointerMove.Reserve(17u); };
			auto lambda18 = [&externalPointer]() { externalPointer.Reserve(10u); };
			Assert::ExpectException<exception>(lambda17);
			Assert::ExpectException<exception>(lambda18);
			Assert::IsTrue(externalPointer == externalPointerMove);
			externalPointer = &foos[0];
			Assert::IsTrue(&foos[0] == externalPointerMove.GetPointer());
		}

		TEST_METHOD(DatumInitializerListConstructorTest)
		{
			//testing for integers
			Datum dInt({ 0, 1, 2, 3, 4, 5, 6 });
			Assert::AreEqual(7u, dInt.Size());
			for (int i = 0; i < 7; ++i)
			{
				Assert::AreEqual(i, dInt.GetInt(i));
			}

			//testing for floats
			Datum dFloat({ 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f });
			Assert::AreEqual(7u, dFloat.Size());
			for (int i = 0; i < 7; ++i)
			{
				Assert::AreEqual(static_cast<float>(i), dFloat.GetFloat(i));
			}

			//testing for vectors
			glm::vec4 vectors[4];
			for (int i = 0; i < 4; ++i)
			{
				vectors[i] = glm::vec4(i, i, i, i);
			}
			Datum dVector({ vectors[0], vectors[1], vectors[2], vectors[3] });
			Assert::AreEqual(4u, dVector.Size());
			for (int i = 0; i < 4; ++i)
			{
				Assert::IsTrue(vectors[i] == dVector.GetVector(i));
			}

			//testing for matrices
			glm::mat4 matrices[4];
			for (int i = 0; i < 4; ++i)
			{
				matrices[i] = glm::mat4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i);
			}
			Datum dMatrix({ matrices[0], matrices[1], matrices[2], matrices[3] });
			Assert::AreEqual(4u, dMatrix.Size());
			for (int i = 0; i < 4; ++i)
			{
				Assert::IsTrue(matrices[i] == dMatrix.GetMatrix(i));
			}

			//testing for strings
			Datum dString({"One", "Two", "Three"});
			Assert::AreEqual(3u, dString.Size());
			Assert::IsTrue("Three" == dString.GetString(2u));
			Assert::IsTrue("Two" == dString.GetString(1u));
			Assert::IsTrue("One" == dString.GetString());

			//testing for pointers
			Foo foos[4];
			for (int i = 0; i < 4; ++i)
			{
				Foo f(i * 2000001);
				foos[i] = f;
			}
			Datum dPointer({ &foos[0], &foos[1], &foos[2], &foos[3] });
			Assert::AreEqual(4u, dPointer.Size());
			for (int i = 0; i < 4; ++i)
			{
				Assert::IsTrue(&foos[i] == dPointer.GetPointer(i));
			}

			//testing for scope pointers
			Scope s1;
			Scope* s2 = new Scope();
			s1.AppendScope("child");
			s1.Adopt("s2", *s2);
			Datum dScope = { &s1, s2 };
			Assert::AreEqual(2u, dScope.Size());
			Assert::IsTrue(&dScope.GetScope(0u) == &s1);
			Assert::IsTrue(&dScope.GetScope(1u) == s2);

			//delete s2;
		}

		TEST_METHOD(DatumAssignmentOperatorInitializerListTest)
		{
			//testing for integers assigned to integers
			Datum dInt({ 3 });
			dInt = {0, 1, 2, 3};
			Assert::AreEqual(4u, dInt.Size());
			for (uint32_t i = 0; i < dInt.Size(); ++i)
			{
				Assert::AreEqual(static_cast<int32_t>(i), dInt.GetInt(i));
			}

			//testing for integers assigned to empty
			Datum emptyInt;
			emptyInt = { 0, 1, 2, 3 };
			Assert::AreEqual(4u, emptyInt.Size());
			for (uint32_t i = 0; i < emptyInt.Size(); ++i)
			{
				Assert::AreEqual(static_cast<int32_t>(i), emptyInt.GetInt(i));
			}

			//testing for integers assigned to other
			Datum otherInt = { 2.0f };
			auto lambda1 = [&otherInt]() { otherInt = { 0, 1, 2 }; };
			Assert::ExpectException<exception>(lambda1);

			//testing for floats assigned to floats
			Datum dFloat({ 3.0f });
			dFloat = { 0.0f, 1.0f, 2.0f, 3.0f };
			Assert::AreEqual(4u, dFloat.Size());
			for (uint32_t i = 0; i < dFloat.Size(); ++i)
			{
				Assert::AreEqual(static_cast<float>(i), dFloat.GetFloat(i));
			}

			//testing for floats assigned to empty
			Datum emptyFloat;
			emptyFloat = { 0.0f, 1.0f, 2.0f, 3.0f };
			Assert::AreEqual(4u, emptyFloat.Size());
			for (uint32_t i = 0; i < emptyFloat.Size(); ++i)
			{
				Assert::AreEqual(static_cast<float>(i), emptyFloat.GetFloat(i));
			}

			//testing for floats assigned to other
			Datum otherFloat = { 10 };
			auto lambda2 = [&otherFloat]() { otherFloat = { 0.0f, 1.0f, 2.0f }; };
			Assert::ExpectException<exception>(lambda2);

			//testing for vectors assigned to vectors
			glm::vec4 vectors[4];
			for (int i = 0; i < 4; ++i)
			{
				vectors[i] = glm::vec4(i, i, i, i);
			}
			Datum dVector({vectors[1], vectors[2]});
			dVector = {vectors[0], vectors[1], vectors[2], vectors[3]};
			Assert::AreEqual(4u, dVector.Size());
			for (uint32_t i = 0u; i < dVector.Size(); ++i)
			{
				Assert::IsTrue(vectors[i] == dVector.GetVector(i));
			}

			//testing for vectors assigned to empty
			Datum emptyVector;
			emptyVector = {vectors[0], vectors[1], vectors[2], vectors[3]};
			Assert::AreEqual(4u, emptyVector.Size());
			for (uint32_t i = 0u; i < emptyVector.Size(); ++i)
			{
				Assert::IsTrue(vectors[i] == emptyVector.GetVector(i));
			}

			//testing for vectors assigned to other
			auto lambda3 = [&emptyInt, &vectors]() {emptyInt = { vectors[0] }; };
			Assert::ExpectException<exception>(lambda3);

			//testing for matrices assigned to matrices
			glm::mat4 matrices[4];
			for (int i = 0; i < 4; ++i)
			{
				matrices[i] = glm::mat4(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i);
			}
			Datum dMatrix({ matrices[1], matrices[2] });
			dMatrix = { matrices[0], matrices[1], matrices[2], matrices[3] };
			Assert::AreEqual(4u, dMatrix.Size());
			for (uint32_t i = 0u; i < dMatrix.Size(); ++i)
			{
				Assert::IsTrue(matrices[i] == dMatrix.GetMatrix(i));
			}

			//testing for matrices assigned to empty
			Datum emptyMatrix;
			emptyMatrix = { matrices[0], matrices[1], matrices[2], matrices[3] };
			Assert::AreEqual(4u, emptyMatrix.Size());
			for (uint32_t i = 0u; i < emptyMatrix.Size(); ++i)
			{
				Assert::IsTrue(matrices[i] == emptyMatrix.GetMatrix(i));
			}

			//testing for matrices assigned to other
			auto lambda4 = [&emptyFloat, &matrices]() {emptyFloat = { matrices[0] }; };
			Assert::ExpectException<exception>(lambda4);

			//testing for strings assigned to strings
			Datum dString({ "soljidfe" });
			dString = { "up", "side", "down"};
			Assert::AreEqual(3u, dString.Size());
			Assert::IsTrue("up" == dString.GetString(0));
			Assert::IsTrue("side" == dString.GetString(1));
			Assert::IsTrue("down" == dString.GetString(2));

			//testing for strings assigned to empty
			Datum emptyString;
			emptyString = { "what" };
			Assert::AreEqual(1u, emptyString.Size());
			Assert::IsTrue("what" == emptyString.GetString());

			//testing for strings assigned to other
			auto lambda5 = [&dInt]() { dInt = { "2364" }; };
			Assert::ExpectException<exception>(lambda5);

			//testing for pointers assigned to pointers
			Foo foos[4];
			for (int i = 0; i < 4; ++i)
			{
				Foo f(i - 15);
				foos[i] = f;
			}
			Datum dPointer({ &foos[1], &foos[2] });
			dPointer = { &foos[0], &foos[1], &foos[2], &foos[3] };
			Assert::AreEqual(4u, dPointer.Size());
			for (uint32_t i = 0u; i < dPointer.Size(); ++i)
			{
				Assert::IsTrue(&foos[i] == dPointer.GetPointer(i));
			}

			//testing for pointers assigned to empty
			Datum emptyPointer;
			emptyPointer = { &foos[0], &foos[1], &foos[2], &foos[3] };
			Assert::AreEqual(4u, emptyPointer.Size());
			for (uint32_t i = 0u; i < emptyPointer.Size(); ++i)
			{
				Assert::IsTrue(&foos[i] == emptyPointer.GetPointer(i));
			}

			//testing for pointers assigned to other
			auto lambda6 = [&emptyMatrix, &foos]() {emptyMatrix = { &foos[0] }; };
			Assert::ExpectException<exception>(lambda6);

			//testing for scope pointers assigned to scope pointers
			Scope s1;
			Scope* s2 = new Scope();
			s1.Adopt("2", *s2);
			Datum dScopePointer({ &s1 });
			dScopePointer = { s2, &s1 };
			Assert::AreEqual(2u, dScopePointer.Size());
			Assert::IsTrue(&dScopePointer.GetScope(0u) == s2);
			Assert::IsTrue(&dScopePointer.GetScope(1u) == &s1);

			//testing for scope pointers assigned to empty
			Datum emptyScopePointer;
			emptyScopePointer = { s2, s2, &s1 };
			Assert::AreEqual(3u, emptyScopePointer.Size());
			Assert::IsTrue(s2 == &emptyScopePointer.GetScope());
			Assert::IsTrue(s2 == &emptyScopePointer.GetScope(1u));
			Assert::IsTrue(&s1 == &emptyScopePointer.GetScope(2u));

			//testing for scope pointers assigned to other
			auto lambda7 = [&emptyMatrix, &s1]() {emptyMatrix = { &s1 }; };
			Assert::ExpectException<exception>(lambda7);
		}

		TEST_METHOD(DatumCopyAssignmentOperatorTest)
		{
			//testing for invalid assignment
			Datum bad1({ "wut" });
			Datum bad2({2});
			auto lambda1 = [&bad2, &bad1]() { bad2 = bad1; };
			Assert::ExpectException<exception>(lambda1);

			//testing internal = internal
			Datum int1({ "Sly", "Bentley", "Murray", "Carmelita" });
			Datum int2({ "Jak", "Daxter" });
			int2 = int1;
			Assert::IsTrue(int2 == int1);

			//testing internal = external
			Datum ext1 = {0.2f, 0.3f, 0.4f};
			Datum dummy1 = move(ext1); //making ext1 external
			Datum int3 = {0.1f};
			int3 = ext1;
			Assert::IsTrue(int3 == ext1);

			//testing external = internal
			Datum ext2 = { "Whirlm", "Sparrowmint", "Syrupent" };
			Datum int4 = { "Ratchet", "Clank", "Qwark", "Dr. Nefarious" };
			Datum dummy2 = move(ext2);
			ext2 = int4;
			Assert::IsTrue(ext2 == int4);

			//testing external = external
			Datum ext3 = {7.0f, 9.999f};
			Datum ext4 = {0.01f, 90.09f, -777.0f};
			Datum dummy3 = move(ext3);
			Datum dummy4 = move(ext4);
			ext4 = ext3;
			Assert::IsTrue(ext4 == ext3);
		}

		TEST_METHOD(DatumMoveAssignmentOperatorTest)
		{
			//testing for invalid assignment
			Datum bad1({ "wut" });
			Datum bad2({ 2 });
			auto lambda1 = [&bad2, &bad1]() { bad2 = move(bad1); };
			Assert::ExpectException<exception>(lambda1);

			//testing internal = internal
			Datum int1({ "Sly", "Bentley", "Murray", "Carmelita" });
			Datum int2({ "Jak", "Daxter" });
			int2 = move(int1);
			Assert::IsTrue(int2 == int1);

			////testing internal = external
			Datum ext1 = { 0.2f, 0.3f, 0.4f };
			Datum dummy1 = move(ext1); //making ext1 external
			Datum int3 = { 0.1f };
			int3 = move(ext1);
			Assert::IsTrue(int3 == ext1);

			//testing external = internal
			Datum ext2 = { "Whirlm", "Sparrowmint", "Syrupent" };
			Datum int4 = { "Ratchet", "Clank", "Qwark", "Dr. Nefarious" };
			Datum dummy2 = move(ext2);
			ext2 = move(int4);
			Assert::IsTrue(ext2 == int4);

			//testing external = external
			Datum ext3 = { 7.0f, 9.999f };
			Datum ext4 = { 0.01f, 90.09f, -777.0f };
			Datum dummy3 = move(ext3);
			Datum dummy4 = move(ext4);
			ext4 = move(ext3);
			Assert::IsTrue(ext4 == ext3);
		}

		TEST_METHOD(DatumOverloadedAssignmentOperatorTest)
		{
			//testing for ints
			Datum dInt = { 1 }, dInt2;
			dInt = 3;
			dInt2 = 3;
			Assert::AreEqual(3, dInt.GetInt());
			Assert::AreEqual(3, dInt2.GetInt());
			Datum bad1 = { 0.34f };
			auto lambda1 = [&bad1] { bad1 = 1; };
			Assert::ExpectException<exception>(lambda1);
			dInt.RemoveAtIndex(0);
			Datum dummy1 = move(dInt);//making dInt external
			Datum dummy1b = move(dInt2);
			auto lambda2 = [&dInt] { dInt = 22; };
			Assert::ExpectException<exception>(lambda2);

			//testing for floats
			Datum dFloat = { 1.0f }, dFloat2;
			dFloat = 3.0f;
			dFloat2 = 3.0f;
			Assert::AreEqual(3.0f, dFloat.GetFloat());
			Assert::AreEqual(3.0f, dFloat2.GetFloat());
			Datum bad2 = { 34 };
			auto lambda3 = [&bad2] { bad2 = 1.0f; };
			Assert::ExpectException<exception>(lambda3);
			dFloat.RemoveAtIndex(0);
			Datum dummy2 = move(dFloat);//making dFloat external
			auto lambda4 = [&dFloat] { dFloat = 22.0f; };
			Assert::ExpectException<exception>(lambda4);

			//testing for vectors
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f), v2(2.0f, 2.0f, 2.0f, 2.0f);
			Datum dVector({v1}), dVector2;
			dVector = v2;
			dVector2 = v2;
			Assert::IsTrue(v2 == dVector.GetVector());
			Assert::IsTrue(v2 == dVector2.GetVector());
			auto lambda5 = [&bad2, &v1]() { bad2 = v1; };
			Assert::ExpectException<exception>(lambda5);
			dVector.RemoveAtIndex(0u);
			Datum dummy3 = move(dVector);//making dVector external
			auto lambda6 = [&dVector, &v2]() { dVector = v2; };
			Assert::ExpectException<exception>(lambda6);

			//testing for matrices
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			mat4 m2(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
			Datum dMatrix({ m1 }), dMatrix2;
			dMatrix = m2;
			dMatrix2 = m2;
			Assert::IsTrue(m2 == dMatrix.GetMatrix());
			Assert::IsTrue(m2 == dMatrix2.GetMatrix());
			auto lambda7 = [&bad2, &m1]() { bad2 = m1; };
			Assert::ExpectException<exception>(lambda7);
			dMatrix.RemoveAtIndex(0u);
			Datum dummy4 = move(dMatrix);//making dMatrix external
			auto lambda8 = [&dMatrix, &m1]() { dMatrix = m1; };
			Assert::ExpectException<exception>(lambda8);

			//testing for strings
			Datum dString = { "yuk" }, dString2;
			dString = "yo";
			dString2 = "yo";
			Assert::IsTrue("yo" == dString.GetString());
			Assert::IsTrue("yo" == dString2.GetString());
			auto lambda9 = [&bad2] { bad2 = "asreyghs"; };
			Assert::ExpectException<exception>(lambda9);
			dString.RemoveAtIndex(0);
			Datum dummy5 = move(dString);//making dString external
			auto lambda10 = [&dString] { dString = "2"; };
			Assert::ExpectException<exception>(lambda10);

			//testing for pointers
			Foo f1(1), f2(2);
			Datum dPointer = { &f1 }, dPointer2;
			dPointer = &f2;
			dPointer2 = &f2;
			Assert::IsTrue(&f2 == dPointer.GetPointer());
			Assert::IsTrue(&f2 == dPointer2.GetPointer());
			auto lambda11 = [&bad2, &f1] { bad2 = &f1; };
			Assert::ExpectException<exception>(lambda11);
			dPointer.RemoveAtIndex(0);
			Datum dummy6 = move(dPointer);//making dPointer external
			auto lambda12 = [&dPointer, &f2] { dPointer = &f2; };
			Assert::ExpectException<exception>(lambda12);

			//testing for scope pointers
			Scope s1, s2;
			Datum dScopePointer = { &s1 }, dScopePointer2;
			dScopePointer = s2;
			dScopePointer2 = s2;
			Assert::IsTrue(&s2 == &dScopePointer.GetScope());
			Assert::IsTrue(&s2 == &dScopePointer2.GetScope());
			auto lambda13 = [&bad2, &s1] { bad2 = &s1; };
			Assert::ExpectException<exception>(lambda13);
			dScopePointer.RemoveAtIndex(0);
			Datum dummy7 = move(dScopePointer);//making dPointer external
			auto lambda14 = [&dScopePointer, &s2] { dScopePointer = &s2; };
			Assert::ExpectException<exception>(lambda14);
			Assert::IsFalse(dScopePointer == static_cast<Scope*>(nullptr));
		}

		TEST_METHOD(DatumShrinkToFitTest)
		{
			Datum dString = { "we547u" };
			Foo f(1);
			Datum dPointer = {&f};
			Datum dummy = move(dPointer);//making dPointer external
			auto lambda = [&dPointer] {dPointer.ShrinkToFit(); };
			Assert::ExpectException<exception>(lambda);
			
			Datum dInt = { 4 };
			dInt.Reserve(12u);
			dString.Reserve(12u);
			dInt.ShrinkToFit();
			dString.ShrinkToFit();
		}

		TEST_METHOD(DatumPushFrontTest)
		{
			//testing for ints
			Datum dInt;
			dInt.PushFront(-1);
			Assert::AreEqual(-1, dInt.GetInt());
			Datum dInt2 = dInt;
			auto lambda1 = [&dInt]() { dInt.PushFront(-1.0f); };
			Assert::ExpectException<exception>(lambda1);
			Datum dummy1 = move(dInt2);
			auto lambda2 = [&dInt2]() { dInt2.PushFront(11); };
			Assert::ExpectException<exception>(lambda2);

			//testing for floats
			Datum dFloat;
			dFloat.PushFront(-1.0f);
			Assert::AreEqual(-1.0f, dFloat.GetFloat());
			Datum dFloat2 = dFloat;
			auto lambda3 = [&dFloat]() { dFloat.PushFront(-1); };
			Assert::ExpectException<exception>(lambda3);
			Datum dummy2 = move(dFloat2);
			auto lambda4 = [&dFloat2]() { dFloat2.PushFront(-11.11f); };
			Assert::ExpectException<exception>(lambda4);

			//testing for vectors
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f), v2(2.0f, 2.0f, 2.0f, 2.0f);
			Datum dVector;
			dVector.PushFront(v1);
			Assert::IsTrue(v1 == dVector.GetVector());
			Datum dVector2 = dVector;
			Scope sTemp;
			auto lambda5 = [&dVector, &sTemp]() { dVector.PushFront(&sTemp); };
			Assert::ExpectException<exception>(lambda5);
			Datum dummy3 = move(dVector2);
			auto lambda6 = [&dVector2, &v2]() { dVector2.PushFront(v2); };
			Assert::ExpectException<exception>(lambda6);

			//testing for matrices
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			mat4 m2(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
			Datum dMatrix;
			dMatrix.PushFront(m1);
			Assert::IsTrue(m1 == dMatrix.GetMatrix());
			Datum dMatrix2 = dMatrix;
			auto lambda7 = [&dMatrix]() { dMatrix.PushFront("-10.2f"); };
			Assert::ExpectException<exception>(lambda7);
			Datum dummy4 = move(dMatrix2);
			auto lambda8 = [&dMatrix2, &m2]() { dMatrix2.PushFront(m2); };
			Assert::ExpectException<exception>(lambda8);

			//testing for strings
			Datum dString = {"Yo", "wazzup"};
			dString.PushFront("Whatever");
			Assert::IsTrue("Whatever" == dString.GetString());
			Datum dString2 = dString;
			auto lambda9 = [&dString, &m1]() { dString.PushFront(m1); };
			Assert::ExpectException<exception>(lambda9);
			Datum dummy5 = move(dString2);
			auto lambda10 = [&dString2]() { dString2.PushFront("Nah"); };
			Assert::ExpectException<exception>(lambda10);

			//testing for pointers
			Foo f1(1), f2(2);
			Datum dPointer;
			dPointer.PushFront(&f1);
			Assert::IsTrue(&f1 == dPointer.GetPointer());
			Datum dPointer2 = dPointer;
			auto lambda11 = [&dPointer, &v1]() { dPointer.PushFront(v1); };
			Assert::ExpectException<exception>(lambda11);
			Datum dummy6 = move(dPointer2);
			auto lambda12 = [&dPointer2, &f2]() { dPointer2.PushFront(&f2); };
			Assert::ExpectException<exception>(lambda12);

			//testing for scope pointers
			Scope s1, s2;
			Datum dScopePointer;
			dScopePointer.PushFront(s1);
			Assert::IsTrue(dScopePointer.Type() == DatumType::TABLE);
			Assert::IsTrue(&s1 == &dScopePointer.GetScope());
			Datum dScopePointer2 = dScopePointer;
			auto lambda13 = [&dScopePointer, &v1]() { dScopePointer.PushFront(v1); };
			Assert::ExpectException<exception>(lambda13);
			Datum dummy7 = move(dScopePointer2);
			auto lambda14 = [&dScopePointer2, &s2]() { dScopePointer2.PushFront(s2); };
			Assert::ExpectException<exception>(lambda14);
			auto lambda15 = [&dScopePointer, &f1]() { dScopePointer.PushFront(&f1); };
			Assert::ExpectException<exception>(lambda15);
		}

		TEST_METHOD(DatumPopFrontTest)
		{
			Datum dInt, dString = {"two", "three"};
			auto lambda = [&dInt]() { dInt.PopFront(); };
			Assert::ExpectException<exception>(lambda);
			dInt.PushFront(1);
			dString.PushFront("one");
			Assert::AreEqual(1u, dInt.Size());
			Assert::AreEqual(3u, dString.Size());
			dString.PopFront();
			dInt.PopFront();
			Assert::AreEqual(0u, dInt.Size());
			Assert::AreEqual(2u, dString.Size());
			dInt.PushFront(11);
			Datum dummy = move(dInt);
			auto lambda2 = [&dInt]() { dInt.PopFront(); };
			Assert::ExpectException<exception>(lambda2);
		}

		TEST_METHOD(DatumPushBackTest)
		{
			//testing for ints
			Datum dInt;
			dInt.PushBack(-1);
			Assert::AreEqual(-1, dInt.GetInt());
			Datum dInt2 = dInt;
			auto lambda1 = [&dInt]() { dInt.PushBack(-1.0f); };
			Assert::ExpectException<exception>(lambda1);
			Datum dummy1 = move(dInt2);
			auto lambda2 = [&dInt2]() { dInt2.PushBack(11); };
			Assert::ExpectException<exception>(lambda2);

			//testing for floats
			Datum dFloat;
			dFloat.PushBack(-1.0f);
			Assert::AreEqual(-1.0f, dFloat.GetFloat());
			Datum dFloat2 = dFloat;
			auto lambda3 = [&dFloat]() { dFloat.PushBack(-1); };
			Assert::ExpectException<exception>(lambda3);
			Datum dummy2 = move(dFloat2);
			auto lambda4 = [&dFloat2]() { dFloat2.PushBack(-11.11f); };
			Assert::ExpectException<exception>(lambda4);

			//testing for vectors
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f), v2(2.0f, 2.0f, 2.0f, 2.0f);
			Datum dVector;
			dVector.PushBack(v1);
			Assert::IsTrue(v1 == dVector.GetVector());
			Datum dVector2 = dVector;
			auto lambda5 = [&dVector]() { dVector.PushBack("zing"); };
			Assert::ExpectException<exception>(lambda5);
			Datum dummy3 = move(dVector2);
			auto lambda6 = [&dVector2, &v2]() { dVector2.PushBack(v2); };
			Assert::ExpectException<exception>(lambda6);

			//testing for matrices
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			mat4 m2(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
			Datum dMatrix;
			dMatrix.PushBack(m1);
			Assert::IsTrue(m1 == dMatrix.GetMatrix());
			Datum dMatrix2 = dMatrix;
			auto lambda7 = [&dMatrix, &v1]() { dMatrix.PushBack(v1); };
			Assert::ExpectException<exception>(lambda7);
			Datum dummy4 = move(dMatrix2);
			auto lambda8 = [&dMatrix2, &m2]() { dMatrix2.PushBack(m2); };
			Assert::ExpectException<exception>(lambda8);

			//testing for strings
			Datum dString;
			dString.PushBack("Whatever");
			Assert::IsTrue("Whatever" == dString.GetString());
			Datum dString2 = dString;
			auto lambda9 = [&dString, &m2]() { dString.PushBack(m2); };
			Assert::ExpectException<exception>(lambda9);
			Datum dummy5 = move(dString2);
			auto lambda10 = [&dString2]() { dString2.PushBack("Nah"); };
			Assert::ExpectException<exception>(lambda10);

			//testing for pointers
			Foo f1(1), f2(2);
			Datum dPointer;
			dPointer.PushBack(&f1);
			Assert::IsTrue(&f1 == dPointer.GetPointer());
			Datum dPointer2 = dPointer;
			auto lambda11 = [&dPointer, &v1]() { dPointer.PushBack(v1); };
			Assert::ExpectException<exception>(lambda11);
			Datum dummy6 = move(dPointer2);
			auto lambda12 = [&dPointer2, &f2]() { dPointer2.PushBack(&f2); };
			Assert::ExpectException<exception>(lambda12);
			auto lambda12b = [&dString, &f2]() { dString.PushBack(&f2); };
			Assert::ExpectException<exception>(lambda12b);

			//testing for scope pointers
			Scope s1, s2;
			Datum dScopePointer;
			dScopePointer.PushBack(s1);
			Assert::IsTrue(dScopePointer.Type() == DatumType::TABLE);
			Assert::IsTrue(&s1 == &dScopePointer.GetScope());
			Datum dScopePointer2 = dScopePointer;
			auto lambda13 = [&dScopePointer, &v1]() { dScopePointer.PushBack(v1); };
			Assert::ExpectException<exception>(lambda13);
			Datum dummy7 = move(dScopePointer2);
			auto lambda14 = [&dScopePointer2, &s2]() { dScopePointer2.PushBack(s2); };
			Assert::ExpectException<exception>(lambda14);
			auto lambda15 = [&dString, &s2]() { dString.PushBack(s2); };
			Assert::ExpectException<exception>(lambda15);
		}

		TEST_METHOD(DatumPopBackTest)
		{
			Datum dInt, dString;
			auto lambda = [&dInt]() { dInt.PopBack(); };
			Assert::ExpectException<exception>(lambda);
			dInt.PushBack(1);
			dString.PushBack("one");
			Assert::AreEqual(1u, dInt.Size());
			Assert::AreEqual(1u, dString.Size());
			dString.PopBack();
			dInt.PopBack();
			Assert::AreEqual(0u, dInt.Size());
			Assert::AreEqual(0u, dString.Size());
			dInt.PushBack(11);
			Datum dummy = move(dInt);
			auto lambda2 = [&dInt]() { dInt.PopBack(); };
			Assert::ExpectException<exception>(lambda2);
		}

		TEST_METHOD(DatumTypeTest)
		{
			Datum d1 = { 1 }, d2 = { 2.0f };
			vec4 v(1.0f, 1.0f, 1.0f, 1.0f);
			mat4 m(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			Foo f(1);
			Datum d3 = { v }, d4 = { m }, d5 = { "" }, d6 = { &f }, d7;
			Assert::IsTrue(DatumType::INTEGER == d1.Type());
			Assert::IsTrue(DatumType::FLOAT == d2.Type());
			Assert::IsTrue(DatumType::VECTOR == d3.Type());
			Assert::IsTrue(DatumType::MATRIX == d4.Type());
			Assert::IsTrue(DatumType::STRING == d5.Type());
			Assert::IsTrue(DatumType::POINTER == d6.Type());
			Assert::IsTrue(DatumType::UNKNOWN == d7.Type());
		}

		TEST_METHOD(DatumSetTypeTest)
		{
			Datum d1, d2 = { 2 };
			Assert::IsTrue(d1.SetType(DatumType::POINTER));
			Assert::IsTrue(d2.SetType(DatumType::INTEGER));
			Assert::IsTrue(DatumType::POINTER == d1.Type());
			Assert::IsFalse(d2.SetType(DatumType::STRING));
		}

		TEST_METHOD(DatumSizeTest)
		{
			Datum d1, d2({ 1, 2, 3, 4 });
			Assert::AreEqual(0u, d1.Size());
			Assert::AreEqual(4u, d2.Size());
		}

		TEST_METHOD(DatumSetSizeTest)
		{
			//testing for int
			Datum d1;
			auto lambda1 = [&d1]() { d1.SetSize(8u); };
			Assert::ExpectException<exception>(lambda1);
			d1.PushBack(4);
			Datum dummy = move(d1);
			Assert::ExpectException<exception>(lambda1);
			Datum d2 = { "I", "am", "tired" };
			d2.SetSize(1u);
			Assert::AreEqual(1u, d2.Size());
			uint32_t oldCapacity = d2.Capacity();
			d2.SetSize(oldCapacity + 1u);
			Assert::AreEqual(oldCapacity + 1u, d2.Size());
		}

		TEST_METHOD(DatumCapacityTest)
		{
			Datum d;
			Assert::AreEqual(0u, d.Capacity());
			int i;
			for (i = 0; i < rand(); ++i)
			{
				d.PushFront(i);
			}
			d.ShrinkToFit();
			Assert::AreEqual(static_cast<uint32_t>(i), d.Capacity());
		}

		TEST_METHOD(DatumClearTest)
		{
			Datum d = { "A", "B", "C", "D", "Q", "X", "Z", "$", "sailboat" };
			d.Clear();
			Assert::AreEqual(0u, d.Size());
			Assert::AreEqual(0u, d.Capacity());
			Assert::IsTrue(DatumType::UNKNOWN == d.Type());
		}

		TEST_METHOD(DatumIsEmptyTest)
		{
			Datum d1 = { 0.0f, 34.7f }, d2;
			Assert::IsTrue(d2.IsEmpty());
			Assert::IsFalse(d1.IsEmpty());
		}

		TEST_METHOD(DatumReserveTest)
		{
			//testing for exceptions
			Datum d;
			auto lambda = [&d]() { d.Reserve(4u); };
			Assert::ExpectException<exception>(lambda);
			d = { "5", "6", "7" };
			auto lambda2 = [&d]() { d.Reserve(1u); };
			Assert::ExpectException<exception>(lambda2);
			Datum dummy = move(d);
			Assert::ExpectException<exception>(lambda);

			//testing with ints
			Datum dInt = { 1, 2, 3, 4 };
			dInt.Reserve(10u);
			Assert::AreEqual(10u, dInt.Capacity());
			dInt.Remove(1);
			dInt.Remove(2);
			dInt.Remove(3);
			dInt.Remove(4);
			dInt.Reserve(20u);
			Assert::AreEqual(20u, dInt.Capacity());

			//testing with floats
			Datum dFloat = { 1.0f, 2.0f, 3.0f, 4.0f };
			dFloat.Reserve(10u);
			Assert::AreEqual(10u, dFloat.Capacity());
			dFloat.Remove(1.0f);
			dFloat.Remove(2.0f);
			dFloat.Remove(3.0f);
			dFloat.Remove(4.0f);
			dFloat.Reserve(20u);
			Assert::AreEqual(20u, dFloat.Capacity());

			//testing with vectors
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f);
			Datum dVector = { v1 };
			dVector.Reserve(10u);
			Assert::AreEqual(10u, dVector.Capacity());
			dVector.Remove(v1);
			dVector.Reserve(20u);
			Assert::AreEqual(20u, dVector.Capacity());

			//testing with matrices
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			Datum dMatrix = { m1 };
			dMatrix.Reserve(10u);
			Assert::AreEqual(10u, dMatrix.Capacity());
			dMatrix.Remove(m1);
			dMatrix.Reserve(20u);
			Assert::AreEqual(20u, dMatrix.Capacity());

			//testing with strings
			Datum dString = { "v1" };
			dString.Reserve(10u);
			Assert::AreEqual(10u, dString.Capacity());
			dString.Remove("v1");
			dString.Reserve(20u);
			Assert::AreEqual(20u, dString.Capacity());

			//testing with pointers
			Foo f1(0);
			Datum dPointer = { &f1 };
			dPointer.Reserve(10u);
			Assert::AreEqual(10u, dPointer.Capacity());
			dPointer.Remove(&f1);
			dPointer.Reserve(20u);
			Assert::AreEqual(20u, dPointer.Capacity());

			//testing with scope pointers
			Scope s1;
			Datum dScopePointer = { &s1 };
			dScopePointer.Reserve(10u);
			Assert::AreEqual(10u, dScopePointer.Capacity());
			dScopePointer.Remove(&s1);
			dScopePointer.Reserve(20u);
			Assert::AreEqual(20u, dScopePointer.Capacity());
		}

		TEST_METHOD(DatumRemoveTest)
		{
			//testing with ints
			Datum dInt = { 4, 5 };
			Assert::IsFalse(dInt.Remove(1.0f));
			Assert::IsFalse(dInt.Remove(3));
			Assert::IsTrue(dInt.Remove(4));
			Assert::AreEqual(1u, dInt.Size());

			//testing with floats
			Datum dFloat = { 4.4f, 5.5f };
			Assert::IsFalse(dFloat.Remove(1));
			Assert::IsFalse(dFloat.Remove(4.5f));
			Assert::IsTrue(dFloat.Remove(4.4f));
			Assert::AreEqual(1u, dFloat.Size());

			//testing with vectors
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f), v2(2.0f, 2.0f, 2.0f, 2.0f);
			Datum dVector = { v2, v1 };
			Assert::IsTrue(dVector.Remove(v2));
			Assert::AreEqual(1u, dVector.Size());
			Assert::IsFalse(dVector.Remove(v2));

			//testing with matrices
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			mat4 m2(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
			Datum dMatrix = {m1, m2};
			Assert::IsTrue(dMatrix.Remove(m1));
			Assert::AreEqual(1u, dMatrix.Size());
			Assert::IsFalse(dMatrix.Remove(m1));

			Assert::IsFalse(dVector.Remove(m1));
			Assert::IsFalse(dMatrix.Remove(v1));

			//testing with strings
			Datum dString = { "", "ASDF", "FDA" };
			Assert::IsTrue(dString.Remove("ASDF"));
			Assert::AreEqual(2u, dString.Size());
			Assert::IsFalse(dString.Remove("2)^&*("));

			//testing with pointers
			Foo f(111111), f2(-3);
			Datum dPointer = { &f, &f2, &f };
			Assert::IsTrue(dPointer.Remove(&f2));
			Assert::AreEqual(2u, dPointer.Size());
			Assert::IsFalse(dPointer.Remove(&f2));

			//testing with scope pointers
			Scope s1, s2;
			Datum dScopePointer = { &s1, &s2, &s1 };
			Assert::IsTrue(dScopePointer.Remove(s2));
			Assert::AreEqual(2u, dScopePointer.Size());
			Assert::IsFalse(dScopePointer.Remove(s2));

			Assert::IsFalse(dString.Remove(s2));
			Assert::IsFalse(dPointer.Remove("string"));
			Assert::IsFalse(dScopePointer.Remove(&f));
		}

		TEST_METHOD(DatumRemoveAtIndexTest)
		{
			Datum dString = { "Blah ", "blah", "BLAH" }, dInt = { 4, 5, 6, 7, 8 };
			dString.RemoveAtIndex(1u);
			dInt.RemoveAtIndex(3u);
			Assert::AreEqual(2u, dString.Size());
			Assert::AreEqual(4u, dInt.Size());
			auto lambda = [&dInt]() { dInt.RemoveAtIndex(4u); };
			Assert::ExpectException<exception>(lambda);
		}

		TEST_METHOD(DatumSetStorageTest)
		{
			//testing with ints
			Datum dInt1, dInt2 = { 1, 2 };
			int arrInt[4] = {0, 3, 5, 4};
			Assert::IsTrue(dInt1.SetStorage(arrInt, 4u));
			Assert::IsTrue(dInt2.SetStorage(arrInt, 4u));
			Assert::AreEqual(4u, dInt1.Size());
			Assert::AreEqual(4u, dInt2.Size());
			for (int i = 0; i < 4; ++i)
			{
				Assert::AreEqual(arrInt[i], dInt2.GetInt(i));
			}

			//testing with floats
			Datum dFloat1, dFloat2 = { 1.0f, 2.0f };
			float arrFloat[4] = {-10.2f, -10.3f, -3.0f, 8989898.89f};
			Assert::IsTrue(dFloat1.SetStorage(arrFloat, 4u));
			Assert::IsTrue(dFloat2.SetStorage(arrFloat, 4u));
			Assert::AreEqual(4u, dFloat1.Size());
			Assert::AreEqual(4u, dFloat2.Size());
			for (int i = 0; i < 4; ++i)
			{
				Assert::AreEqual(arrFloat[i], dFloat2.GetFloat(i));
			}

			//testing with vectors
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f), v2(2.0f, 2.0f, 2.0f, 2.0f);
			Datum dVector1, dVector2 = { v1, v2 };
			vec4 arrVector[4] = {v2, v1, v2, v1};
			Assert::IsTrue(dVector1.SetStorage(arrVector, 4u));
			Assert::IsTrue(dVector2.SetStorage(arrVector, 4u));
			Assert::AreEqual(4u, dVector1.Size());
			Assert::AreEqual(4u, dVector2.Size());
			for (int i = 0; i < 4; ++i)
			{
				Assert::IsTrue(arrVector[i] == dVector2.GetVector(i));
			}

			//testing with matrices
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			mat4 m2(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
			Datum dMatrix1, dMatrix2 = { m1, m2 };
			mat4 arrMatrix[4] = {m2, m1, m1, m2};
			Assert::IsTrue(dMatrix1.SetStorage(arrMatrix, 4u));
			Assert::IsTrue(dMatrix2.SetStorage(arrMatrix, 4u));
			for (int i = 0; i < 4; ++i)
			{
				Assert::IsTrue(arrMatrix[i] == dMatrix2.GetMatrix(i));
			}

			//testing with strings
			Datum dString1, dString2 = { "1.0f", "2.0f" };
			string arrString[4] = {"I", "didn't", "test", "this!!!!"};
			Assert::IsTrue(dString1.SetStorage(arrString, 4u));
			Assert::IsTrue(dString2.SetStorage(arrString, 4u));
			Assert::AreEqual(4u, dString1.Size());
			Assert::AreEqual(4u, dString2.Size());
			for (int i = 0; i < 4; ++i)
			{
				Assert::AreEqual(arrString[i], dString2.GetString(i));
			}

			//testing with pointers
			Foo f1(10), f2(20);
			Foo* arrPointer[4] = {&f2, &f1, &f1, &f2};
			Datum dPointer1, dPointer2 = { &f1, &f2 };
			Assert::IsTrue(dPointer1.SetStorage(reinterpret_cast<RTTI**>(arrPointer), 4u));
			Assert::IsTrue(dPointer2.SetStorage(reinterpret_cast<RTTI**>(arrPointer), 4u));
			Assert::AreEqual(4u, dPointer1.Size());
			Assert::AreEqual(4u, dPointer2.Size());
			for (int i = 0; i < 4; ++i)
			{
				Assert::IsTrue(dPointer1.GetPointer(i) == arrPointer[i]);
			}

			Assert::IsFalse(dInt1.SetStorage(arrString, 4u));
			Assert::IsFalse(dFloat1.SetStorage((arrInt), 4u));
			Assert::IsFalse(dVector1.SetStorage(arrMatrix, 4u));
			Assert::IsFalse(dMatrix1.SetStorage(arrVector, 4u));
			Assert::IsFalse(dString1.SetStorage(reinterpret_cast<RTTI**>(arrPointer), 4u));
			Assert::IsFalse(dPointer1.SetStorage(arrFloat, 4u));
		}

		TEST_METHOD(DatumSetTest)
		{
			//testing with ints
			Datum dInt = { 1, 2, 3, 4 };
			Assert::IsTrue(dInt.Set(0));
			Assert::AreEqual(0, dInt.GetInt());
			Assert::IsFalse(dInt.Set(24, 24u));

			//testing with floats
			Datum dFloat = { 1.0f, 2.0f, 3.0f, 4.0f };
			Assert::IsTrue(dFloat.Set(0.0f));
			Assert::AreEqual(0.0f, dFloat.GetFloat());
			Assert::IsFalse(dFloat.Set(24.0f, 24));

			//testing with vectors.
			vec4 v1 = {1.0f, 1.0f, 1.0f, 1.0f};
			Datum dVector = { v1, v1 };
			Assert::IsTrue(dVector.Set(v1));
			Assert::IsFalse(dVector.Set(v1, 24u));

			//testing with matrices
			mat4 m1 = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
			Datum dMatrix = { m1, m1 };
			Assert::IsTrue(dMatrix.Set(m1, 1u));
			Assert::IsFalse(dMatrix.Set(m1, 3u));

			//testing with strings
			Datum dString = { "what", "ever" };
			Assert::IsTrue(dString.Set("WHAT"));
			Assert::IsTrue("WHAT" == dString.GetString());
			Assert::IsFalse(dString.Set("WE", 34u));

			//testing with pointers
			Foo f1(1), f2(2);
			Datum dPointer = {&f1, &f2};
			Assert::IsTrue(dPointer.Set(&f2));
			Assert::IsTrue(&f2 == dPointer.GetPointer());
			Assert::IsFalse(dPointer.Set(&f2, 5u));

			//testing with scope pointers
			Scope s1, s2;
			s1.AppendScope("child");
			Datum dScopePointer = { &s1, &s2 };
			Assert::IsTrue(dScopePointer.Set(s2));
			Assert::IsTrue(&dScopePointer.GetScope() == &s2);
			Assert::IsFalse(dPointer.Set(s1, 10u));

			Assert::IsFalse(dInt.Set(10.0f));
			Assert::IsFalse(dFloat.Set(v1));
			Assert::IsFalse(dVector.Set(1));
			Assert::IsFalse(dMatrix.Set(s1));
			Assert::IsFalse(dString.Set(&f2));
			Assert::IsFalse(dPointer.Set(m1));
			Assert::IsFalse(dPointer.Set("blank"));
		}

		TEST_METHOD(DatumGetIntTest)
		{
			Datum dInt = { 1, 2, 3, 4 };
			Assert::IsTrue(dInt.GetInt(0) == 1);

			const Datum& dIntConst = dInt;
			Assert::IsTrue(dIntConst.GetInt(3) == 4);

			auto lambda = [&dInt]() { dInt.GetInt(45); };
			Assert::ExpectException<exception>(lambda);

			auto lambda2 = [&dIntConst]() { dIntConst.GetInt(5); };
			Assert::ExpectException<exception>(lambda2);

			Datum dEmpty;
			const Datum& dEmptyConst = dEmpty;
			auto lambda3 = [&dEmpty]() { dEmpty.GetInt(); };
			auto lambda4 = [&dEmptyConst]() { dEmptyConst.GetInt(); };
			Assert::ExpectException<exception>(lambda3);
			Assert::ExpectException<exception>(lambda4);
		}

		TEST_METHOD(DatumGetFloatTest)
		{
			Datum dFloat = { 1.0f, 2.0f, 3.0f, 4.0f };
			Assert::IsTrue(dFloat.GetFloat(0) == 1.0f);

			const Datum& dFloatConst = dFloat;
			Assert::IsTrue(dFloatConst.GetFloat(3) == 4.0f);

			auto lambda = [&dFloat]() { dFloat.GetFloat(45); };
			Assert::ExpectException<exception>(lambda);

			auto lambda2 = [&dFloatConst]() { dFloatConst.GetFloat(5); };
			Assert::ExpectException<exception>(lambda2);

			Datum dEmpty;
			const Datum& dEmptyConst = dEmpty;
			auto lambda3 = [&dEmpty]() { dEmpty.GetFloat(); };
			auto lambda4 = [&dEmptyConst]() { dEmptyConst.GetFloat(); };
			Assert::ExpectException<exception>(lambda3);
			Assert::ExpectException<exception>(lambda4);
		}

		TEST_METHOD(DatumGetVectorTest)
		{
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f), v2(2.0f, 2.0f, 2.0f, 2.0f);
			Datum dVector = { v1, v2, };
			Assert::IsTrue(dVector.GetVector(0) == v1);

			const Datum& dVectorConst = dVector;
			Assert::IsTrue(dVectorConst.GetVector(1) == v2);

			auto lambda = [&dVector]() { dVector.GetVector(45); };
			Assert::ExpectException<exception>(lambda);

			auto lambda2 = [&dVectorConst]() { dVectorConst.GetVector(5); };
			Assert::ExpectException<exception>(lambda2);

			Datum dEmpty;
			const Datum& dEmptyConst = dEmpty;
			auto lambda3 = [&dEmpty]() { dEmpty.GetVector(); };
			auto lambda4 = [&dEmptyConst]() { dEmptyConst.GetVector(); };
			Assert::ExpectException<exception>(lambda3);
			Assert::ExpectException<exception>(lambda4);
		}

		TEST_METHOD(DatumGetMatrixTest)
		{
			mat4 v1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			mat4 v2(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
			Datum dMatrix = { v1, v2, };
			Assert::IsTrue(dMatrix.GetMatrix(0) == v1);

			const Datum& dMatrixConst = dMatrix;
			Assert::IsTrue(dMatrixConst.GetMatrix(1) == v2);

			auto lambda = [&dMatrix]() { dMatrix.GetMatrix(45); };
			Assert::ExpectException<exception>(lambda);

			auto lambda2 = [&dMatrixConst]() { dMatrixConst.GetMatrix(5); };
			Assert::ExpectException<exception>(lambda2);

			Datum dEmpty;
			const Datum& dEmptyConst = dEmpty;
			auto lambda3 = [&dEmpty]() { dEmpty.GetMatrix(); };
			auto lambda4 = [&dEmptyConst]() { dEmptyConst.GetMatrix(); };
			Assert::ExpectException<exception>(lambda3);
			Assert::ExpectException<exception>(lambda4);
		}

		TEST_METHOD(DatumGetStringTest)
		{
			Datum dString = { "1.0f", "2.0f", "3.0f", "4.0f" };
			Assert::IsTrue(dString.GetString(0) == "1.0f");

			const Datum& dStringConst = dString;
			Assert::IsTrue(dStringConst.GetString(3) == "4.0f");

			auto lambda = [&dString]() { dString.GetString(45); };
			Assert::ExpectException<exception>(lambda);

			auto lambda2 = [&dStringConst]() { dStringConst.GetString(5); };
			Assert::ExpectException<exception>(lambda2);

			Datum dEmpty;
			const Datum& dEmptyConst = dEmpty;
			auto lambda3 = [&dEmpty]() { dEmpty.GetString(); };
			auto lambda4 = [&dEmptyConst]() { dEmptyConst.GetString(); };
			Assert::ExpectException<exception>(lambda3);
			Assert::ExpectException<exception>(lambda4);
		}

		TEST_METHOD(DatumGetPointerTest)
		{
			Foo f1(1), f2(2), f3(3), f4(4);

			Datum dPointer = { &f1, &f2, &f3, &f4 };
			Assert::IsTrue(dPointer.GetPointer(0) == &f1);

			const Datum& dPointerConst = dPointer;
			Assert::IsTrue(dPointerConst.GetPointer(3) == &f4);

			auto lambda = [&dPointer]() { dPointer.GetPointer(45); };
			Assert::ExpectException<exception>(lambda);

			auto lambda2 = [&dPointerConst]() { dPointerConst.GetPointer(5); };
			Assert::ExpectException<exception>(lambda2);

			Datum dEmpty;
			const Datum& dEmptyConst = dEmpty;
			auto lambda3 = [&dEmpty]() { dEmpty.GetPointer(); };
			auto lambda4 = [&dEmptyConst]() { dEmptyConst.GetPointer(); };
			Assert::ExpectException<exception>(lambda3);
			Assert::ExpectException<exception>(lambda4);
		}

		TEST_METHOD(DatumGetScopePointerTest)
		{
			Scope s1, s2;

			Datum dScopePointer = { &s1, &s2 };
			Assert::IsTrue(&dScopePointer.GetScope(0) == &s1);

			const Datum& dScopePointerConst = dScopePointer;
			Assert::IsTrue(&dScopePointerConst.GetScope(1) == &s2);

			auto lambda = [&dScopePointer]() { dScopePointer.GetScope(45); };
			Assert::ExpectException<exception>(lambda);

			auto lambda2 = [&dScopePointerConst]() { dScopePointerConst.GetScope(5); };
			Assert::ExpectException<exception>(lambda2);

			Datum dEmpty;
			const Datum& dEmptyConst = dEmpty;
			auto lambda3 = [&dEmpty]() { dEmpty.GetScope(); };
			auto lambda4 = [&dEmptyConst]() { dEmptyConst.GetScope(); };
			Assert::ExpectException<exception>(lambda3);
			Assert::ExpectException<exception>(lambda4);
		}

		TEST_METHOD(DatumSetFromStringTest)
		{
			//testing for ints
			Datum dInt = { 0 };
			Assert::IsTrue(dInt.SetFromString("234"));
			Assert::IsFalse(dInt.SetFromString("23", 1u));

			//testing for floats
			Datum dFloat = { 0.0f };
			Assert::IsTrue(dFloat.SetFromString("2.0f"));
			Assert::IsFalse(dFloat.SetFromString("2.0f", 4u));

			//testing for vectors
			vec4 v1 = { 1.0f, 1.0f, 1.0f, 1.0f };
			Datum dVector = { v1 };
			Assert::IsTrue(dVector.SetFromString("vec4(1.0, 2.1140, 5.0, -202.2)"));
			Assert::IsFalse(dVector.SetFromString("vec4(1.0, 2.1140, 5.0, -202.2)", 4u));

			//testing for matrices
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			Datum dMatrix = { m1 };
			Assert::IsTrue(dMatrix.SetFromString("mat4((5.0, 5.9, 5.85, 5.4), (5.0, 5.9, 5.85, 5.4), (5.0, 5.9, 5.85, 5.4), (5.0, 5.9, 5.85, 5.4))"));
			Assert::IsFalse(dMatrix.SetFromString("mat4((5.0, 5.9, 5.85, 5.4), (5.0, 5.9, 5.85, 5.4), (5.0, 5.9, 5.85, 5.4), (5.0, 5.9, 5.85, 5.4))", 78u));

			//testing for strings
			Datum dString = {"str"};
			Assert::IsTrue(dString.SetFromString("As"));
			Assert::IsFalse(dString.SetFromString("w4t", 4u));

			//testing for pointers
			Foo f(9);
			Datum dPointer = { &f };
			Assert::IsFalse(dPointer.SetFromString("N/A"));

			Datum dEmpty;
			Assert::IsFalse(dEmpty.SetFromString("swrh"));
		}

		TEST_METHOD(DatumToString)
		{
			//testing to get empty string
			Datum d;
			Assert::IsTrue("" == d.ToString());
			d.PushBack(5);
			Assert::IsTrue("" == d.ToString(45u));

			//testing with ints
			Datum dInt = { 0 };
			Assert::IsTrue("0" == dInt.ToString());

			//testing with floats
			Datum dFloat = { 0.0f };
			Assert::IsTrue("0.000000" == dFloat.ToString());

			//testing with vectors
			vec4 v1 = { 1.0f, 1.0f, 1.0f, 1.0f };
			Datum dVector = { v1 };
			dVector.ToString();

			//testing with matrices
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			Datum dMatrix = { m1 };
			dMatrix.ToString();

			//testing with strings
			Datum dString = { "w" };
			Assert::IsTrue("w" == dString.ToString());
		}

		TEST_METHOD(DatumEqualityOperatorTest)
		{
			//Datum-to-datum
			Datum dInt({ 2 }), dString({ "2.0f" }), dString2({"2.1f"});
			Assert::IsFalse(dInt == dString);
			Assert::IsFalse(dString2 == dString);
			Foo f1(1), f2(2);
			Datum dPointer({ &f1 }), dPointer2({ &f2 });
			Assert::IsFalse(dPointer == dPointer2);
			Scope s1, s2;
			s1.Append("name");
			Datum dScopePointer1({ &s1 }), dScopePointer2({ &s2 });
			Assert::IsFalse(dScopePointer1 == dScopePointer2);

			//Datum-to-int
			Datum dEmpty;
			Assert::IsFalse(dEmpty == 3);
			Assert::IsTrue(dInt == 2);

			//Datum-to-float
			Assert::IsFalse(dEmpty == 4.4f);
			Datum dFloat({ 4.4f });
			Assert::IsTrue(dFloat == 4.4f);

			//Datum-to-vector
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f);
			Assert::IsFalse(dEmpty == v1);
			Datum dVector({ v1 });
			Assert::IsTrue(dVector == v1);

			//Datum-to-matrix
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			Assert::IsFalse(dEmpty == m1);
			Datum dMatrix({ m1 });
			Assert::IsTrue(dMatrix == m1);

			//Datum-to-string
			Assert::IsFalse(dEmpty == "blah");
			dString ={"blah"};
			Assert::IsTrue(dString == "blah");

			//Datum-to-pointer
			Assert::IsFalse(dEmpty == &f1);
			Assert::IsTrue(dPointer == &f1);

			//Datum-to-table (Scope)
			Assert::IsFalse(dEmpty == &s2);
			Datum dScopePointer({&s1});
			Assert::IsTrue(dScopePointer == s1);
		}

		TEST_METHOD(DatumInequalityOperatorTest)
		{
			//Datum-to-datum
			Datum dInt({ 2 }), dString({ "2.0f" }), dString2({ "2.1f" });
			Assert::IsTrue(dInt != dString);
			Assert::IsTrue(dString2 != dString);
			Foo f1(1), f2(2);
			Datum dPointer({ &f1 }), dPointer2({ &f2 });
			Assert::IsTrue(dPointer != dPointer2);
			Scope s1, s2;
			s1.Append("name");
			Datum dScopePointer1({ &s1 }), dScopePointer2({ &s2 });
			Assert::IsTrue(dScopePointer1 != dScopePointer2);

			//Datum-to-int
			Datum dEmpty;
			Assert::IsTrue(dEmpty != 3);
			Assert::IsFalse(dInt != 2);

			//Datum-to-float
			Assert::IsTrue(dEmpty != 4.4f);
			Datum dFloat({ 4.4f });
			Assert::IsFalse(dFloat != 4.4f);

			//Datum-to-vector
			vec4 v1(1.0f, 1.0f, 1.0f, 1.0f);
			Assert::IsTrue(dEmpty != v1);
			Datum dVector({ v1 });
			Assert::IsFalse(dVector != v1);

			//Datum-to-matrix
			mat4 m1(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
			Assert::IsTrue(dEmpty != m1);
			Datum dMatrix({ m1 });
			Assert::IsFalse(dMatrix != m1);

			//Datum-to-string
			Assert::IsTrue(dEmpty != "blah");
			dString = { "blah" };
			Assert::IsFalse(dString != "blah");

			//Datum-to-pointer
			Assert::IsTrue(dEmpty != &f1);
			Assert::IsFalse(dPointer != &f1);

			//Datum-to-table (Scope)
			Assert::IsTrue(dEmpty != &s2);
			Datum dScopePointer({ &s1 });
			Assert::IsFalse(dScopePointer != s1);
		}

		TEST_METHOD(DatumIsInternalTest)
		{
			Datum d1, d2;
			d1.Set(3);
			Assert::IsTrue(d1.IsInternal());
			string s = "yep";
			d2.SetStorage(&s, 1u);
			Assert::IsFalse(d2.IsInternal());
		}
		
	private:
		_CrtMemState initialMemoryState;
	};
}