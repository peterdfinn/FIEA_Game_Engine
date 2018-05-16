#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(FactoryUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
	public:
		TEST_METHOD_INITIALIZE(FactoryUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(FactoryUnitTestCleanup)
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

		TEST_METHOD(FactoryIteratorDefaultConstructorTest)
		{
			/*I expect this to construct an iterator that contains a HashMap
			iterator that has been default-constructed, so I expect that
			incrementing it or dereferencing it will cause an exception to be
			thrown.*/
			Factory<AbstractBase>::Iterator it;
			auto lambda1 = [&it]() { ++it; };
			auto lambda2 = [&it]() { *it; };
			Assert::ExpectException<exception>(lambda1);
			Assert::ExpectException<exception>(lambda2);
		}

		TEST_METHOD(FactoryIteratorDestructorTest)
		{
			/*I expect that an Iterator not leak memory, whether it is the begin
			iterator, the end iterator, something in-between, or an
			uninitialized iterator.*/
			Factory<AbstractBase>::Iterator uninitialized;
			ConcreteDerivedAFactory concreteFactoryA;
			ConcreteDerivedBFactory concreteFactoryB;
			Factory<AbstractBase>::Add(concreteFactoryA);
			Factory<AbstractBase>::Add(concreteFactoryB);
			Factory<AbstractBase>::Iterator begin =
				Factory<AbstractBase>::begin();
			Factory<AbstractBase>::Iterator end = Factory<AbstractBase>::end();
			Factory<AbstractBase>::Iterator middle = begin;
			++middle;

			Factory<AbstractBase>::Remove(concreteFactoryA);
			Factory<AbstractBase>::Remove(concreteFactoryB);
		}

		TEST_METHOD(FactoryIteratorCopyConstructorTest)
		{
			/*I expect that a copy constructor should produce an iterator that
			behaves exactly like the original in every way.*/
			Factory<AbstractBase>::Iterator uninitialized;
			Factory<AbstractBase>::Iterator uninitializedCopy = uninitialized;
			Assert::IsTrue(uninitialized == uninitializedCopy);
			
			ConcreteDerivedAFactory factoryA;
			ConcreteDerivedBFactory factoryB;
			Factory<AbstractBase>::Add(factoryA);
			Factory<AbstractBase>::Add(factoryB);
			Factory<AbstractBase>::Iterator begin =
				Factory<AbstractBase>::begin();
			Factory<AbstractBase>::Iterator beginCopy = begin;
			Assert::IsTrue(begin == beginCopy);
			while (begin != Factory<AbstractBase>::end())
			{
				Assert::IsTrue(++begin == ++beginCopy);
			}

			Factory<AbstractBase>::Remove(factoryA);
			Factory<AbstractBase>::Remove(factoryB);
		}

		TEST_METHOD(FactoryIteratorMoveConstructorTest)
		{
			/*I expect that the move constructor will create an iterator that
			behaves exactly as the original would have if it hadn't been moved;
			the original should now be "nulled out".*/
			ConcreteDerivedAFactory factoryA;
			ConcreteDerivedBFactory factoryB;
			Factory<AbstractBase>::Add(factoryA);
			Factory<AbstractBase>::Add(factoryB);

			Factory<AbstractBase>::Iterator original =
				Factory<AbstractBase>::begin();
			Factory<AbstractBase>::Iterator copy(move(original));

			auto lambda1 = [&original]() { ++original; };
			auto lambda2 = [&original]() { *original; };
			Assert::ExpectException<exception>(lambda1);
			Assert::ExpectException<exception>(lambda2);
			
			Factory<AbstractBase>* pointerToFirst = &(*copy);
			Assert::IsTrue(pointerToFirst == &factoryA || pointerToFirst == &factoryB);
			bool aWasFirst = false;
			if (pointerToFirst == &factoryA)
			{
				aWasFirst = true;
			}
			Factory<AbstractBase>* pointerToSecond = &(*(++copy));
			Assert::IsTrue(aWasFirst ? pointerToSecond == &factoryB :
				pointerToSecond == &factoryA);
			Assert::IsTrue(++copy == Factory<AbstractBase>::end());

			Factory<AbstractBase>::Remove(factoryB);
			Factory<AbstractBase>::Remove(factoryA);
		}

		TEST_METHOD(FactoryIteratorCopyAssignmentOperatorTest)
		{
			/*I expect that the copy assignment operator should produce an
			iterator that behaves exactly like the original in every way.*/
			ConcreteDerivedAFactory factoryA;
			ConcreteDerivedBFactory factoryB;
			Factory<AbstractBase>::Add(factoryA);
			Factory<AbstractBase>::Add(factoryB);

			Factory<AbstractBase>::Iterator begin1 =
				Factory<AbstractBase>::begin();
			Factory<AbstractBase>::Iterator uninitialized1;
			uninitialized1 = begin1;
			Assert::IsTrue(begin1 == uninitialized1);
			while (begin1 != Factory<AbstractBase>::end())
			{
				Assert::IsTrue(++begin1 == ++uninitialized1);
			}

			Factory<AbstractBase>::Iterator begin2 =
				Factory<AbstractBase>::begin();
			Factory<AbstractBase>::Iterator uninitialized2;
			begin2 = uninitialized2;
			auto lambda1 = [&uninitialized2]() { ++uninitialized2; };
			auto lambda2 = [&uninitialized2]() { *uninitialized2; };
			Assert::ExpectException<exception>(lambda1);
			Assert::ExpectException<exception>(lambda2);

			Factory<AbstractBase>::Remove(factoryA);
			Factory<AbstractBase>::Remove(factoryB);
		}

		TEST_METHOD(FactoryIteratorMoveAssignmentOperatorTest)
		{
			/*I expect that the move assignment operator should produce an
			iterator that behaves exactly like the original in every way. I also
			expect that the right-hand side of the assignment should have been
			"nulled out".*/
			ConcreteDerivedAFactory factoryA;
			ConcreteDerivedBFactory factoryB;
			Factory<AbstractBase>::Add(factoryA);
			Factory<AbstractBase>::Add(factoryB);

			Factory<AbstractBase>::Iterator begin1 =
				Factory<AbstractBase>::begin();
			Factory<AbstractBase>::Iterator uninitialized1;
			uninitialized1 = move(begin1);

			auto lambda1 = [&begin1]() { ++begin1; };
			auto lambda2 = [&begin1]() { *begin1; };
			Assert::ExpectException<exception>(lambda1);
			Assert::ExpectException<exception>(lambda2);

			Factory<AbstractBase>* pointerToFirst = &(*uninitialized1);
			Assert::IsTrue(pointerToFirst == &factoryA || pointerToFirst == &factoryB);
			bool aWasFirst = false;
			if (pointerToFirst == &factoryA)
			{
				aWasFirst = true;
			}
			Factory<AbstractBase>* pointerToSecond = &(*(++uninitialized1));
			Assert::IsTrue(aWasFirst ? pointerToSecond == &factoryB :
				pointerToSecond == &factoryA);
			Assert::IsTrue(++uninitialized1 == Factory<AbstractBase>::end());

			Factory<AbstractBase>::Iterator uninitialized2;
			Factory<AbstractBase>::Iterator begin2 =
				Factory<AbstractBase>::begin();

			begin2 = uninitialized2;

			auto lambda3 = [&begin2]() { ++begin2; };
			auto lambda4 = [&begin2]() { *begin2; };
			Assert::ExpectException<exception>(lambda3);
			Assert::ExpectException<exception>(lambda4);
			auto lambda5 = [&uninitialized2]() { ++uninitialized2; };
			auto lambda6 = [&uninitialized2]() { *uninitialized2; };
			Assert::ExpectException<exception>(lambda5);
			Assert::ExpectException<exception>(lambda6);

			Factory<AbstractBase>::Remove(factoryB);
			Factory<AbstractBase>::Remove(factoryA);
		}

		TEST_METHOD(FactoryIteratorPrefixIncrementOperatorTest)
		{
			Factory<AbstractBase>::Iterator uninitialized;
			auto lambda = [&uninitialized]() { ++uninitialized; };
			Assert::ExpectException<exception>(lambda);

			ConcreteDerivedAFactory factoryA;
			Factory<AbstractBase>::Add(factoryA);
			Factory<AbstractBase>::Iterator begin =
				Factory<AbstractBase>::begin();
			Assert::IsTrue(++begin == Factory<AbstractBase>::end());
			auto lambda2 = [&begin]() { ++begin; };
			Assert::ExpectException<exception>(lambda2);

			Factory<AbstractBase>::Remove(factoryA);
		}

		TEST_METHOD(FactoryIteratorPostfixIncrementOperatorTest)
		{
			Factory<AbstractBase>::Iterator uninitialized;
			auto lambda = [&uninitialized]() { uninitialized++; };
			Assert::ExpectException<exception>(lambda);

			ConcreteDerivedAFactory factoryA;
			Factory<AbstractBase>::Add(factoryA);
			Factory<AbstractBase>::Iterator begin =
				Factory<AbstractBase>::begin();
			Assert::IsTrue(begin++ == Factory<AbstractBase>::begin());
			auto lambda2 = [&begin]() { begin++; };
			Assert::ExpectException<exception>(lambda2);

			Factory<AbstractBase>::Remove(factoryA);
		}

		TEST_METHOD(FactoryIteratorDereferenceOperatorTest)
		{
			//testing const version
			ConcreteDerivedAFactory factoryA;
			Factory<AbstractBase>::Add(factoryA);
			const Factory<AbstractBase>::Iterator constBegin =
				Factory<AbstractBase>::begin();
			const Factory<AbstractBase>::Iterator constEnd =
				Factory<AbstractBase>::end();
			Assert::IsTrue(&factoryA == &(*constBegin));
			auto lambda1 = [&constEnd]() { *constEnd; };
			Assert::ExpectException<exception>(lambda1);

			//testing non-const version
			Assert::IsTrue(&factoryA == &(*Factory<AbstractBase>::begin()));
			auto lambda2 = []() { *Factory<AbstractBase>::end(); };
			Assert::ExpectException<exception>(lambda2);

			Factory<AbstractBase>::Remove(factoryA);
		}

		TEST_METHOD(FactoryIteratorEqualityOperatorTest)
		{
			const Factory<AbstractBase>::Iterator uninitialized1,
				uninitialized2;
			Assert::IsTrue(uninitialized2 == uninitialized1);
			ConcreteDerivedAFactory factoryA;
			Factory<AbstractBase>::Add(factoryA);
			const Factory<AbstractBase>::Iterator
				begin1 = Factory<AbstractBase>::begin(),
				end1 = Factory<AbstractBase>::end(),
				begin2 = Factory<AbstractBase>::begin(),
				end2 = Factory<AbstractBase>::end();
			Assert::IsTrue(begin1 == begin2);
			Assert::IsFalse(begin1 == end1);
			Assert::IsTrue(end2 == end1);

			Factory<AbstractBase>::Remove(factoryA);
		}

		TEST_METHOD(FactoryIteratorInequalityOperatorTest)
		{
			const Factory<AbstractBase>::Iterator uninitialized1,
				uninitialized2;
			Assert::IsFalse(uninitialized2 != uninitialized1);
			ConcreteDerivedAFactory factoryA;
			Factory<AbstractBase>::Add(factoryA);
			const Factory<AbstractBase>::Iterator
				begin1 = Factory<AbstractBase>::begin(),
				end1 = Factory<AbstractBase>::end(),
				begin2 = Factory<AbstractBase>::begin(),
				end2 = Factory<AbstractBase>::end();
			Assert::IsFalse(begin1 != begin2);
			Assert::IsTrue(begin1 != end1);
			Assert::IsFalse(end2 != end1);

			Factory<AbstractBase>::Remove(factoryA);
		}

		TEST_METHOD(FactoryAddTest)
		{
			/*I expect that, once a concrete factory pointer has been added to
			the static HashMap, the number of entries has increased by 1, but
			only if the pointer inserted was not already in the HashMap. Also, I
			expect that I should now be able to access the concrete factory via
			a call to Find - Find should not return nullptr.*/
			
			ConcreteDerivedAFactory factoryA;

			Assert::IsTrue(Factory<AbstractBase>::Add(factoryA));
			uint32_t numberOfConcreteFactories = 0u;
			for (auto it = Factory<AbstractBase>::begin();
				it != Factory<AbstractBase>::end(); ++it)
			{
				++numberOfConcreteFactories;
			}
			Assert::AreEqual(1u, numberOfConcreteFactories);

			Assert::IsFalse(Factory<AbstractBase>::Add(factoryA));
			numberOfConcreteFactories = 0u;
			for (auto it = Factory<AbstractBase>::begin();
				it != Factory<AbstractBase>::end(); ++it)
			{
				++numberOfConcreteFactories;
			}
			Assert::AreEqual(1u, numberOfConcreteFactories);

			Assert::IsNotNull(Factory<AbstractBase>::Find("ConcreteDerivedA"));

			Factory<AbstractBase>::Remove(factoryA);
		}

		TEST_METHOD(FactoryRemoveTest)
		{
			/*I expect that Remove returns true if the sought-after concrete
			factory exists in the static HashMap, and false otherwise. I expect
			that static HashMap now no longer contains a concrete factory with
			the removed factory's class name.*/
			ConcreteDerivedAFactory factoryA;
			Assert::IsFalse(Factory<AbstractBase>::Remove(factoryA));
			Factory<AbstractBase>::Add(factoryA);
			Assert::IsTrue(Factory<AbstractBase>::Remove(factoryA));
			Assert::IsNull(Factory<AbstractBase>::Find(factoryA.ClassName()));
		}

		TEST_METHOD(FactoryFindTest)
		{
			/*I expect that Find return nullptr when given a class name that
			doesn't exist in the static HashMap, and that it return a pointer to
			the appropriate concrete factory when given a valid class name.*/
			ConcreteDerivedAFactory factoryA;
			ConcreteDerivedBFactory factoryB;
			Factory<AbstractBase>::Add(factoryB);
			Factory<AbstractBase>::Add(factoryA);

			Assert::IsNull(Factory<AbstractBase>::Find("a"));
			Assert::IsTrue(Factory<AbstractBase>::Find(factoryA.ClassName()) ==
				&factoryA);
			Assert::IsTrue(Factory<AbstractBase>::Find(factoryB.ClassName()) ==
				&factoryB);

			Factory<AbstractBase>::Remove(factoryA);
			Factory<AbstractBase>::Remove(factoryB);
		}

		TEST_METHOD(FactoryCreateTest)
		{
			/*I expect that Factory::Create should return a pointer to a
			dynamically allocated concrete factory of the class name passed in,
			if a concrete factory of that name exists in the static HashMap.
			Otherwise, I expect Create to return nullptr.*/
			Assert::IsNull(Factory<AbstractBase>::Create("ConcreteDerivedA"));

			ConcreteDerivedAFactory factoryA;
			ConcreteDerivedBFactory factoryB;
			Factory<AbstractBase>::Add(factoryA);
			Factory<AbstractBase>::Add(factoryB);

			AbstractBase
				*ptrToNewA = Factory<AbstractBase>::Create("ConcreteDerivedA"),
				*ptrToNewB = Factory<AbstractBase>::Create("ConcreteDerivedB");

			Assert::IsTrue(ptrToNewA->Is(ConcreteDerivedA::TypeIdClass()));
			Assert::IsTrue(ptrToNewB->Is(ConcreteDerivedB::TypeIdClass()));

			delete ptrToNewA;
			delete ptrToNewB;
			Factory<AbstractBase>::Remove(factoryA);
			Factory<AbstractBase>::Remove(factoryB);
		}
	};
}