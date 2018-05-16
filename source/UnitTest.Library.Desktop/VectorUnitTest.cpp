#include "pch.h"

#include "Vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;

namespace UnitTest
{
	TEST_CLASS(VectorUnitTest)
	{
	public:
		TEST_METHOD_INITIALIZE(VectorTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(VectorTestCleanup)
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

		TEST_METHOD(VIteratorDefaultConstructorTest)
		{
			Vector<Foo>::Iterator it;
			auto lambda1 = [&it]() { *it; };
			Assert::ExpectException<std::exception>(lambda1);
		}

		TEST_METHOD(VIteratorDestructorTest)
		{
			Vector<Foo> fooVector(1u);
			fooVector.PushBack(Foo(1));
			auto it = fooVector.begin();
			//Only requirement here is that memory should not leak.
		}

		TEST_METHOD(VIteratorCopyConstructorTest)
		{
			Vector<Foo> fooVector;
			fooVector.PushBack(Foo(1));
			fooVector.PushBack(Foo(2));
			auto it(fooVector.begin());
			Assert::IsTrue(*it == Foo(1));
			Assert::IsTrue(*(++it) == Foo(2));
		}

		TEST_METHOD(VIteratorMoveConstructorTest)
		{
			Vector<Foo> fooVector;
			fooVector.PushBack(Foo(1));
			fooVector.PushBack(Foo(2));
			auto it = std::move(fooVector.begin());
			Assert::IsTrue(*it == Foo(1));
			Assert::IsTrue(*(++it) == Foo(2));
		}

		TEST_METHOD(VIteratorCopyAssignmentOperatorTest)
		{
			Vector<Foo> fooVector;
			fooVector.PushBack(Foo(1));
			fooVector.PushBack(Foo(2));
			Vector<Foo>::Iterator it;
			it = fooVector.begin();
			Assert::IsTrue(*it == Foo(1));
			Assert::IsTrue(*(++it) == Foo(2));
		}

		TEST_METHOD(VIteratorMoveAssignmentOperatorTest)
		{
			Vector<Foo> fooVector;
			fooVector.PushBack(Foo(1));
			fooVector.PushBack(Foo(2));
			Vector<Foo>::Iterator it;
			it = std::move(fooVector.begin());
			Assert::IsTrue(*it == Foo(1));
			Assert::IsTrue(*(++it) == Foo(2));
		}

		TEST_METHOD(VIteratorEqualityOperatorTest)
		{
			Vector<Foo>::Iterator it1, it2;
			Assert::IsTrue(it1 == it2);
			Vector<Foo> fooVector;
			it1 = fooVector.begin();
			it2 = fooVector.end();
			Assert::IsTrue(it1 == it2);
		}

		TEST_METHOD(VIteratorInequalityOperatorTest)
		{
			Vector<Foo> fooVector1, fooVector2;
			Assert::IsTrue(fooVector1.begin() != fooVector2.begin());
			fooVector1.PushBack(Foo(1));
			fooVector1.PushBack(Foo(2));
			auto it = fooVector1.begin();
			Assert::IsTrue(++it != fooVector1.begin());
		}

		TEST_METHOD(VIteratorPrefixIncrementOperatorTest)
		{
			Vector<Foo>::Iterator it;
			auto lambda1 = [&it]() { ++it; };
			Assert::ExpectException<std::exception>(lambda1);
			Vector<Foo> vector;
			vector.PushBack(Foo(22));
			it = vector.begin();
			Assert::IsTrue(++it == vector.end());
			Assert::ExpectException<std::exception>(lambda1);
		}

		TEST_METHOD(VIteratorPostfixIncrementOperatorTest)
		{
			Vector<Foo>::Iterator it;
			auto lambda1 = [&it]() { it++; };
			Assert::ExpectException<std::exception>(lambda1);
			Vector<Foo> vector;
			vector.PushBack(Foo(22));
			it = vector.begin();
			it++;
			Assert::IsTrue(it == vector.end());
			Assert::ExpectException<std::exception>(lambda1);
		}

		TEST_METHOD(VIteratorDereferenceOperatorTest)
		{
			Vector<Foo>::Iterator it;
			auto lambda1 = [&it]() { *it; };
			Assert::ExpectException<std::exception>(lambda1);
			Vector<Foo> vector;
			it = vector.begin();
			Assert::ExpectException<std::exception>(lambda1);
			vector.PushBack(Foo(234));
			it = vector.begin();
			Assert::AreEqual(234, (*it).GetValue());
		}

		TEST_METHOD(VectorConstructorTest)
		{
			Vector<Foo> vector1;
			Vector<Foo> vector2(1001u);
			Assert::AreEqual(1001u, vector2.Capacity());
			Assert::AreEqual(8u, vector1.Capacity());
		}

		TEST_METHOD(VectorCopyConstructorTest)
		{
			Vector<Foo> vector;
			uint32_t i;
			for (i = 5u; i <= 10u; ++i)
			{
				vector.PushBack(Foo(i));
			}

			Vector<Foo> copy(vector);
			i = 5u;
			for (Foo f : copy)
			{
				Assert::AreEqual(i++, static_cast<uint32_t>(f.GetValue()));
			}

		}

		TEST_METHOD(VectorMoveConstructorTest)
		{
			Vector<Foo> vector;
			uint32_t i;
			for (i = 5u; i <= 10u; ++i)
			{
				vector.PushBack(Foo(i));
			}

			Vector<Foo> moveTarget = std::move(vector);
			i = 5u;
			for (Foo f : moveTarget)
			{
				Assert::AreEqual(i++, static_cast<uint32_t>(f.GetValue()));
			}

			auto lambda1 = [&vector]() { Foo whatever = vector.At(0); };
			Assert::ExpectException<std::exception>(lambda1);
		}

		TEST_METHOD(VectorCopyAssignmentOperatorTest)
		{
			Vector<Foo>* vector = new Vector<Foo>();
			uint32_t i;
			for (i = 5u; i <= 10u; ++i)
			{
				vector->PushBack(Foo(i));
			}

			Vector<Foo> *copy = new Vector<Foo>();
			*copy = *vector;
			i = 5u;
			for (Foo f : *copy)
			{
				Assert::AreEqual(i++, static_cast<uint32_t>(f.GetValue()));
			}

			delete vector;
			delete copy;
		}

		TEST_METHOD(VectorMoveAssignmentOperatorTest)
		{
			Vector<Foo> vector;
			uint32_t i;
			for (i = 5u; i <= 10u; ++i)
			{
				vector.PushBack(Foo(i));
			}

			Vector<Foo> moveTarget;
			moveTarget = std::move(vector);
			i = 5u;
			for (Foo f : moveTarget)
			{
				Assert::AreEqual(i++, static_cast<uint32_t>(f.GetValue()));
			}

			auto lambda1 = [&vector]() { Foo whatever = vector.At(0); };
			Assert::ExpectException<std::exception>(lambda1);
		}

		TEST_METHOD(VectorBracketsOperatorTest)
		{
			//testing non-const version
			Vector<Foo> vector;
			for (uint32_t i = 5u; i <= 10u; ++i)
			{
				vector.PushBack(Foo(i));
			}
			for (uint32_t i = 0u; i < vector.Size(); ++i)
			{
				Assert::AreEqual(static_cast<int32_t>(i + 5u), vector[i].GetValue());
			}
			auto lambda = [&vector]() { vector[vector.Size()]; };
			Assert::ExpectException<std::exception>(lambda);

			//testing const version
			const Vector<Foo> vector2 = vector;
			for (uint32_t i = 0u; i < vector2.Size(); ++i)
			{
				Assert::AreEqual(static_cast<int32_t>(i + 5u), vector2[i].GetValue());
			}
			auto lambda2 = [&vector2]() { vector2[vector2.Size()]; };
			Assert::ExpectException<std::exception>(lambda2);
		}

		TEST_METHOD(VectorPopBackTest)
		{
			Vector<Foo> vector;
			auto lambda = [&vector]() { vector.PopBack(); };
			Assert::ExpectException<std::exception>(lambda);

			for (uint32_t i = 5u; i <= 10u; ++i)
			{
				vector.PushBack(Foo(i));
			}
			vector.PopBack();
			Assert::AreEqual(5u, vector.Size());
			auto lambda2 = [&vector]() { vector[5u]; };
			Assert::ExpectException<std::exception>(lambda2);
		}

		TEST_METHOD(VectorIsEmptyTest)
		{
			Vector<Foo> vector;
			Assert::IsTrue(vector.IsEmpty());
			vector.PushBack(Foo(0));
			Assert::IsFalse(vector.IsEmpty());
		}

		TEST_METHOD(VectorFrontTest)
		{
			//testing non-const version
			Vector<Foo> vector;
			auto lambda1 = [&vector]() { vector.Front(); };
			Assert::ExpectException<std::exception>(lambda1);

			vector.PushBack(Foo(234));
			Assert::AreEqual(234, vector.Front().GetValue());

			Foo foo12(12);
			vector.Front() = foo12;
			/*Attention! When the above two lines were instead replaced with
			the single line "vector.Front() = Foo(12);", it caused an asserion
			failure in the following line. Can you help me figure out why?*/
			Assert::AreEqual(12, vector[0u].GetValue());

			//testing const version
			const Vector<Foo> constVector = vector;
			Assert::AreEqual(12, constVector.Front().GetValue());
			const Vector<Foo> emptyConstVector;
			auto lambda2 = [&emptyConstVector]() { emptyConstVector.Front(); };
			Assert::ExpectException<std::exception>(lambda2);
		}

		TEST_METHOD(VectorBackTest)
		{
			//testing non-const version
			Vector<Foo> vector;
			auto lambda1 = [&vector]() { vector.Back(); };
			Assert::ExpectException<std::exception>(lambda1);

			vector.PushBack(Foo(234));
			Assert::AreEqual(234, vector.Back().GetValue());
			vector.PushBack(Foo(2000));
			Assert::AreEqual(2000, vector.Back().GetValue());

			//testing const version
			const Vector<Foo> emptyVector;
			auto lambda2 = [&emptyVector]() { emptyVector.Back(); };
			Assert::ExpectException<std::exception>(lambda2);

			const Vector<Foo> constVector = vector;
			Assert::AreEqual(2000, constVector.Back().GetValue());
		}

		TEST_METHOD(VectorSizeTest)
		{
			Vector<Foo> vector(10u);
			Assert::AreEqual(0u, vector.Size());
			for (int i = 0; i < 5; ++i)
			{
				vector.PushBack(Foo(8));
			}
			Assert::AreEqual(5u, vector.Size());
		}

		TEST_METHOD(VectorCapacityTest)
		{
			Vector<Foo> vector;
			Assert::AreEqual(8u, vector.Capacity());
			vector.Reserve(1u);
			Assert::AreEqual(1u, vector.Capacity());
		}
		
		TEST_METHOD(VectorAtTest)
		{
			//testing non-const version
			Vector<Foo> vector;
			for (uint32_t i = 5u; i <= 10u; ++i)
			{
				vector.PushBack(Foo(i));
			}
			for (uint32_t i = 0u; i < vector.Size(); ++i)
			{
				Assert::AreEqual(static_cast<int32_t>(i + 5u), vector.At(i).GetValue());
			}
			auto lambda = [&vector]() { vector.At(vector.Size()); };
			Assert::ExpectException<std::exception>(lambda);

			//testing const version
			const Vector<Foo> vector2 = vector;
			for (uint32_t i = 0u; i < vector2.Size(); ++i)
			{
				Assert::AreEqual(static_cast<int32_t>(i + 5u), vector2.At(i).GetValue());
			}
			auto lambda2 = [&vector2]() { vector2.At(vector2.Size()); };
			Assert::ExpectException<std::exception>(lambda2);
		}

		TEST_METHOD(VectorBeginEndTest)
		{
			Vector<Foo> v;
			Assert::IsTrue(v.end() == v.begin());
			for (int i = 0; i < 8; ++i)
			{
				v.PushBack(Foo(i));
			}
			uint32_t counter = 0;
			for (auto it = v.begin(); it != v.end(); ++it)
			{
				++counter;
			}
			Assert::AreEqual(8u, counter);
		}

		TEST_METHOD(VectorPushBackTest)
		{
			Vector<Foo> v(1u);
			v.PushBack(Foo(55));
			Assert::AreEqual(55, v.At(0).GetValue());
			Assert::AreEqual(1u, v.Capacity());
			v.PushBack(Foo(66));
			Assert::AreEqual(66, v[1].GetValue());
			Assert::IsTrue(v.Capacity() > 1u);
		}

		TEST_METHOD(VectorReserveTest)
		{
			Vector<Foo> vector1;
			vector1.PushBack(Foo(1));
			vector1.PushBack(Foo(11));
			Assert::IsTrue(vector1.Reserve(111u));
			Assert::AreEqual(111u, vector1.Capacity());
			Assert::IsTrue(vector1.Reserve(100u));
			Assert::AreEqual(100u, vector1.Capacity());
			Assert::IsFalse(vector1.Reserve(1u));
			Assert::AreEqual(2u, vector1.Capacity());
		}

		TEST_METHOD(VectorFindTest)
		{
			Vector<Foo> vector;
			Assert::IsTrue(vector.end() == vector.Find(Foo(2)));
			vector.PushBack(Foo(2));
			vector.PushBack(Foo(3));
			Assert::IsTrue(vector.end() == vector.Find(Foo(1)));
			auto it = vector.begin();
			Assert::IsTrue(++it == vector.Find(Foo(3)));
		}

		TEST_METHOD(VectorClearTest)
		{
			Vector<Foo> vector;
			vector.Clear();
			Assert::AreEqual(0u, vector.Size());
			vector.PushBack(Foo(0));
			vector.PushBack(Foo(-1));
			vector.Clear();
			Assert::AreEqual(0u, vector.Size());
		}

		TEST_METHOD(VectorRemoveTest)
		{
			Vector<Foo> vector;
			for (int i = 0; i < 5; ++i)
			{
				vector.PushBack(Foo(i));
			}
			Assert::IsTrue(vector.Remove(Foo(0)));
			for (uint32_t i = 0u; i < vector.Size(); ++i)
			{
				Assert::AreEqual(static_cast<int>(i + 1u),
					vector[i].GetValue());
			}
			Assert::IsTrue(vector.Remove(Foo(4)));
			Assert::AreEqual(3u, vector.Size());
			Assert::IsFalse(vector.Remove(Foo(-373776)));
		}

	private:
		_CrtMemState initialMemoryState;
	};
}