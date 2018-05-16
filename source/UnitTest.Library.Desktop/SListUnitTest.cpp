#include "pch.h"

#include "SList.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;

namespace UnitTest
{		
	TEST_CLASS(SListUnitTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Init)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
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

		TEST_METHOD(DefaultConstructorTest)
		{
			// testing for primitive type, making sure list is indeed empty
			SList<int> emptyList;
			auto exceptionTryFront = [&emptyList]() { emptyList.Front(); };
			auto exceptionTryBack = [&emptyList]() { emptyList.Back(); };
			Assert::ExpectException<std::exception>(exceptionTryFront);
			Assert::ExpectException<std::exception> (exceptionTryBack);

			// testing for pointers
			SList<int*> emptyPtrList;
			auto exceptionTryPtrFront = [&emptyPtrList]() { emptyPtrList.Front(); };
			auto exceptionTryPtrBack = [&emptyPtrList]() { emptyPtrList.Back(); };
			Assert::ExpectException<std::exception>(exceptionTryPtrFront);
			Assert::ExpectException<std::exception>(exceptionTryPtrBack);

			// testing for user-defined type
			SList<Foo> emptyFooList;
			auto exceptionTryFooFront = [&emptyFooList]() { emptyFooList.Front(); };
			auto exceptionTryFooBack = [&emptyFooList]() { emptyFooList.Back(); };
			Assert::ExpectException<std::exception>(exceptionTryFooFront);
			Assert::ExpectException<std::exception>(exceptionTryFooBack);
		}

		TEST_METHOD(CopyConstructorTest)
		{
			// testing for primitive type
			SList<double> nonEmptyList;
			nonEmptyList.PushBack(1.0);
			SList<double> shouldNotBeEmpty(nonEmptyList);
			Assert::AreEqual(1u, shouldNotBeEmpty.Size());
			Assert::AreEqual(1.0, shouldNotBeEmpty.Front());

			// testing for pointer
			SList<double*> nonEmptyPtrList;
			double local = 2.0;
			double* localPtr = &local;
			nonEmptyPtrList.PushBack(localPtr);
			SList<double*> shouldNotBeEmptyPtr(nonEmptyPtrList);
			Assert::AreEqual(1u, shouldNotBeEmptyPtr.Size());
			Assert::AreEqual(localPtr, shouldNotBeEmptyPtr.Front());
			
			// testing for user-derined type
			SList<Foo> nonEmptyFooList;
			Foo localFoo(3);
			nonEmptyFooList.PushBack(localFoo);
			SList<Foo> shouldNotBeEmptyFoo(nonEmptyFooList);
			Assert::AreEqual(1u, shouldNotBeEmptyFoo.Size());
			Assert::AreEqual(3, shouldNotBeEmptyFoo.Front().GetValue());
		}

		TEST_METHOD(DestructorTest)
		{
			// testing for primitive types
			SList<int>* const list = new SList<int>;
			list->PushBack(1);
			list->PushBack(2);
			delete list;

			// testing for pointers
			SList<int*>* const ptrList = new SList<int*>;
			ptrList->PushFront(nullptr);
			delete ptrList;

			// testing for user-defined type
			SList<Foo>* const fooList = new SList<Foo>;
			Foo localFoo(3);
			fooList->PushBack(localFoo);
			delete fooList;
		}

		TEST_METHOD(PushFrontTest)
		{
			// Testing on empty list with primitive types
			SList<double> list;
			list.PushFront(-100.001);
			Assert::AreEqual(-100.001, list.Front());
			Assert::AreEqual(-100.001, list.Back());

			//Testing on non-empty list with primitive types
			list.PushFront(-2000.0);
			Assert::AreEqual(-2000.0, list.Front());
			list.PushFront(3.03);
			Assert::AreEqual(3.03, list.Front());

			//testing on empty list with pointers
			SList<char*> ptrList;
			char localChar6 = '6';
			char* localCharPtr6 = &localChar6;
			ptrList.PushFront(localCharPtr6);
			Assert::AreEqual(localCharPtr6, ptrList.Front());

			//testing on non-empty list with pointers
			char localChar7 = '7';
			char localChar8 = '8';
			char* localCharPtr7 = &localChar7;
			char* localCharPtr8 = &localChar8;
			ptrList.PushFront(localCharPtr7);
			Assert::AreEqual(localCharPtr7, ptrList.Front());
			ptrList.PushFront(localCharPtr8);
			Assert::AreEqual(localCharPtr8, ptrList.Front());

			//testing on empty list with user-defined type
			SList<Foo> fooList;
			Foo foo0(0);
			fooList.PushFront(foo0);
			Assert::AreEqual(0, fooList.Front().GetValue());

			//testing on non-empty list with user-defined type
			Foo foo1(1);
			fooList.PushFront(foo1);
			Assert::AreEqual(1, fooList.Front().GetValue());
			Foo foo2(2);
			fooList.PushFront(foo2);
			Assert::AreEqual(2, fooList.Front().GetValue());
		}

		TEST_METHOD(PopFrontTest)
		{
			// Testing on empty list with primitive types
			SList<char> list;
			auto listTry = [&list]() { list.PopFront();  };
			Assert::ExpectException<std::exception>(listTry);

			//testing on list of size 1 with primitive types
			list.PushFront('Q');
			list.PopFront();
			//Assert::AreEqual('Q', popped);
			Assert::AreEqual(0u, list.Size());
			auto listTry2 = [&list]() { list.Front(); };
			auto listTry3 = [&list]() { list.Back(); };
			Assert::ExpectException<std::exception>(listTry2);
			Assert::ExpectException<std::exception>(listTry3);

			//Testing on list of size 2 with primitive types
			list.PushFront('0');
			list.PushFront('1');
			list.PopFront();
			//Assert::AreEqual('1', popped);
			Assert::AreEqual(1u, list.Size());
			Assert::AreEqual('0', list.Front());
			Assert::AreEqual('0', list.Back());

			// Testing on empty list with pointers
			SList<char*> ptrList;
			auto ptrListTry = [&ptrList]() { ptrList.PopFront();  };
			Assert::ExpectException<std::exception>(ptrListTry);

			//testing on list of size 1 with pointers
			char localQ = 'Q';
			char *localQPtr = &localQ;
			ptrList.PushFront(localQPtr);
			//Assert::AreEqual(localQPtr, ptrList.PopFront());
			ptrList.PopFront();
			Assert::AreEqual(0u, ptrList.Size());
			auto ptrListTry2 = [&ptrList]() { ptrList.Front(); };
			auto ptrListTry3 = [&ptrList]() { ptrList.Back(); };
			Assert::ExpectException<std::exception>(ptrListTry2);
			Assert::ExpectException<std::exception>(ptrListTry3);

			//Testing on list of size 2 with pointers
			char local0 = '0', local1 = '1';
			char *local0Ptr = &local0, *local1Ptr = &local1;
			ptrList.PushFront(local0Ptr);
			ptrList.PushFront(local1Ptr);
			//Assert::AreEqual(local1Ptr, ptrList.PopFront());
			ptrList.PopFront();
			Assert::AreEqual(1u, ptrList.Size());
			Assert::AreEqual(local0Ptr, ptrList.Front());
			Assert::AreEqual(local0Ptr, ptrList.Back());

			//testing on empty list with user-defined types
			SList<Foo> fooList;
			auto fooListTry = [&fooList]() { fooList.PopFront();  };
			Assert::ExpectException<std::exception>(fooListTry);

			//testing on list of size 1 with user-defined types
			Foo foo0(0);
			fooList.PushFront(foo0);
			//Assert::AreEqual(0, fooList.PopFront().GetValue());
			fooList.PopFront();
			Assert::AreEqual(0u, fooList.Size());
			auto fooListTry2 = [&fooList]() { fooList.Front(); };
			auto fooListTry3 = [&fooList]() { fooList.Back(); };
			Assert::ExpectException<std::exception>(fooListTry2);
			Assert::ExpectException<std::exception>(fooListTry3);

			//testing on list of size 2 with user-defined types
			Foo foo1(1);
			fooList.PushFront(foo0);
			fooList.PushFront(foo1);
			//Assert::AreEqual(1, fooList.PopFront().GetValue());
			fooList.PopFront();
			Assert::AreEqual(0, fooList.Front().GetValue());
			Assert::AreEqual(0, fooList.Back().GetValue());
		}

		TEST_METHOD(PushBackTest)
		{
			//testing on empty list with primitive types
			SList<int> list;
			list.PushBack(10);
			Assert::AreEqual(1u, list.Size());
			Assert::AreEqual(10, list.Back());
			Assert::AreEqual(10, list.Front());

			//testing on non-empty list with primitive types
			list.PushBack(11);
			Assert::AreEqual(2u, list.Size());
			Assert::AreEqual(11, list.Back());
			Assert::AreEqual(10, list.Front());

			//testing on empty list with pointers
			SList<int*> ptrList;
			int local = 1;
			ptrList.PushBack(&local);
			Assert::AreEqual(1u, ptrList.Size());
			Assert::AreSame(ptrList.Front(), ptrList.Back());
			Assert::AreEqual(&local, ptrList.Back());

			//testing on non-empty list with pointers
			int local2 = 2;
			ptrList.PushBack(&local2);
			Assert::AreEqual(2u, ptrList.Size());
			Assert::AreEqual(&local2, ptrList.Back());
			Assert::AreEqual(&local, ptrList.Front());

			//testing on empty list with user-defined types
			SList<Foo> fooList;
			Foo foo0(0);
			fooList.PushBack(foo0);
			Assert::AreEqual(1u, fooList.Size());
			Assert::AreEqual(fooList.Back().GetValue(), fooList.Front().GetValue());
			Assert::AreEqual(0, fooList.Back().GetValue());

			//testing on non-empty list with user-defined types
			Foo foo1(1);
			fooList.PushBack(foo1);
			Assert::AreEqual(2u, fooList.Size());
			Assert::AreEqual(1, fooList.Back().GetValue());
			Assert::AreEqual(0, fooList.Front().GetValue());
		}

		TEST_METHOD(IsEmptyTest)
		{
			// testing with primitive types
			SList<float> emptyList;
			Assert::IsTrue(emptyList.IsEmpty());
			SList<float> nonEmptyList;
			nonEmptyList.PushFront(1.0f);
			Assert::IsFalse(nonEmptyList.IsEmpty());

			//testing with pointers
			SList<float*> emptyPtrList;
			Assert::IsTrue(emptyList.IsEmpty());
			SList<float*> nonEmptyPtrList;
			float local = 2.0f;
			nonEmptyPtrList.PushFront(&local);
			Assert::IsFalse(nonEmptyPtrList.IsEmpty());

			//testing with user-defined types
			SList<Foo> fooList;
			Assert::IsTrue(fooList.IsEmpty());
			Foo foo(35);
			fooList.PushBack(foo);
			Assert::IsFalse(fooList.IsEmpty());
		}

		TEST_METHOD(FrontTest)
		{
			//testing const version of method on empty list with primitive types
			const SList<int> constEmptyList;
			auto try1 = [&constEmptyList]() { constEmptyList.Front(); };
			Assert::ExpectException<std::exception>(try1);

			//testing const version of method on non-empty list with primitive types
			SList<int> constNonEmptyList;
			constNonEmptyList.PushFront(787);
			const SList<int>& constNonEmptyListRef = constNonEmptyList;
			Assert::AreEqual(787, constNonEmptyListRef.Front());

			//testing non-const version of method on empty list with primitive types
			SList<int> nonConstEmptyList;
			auto try2 = [&nonConstEmptyList]() { nonConstEmptyList.Front(); };
			Assert::ExpectException<std::exception>(try2);

			//testing non-const version of method on non-empty list with primitive types
			SList<int> nonConstNonEmptyList;
			nonConstNonEmptyList.PushFront(11);
			Assert::AreEqual(11, nonConstNonEmptyList.Front());
			nonConstNonEmptyList.Front() += 9;
			Assert::AreEqual(20, nonConstNonEmptyList.Front());

			//testing const version of method on empty list with pointers
			const SList<int*> constEmptyPtrList;
			auto try3 = [&constEmptyPtrList]() { constEmptyPtrList.Front(); };
			Assert::ExpectException<std::exception>(try3);

			//testing const version of method on non-empty list with pointers
			SList<int*> constNonEmptyPtrList;
			int local1 = 1;
			constNonEmptyPtrList.PushBack(&local1);
			const SList<int*>& constNonEmptyPtrListRef = constNonEmptyPtrList;
			Assert::AreEqual(&local1, constNonEmptyPtrListRef.Front());

			//testing non-const version of method on empty list with pointers
			SList<int*> nonConstEmptyPtrList;
			auto try4 = [&nonConstEmptyPtrList]() { nonConstEmptyPtrList.Front(); };
			Assert::ExpectException<std::exception>(try4);

			//testing non-const version of method on non-empty list with pointers
			SList<int*> nonConstNonEmptyPtrList;
			nonConstNonEmptyPtrList.PushFront(&local1);
			Assert::AreEqual(local1, *(nonConstNonEmptyPtrList.Front()));
			local1 -= 2;
			Assert::AreEqual(-1, *(nonConstNonEmptyPtrList.Front()));

			//testing const version of method on empty list with user-defined types
			SList<Foo> constEmptyFooList;
			auto try5 = [&constEmptyFooList]() { constEmptyFooList.Front(); };
			Assert::ExpectException<std::exception>(try5);

			//testing const version of method on non-empty list with user-defined types
			SList<Foo> constNonEmptyFooList;
			Foo foo1(1);
			constNonEmptyFooList.PushFront(foo1);
			const SList<Foo>& constNonEmptyFooListRef = constNonEmptyFooList;
			Assert::AreEqual(1, constNonEmptyFooListRef.Front().GetValue());

			//testing non-const version of method on empty list with user-defined types
			SList<Foo> nonConstEmptyFooList;
			auto try6 = [&nonConstEmptyFooList]() { nonConstEmptyFooList.Front(); };
			Assert::ExpectException<std::exception>(try6);
			
			//testing non-const version of method of non-empty list with user-defined types
			SList<Foo> nonConstNonEmptyFooList;
			nonConstNonEmptyFooList.PushFront(foo1);
			Assert::AreEqual(foo1.GetValue(), nonConstNonEmptyFooList.Front().GetValue());
		}

		TEST_METHOD(BackTest)
		{
			//testing const version of method on empty list with primitive types
			const SList<int> constEmptyList;
			auto try1 = [&constEmptyList]() { constEmptyList.Back(); };
			Assert::ExpectException<std::exception>(try1);

			//testing const version of method on non-empty list with primitive types
			SList<int> constNonEmptyList;
			constNonEmptyList.PushBack(787);
			const SList<int>& constNonEmptyListRef = constNonEmptyList;
			Assert::AreEqual(787, constNonEmptyListRef.Back());

			//testing non-const version of method on empty list with primitive types
			SList<int> nonConstEmptyList;
			auto try2 = [&nonConstEmptyList]() { nonConstEmptyList.Back(); };
			Assert::ExpectException<std::exception>(try2);

			//testing non-const version of method on non-empty list with primitive types
			SList<int> nonConstNonEmptyList;
			nonConstNonEmptyList.PushBack(11);
			Assert::AreEqual(11, nonConstNonEmptyList.Back());
			nonConstNonEmptyList.Back() += 9;
			Assert::AreEqual(20, nonConstNonEmptyList.Back());

			//testing const version of method on empty list with pointers
			const SList<int*> constEmptyPtrList;
			auto try3 = [&constEmptyPtrList]() { constEmptyPtrList.Back(); };
			Assert::ExpectException<std::exception>(try3);

			//testing const version of method on non-empty list with pointers
			SList<int*> constNonEmptyPtrList;
			int local1 = 1;
			constNonEmptyPtrList.PushBack(&local1);
			const SList<int*>& constNonEmptyPtrListRef = constNonEmptyPtrList;
			Assert::AreEqual(&local1, constNonEmptyPtrListRef.Back());

			//testing non-const version of method on empty list with pointers
			SList<int*> nonConstEmptyPtrList;
			auto try4 = [&nonConstEmptyPtrList]() { nonConstEmptyPtrList.Back(); };
			Assert::ExpectException<std::exception>(try4);

			//testing non-const version of method on non-empty list with pointers
			SList<int*> nonConstNonEmptyPtrList;
			nonConstNonEmptyPtrList.PushBack(&local1);
			Assert::AreEqual(local1, *(nonConstNonEmptyPtrList.Back()));
			local1 -= 2;
			Assert::AreEqual(-1, *(nonConstNonEmptyPtrList.Back()));

			//testing const version of method on empty list with user-defined types
			SList<Foo> constEmptyFooList;
			auto try5 = [&constEmptyFooList]() { constEmptyFooList.Back(); };
			Assert::ExpectException<std::exception>(try5);

			//testing const version of method on non-empty list with user-defined types
			SList<Foo> constNonEmptyFooList;
			Foo foo1(1);
			constNonEmptyFooList.PushBack(foo1);
			const SList<Foo>& constNonEmptyFooListRef = constNonEmptyFooList;
			Assert::AreEqual(1, constNonEmptyFooListRef.Back().GetValue());

			//testing non-const version of method on empty list with user-defined types
			SList<Foo> nonConstEmptyFooList;
			auto try6 = [&nonConstEmptyFooList]() { nonConstEmptyFooList.Back(); };
			Assert::ExpectException<std::exception>(try6);

			//testing non-const version of method of non-empty list with user-defined types
			SList<Foo> nonConstNonEmptyFooList;
			nonConstNonEmptyFooList.PushBack(foo1);
			Assert::AreEqual(foo1.GetValue(), nonConstNonEmptyFooList.Back().GetValue());
		}

		TEST_METHOD(SizeTest)
		{
			//testing with primitive types
			SList<int> list;
			Assert::AreEqual(0u, list.Size());
			for (unsigned int i = 0u; i < 10u; ++i)
			{
				list.PushFront(i);
			}
			Assert::AreEqual(10u, list.Size());

			//testing with pointers
			SList<unsigned int*> ptrList;
			Assert::AreEqual(0u, ptrList.Size());
			for (unsigned int i = 0u; i < 9u; ++i)
			{
				ptrList.PushFront(&i);
			}
			Assert::AreEqual(9u, ptrList.Size());

			//testing with user-defined types
			SList<Foo> fooList;
			Foo foo(5000);
			Assert::AreEqual(0u, fooList.Size());
			for (int i = 0; i < 3; i++)
			{
				fooList.PushFront(foo);
			}
			Assert::AreEqual(3u, fooList.Size());
		}

		TEST_METHOD(AssignmentOperatorTest)
		{
			//testing const version of method with primitive types
			const SList<int> emptyList;
			SList<int> shouldBeEmpty;
			shouldBeEmpty.PushFront(12);
			shouldBeEmpty = emptyList;
			Assert::AreEqual(0u, shouldBeEmpty.Size());
			auto try1 = [&shouldBeEmpty]() { shouldBeEmpty.Front(); };
			auto try2 = [&shouldBeEmpty]() { shouldBeEmpty.Back(); };
			Assert::ExpectException<std::exception>(try1);
			Assert::ExpectException<std::exception>(try2);

			//testing non-const version of method with primitive types
			SList<char> lhs;
			SList<char> rhs;
			lhs.PushFront('l');
			rhs.PushFront('r');
			lhs = rhs;
			Assert::AreEqual('r', lhs.Front());
			Assert::AreEqual('r', rhs.Front());

			//testing const version of method with pointers
			const SList<int*> emptyPtrList;
			SList<int*> shouldBeEmptyPtr;
			int local1 = 1;
			shouldBeEmptyPtr.PushFront(&local1);
			shouldBeEmptyPtr = emptyPtrList;
			Assert::AreEqual(0u, shouldBeEmptyPtr.Size());
			auto try3 = [&shouldBeEmptyPtr]() { shouldBeEmptyPtr.Front(); };
			auto try4 = [&shouldBeEmptyPtr]() { shouldBeEmptyPtr.Back(); };
			Assert::ExpectException<std::exception>(try1);
			Assert::ExpectException<std::exception>(try2);

			//testing non-const version of method with pointers
			SList<char*> lhsPtr;
			SList<char*> rhsPtr;
			char l = 'l', r = 'r';
			lhsPtr.PushFront(&l);
			rhsPtr.PushFront(&r);
			lhsPtr = rhsPtr;
			Assert::AreEqual(&r, lhsPtr.Front());
			Assert::AreEqual(&r, rhsPtr.Front());

			//testing const version of method with user-defined types
			const SList<Foo> emptyFooList;
			SList<Foo> shouldBeEmptyFoo;
			Foo foo(843483);
			shouldBeEmptyFoo.PushBack(foo);
			shouldBeEmptyFoo = emptyFooList;
			Assert::AreEqual(0u, shouldBeEmptyFoo.Size());
			auto try5 = [&shouldBeEmptyFoo]() { shouldBeEmptyFoo.Front(); };
			auto try6 = [&shouldBeEmptyFoo]() { shouldBeEmptyFoo.Back(); };

			//testing non-const version of method with user-defined types
			SList<Foo> lhsFoo, rhsFoo;
			Foo foo1(1);
			Foo foo2(2);
			lhsFoo.PushFront(foo1);
			rhsFoo.PushFront(foo2);
			lhsFoo = rhsFoo;
			Assert::AreEqual(2, lhsFoo.Front().GetValue());
			Assert::AreEqual(2, rhsFoo.Front().GetValue());
		}

		TEST_METHOD(ClearTest)
		{
			//testing on empty list with primitive types
			SList<int> list;
			list.Clear();
			Assert::AreEqual(0u, list.Size());
			auto try1 = [&list]() { list.Front(); };
			auto try2 = [&list]() { list.Back(); };
			Assert::ExpectException<std::exception>(try1);
			Assert::ExpectException<std::exception>(try2);

			//testing on non-empty list with primitive types
			list.PushFront(2);
			list.Clear();
			Assert::AreEqual(0u, list.Size());
			Assert::ExpectException<std::exception>(try1);
			Assert::ExpectException<std::exception>(try2);

			//testing on empty list with pointers
			SList<int*> ptrList;
			ptrList.Clear();
			Assert::AreEqual(0u, ptrList.Size());
			auto try3 = [&ptrList]() { ptrList.Front(); };
			auto try4 = [&ptrList]() { ptrList.Back(); };
			Assert::ExpectException<std::exception>(try3);
			Assert::ExpectException<std::exception>(try4);

			//testing on non-empty list with pointers
			int* dynamically = new int(10);
			ptrList.PushFront(dynamically);
			ptrList.Clear();
			Assert::AreEqual(0u, ptrList.Size());
			Assert::ExpectException<std::exception>(try3);
			Assert::ExpectException<std::exception>(try4);
			delete dynamically;

			//testing on empty list with user-defined types
			SList<Foo> fooList;
			fooList.Clear();
			Assert::AreEqual(0u, fooList.Size());
			auto try5 = [&fooList]() { fooList.Front(); };
			auto try6 = [&fooList]() { fooList.Back(); };
			Assert::ExpectException<std::exception>(try5);
			Assert::ExpectException<std::exception>(try6);

			//testing on non-empty list with user-defined types
			Foo foo(2);
			fooList.PushFront(foo);
			fooList.Clear();
			Assert::AreEqual(0u, fooList.Size());
			Assert::ExpectException<std::exception>(try5);
			Assert::ExpectException<std::exception>(try6);
		}

		TEST_METHOD(IteratorEqualityOperatorTest)
		{
			//testing with primitive types
			SList<int> list;
			Assert::IsTrue(list.begin() == list.end());
			list.PushBack(1);
			Assert::IsTrue(++(list.begin()) == list.end());

			//testing with pointers
			SList<int*> ptrList;
			Assert::IsTrue(ptrList.begin() == ptrList.end());
			int local = 5;
			ptrList.PushBack(&local);
			Assert::IsTrue(++(ptrList.begin()) == ptrList.end());

			//testing with user-defined types
			SList<Foo> fooList;
			Assert::IsTrue(fooList.begin() == fooList.end());
			Foo foo(4);
			fooList.PushBack(foo);
			Assert::IsTrue(++(fooList.begin()) == fooList.end());
		}

		TEST_METHOD(IteratorInequalityOperatorTest)
		{
			//testing with primitive types
			SList<int> list;
			Assert::IsFalse(list.begin() != list.end());
			list.PushBack(1);
			Assert::IsFalse(++(list.begin()) != list.end());

			//testing with pointers
			SList<int*> ptrList;
			Assert::IsFalse(ptrList.begin() != ptrList.end());
			int local = 5;
			ptrList.PushBack(&local);
			Assert::IsFalse(++(ptrList.begin()) != ptrList.end());

			//testing with user-defined types
			SList<Foo> fooList;
			Assert::IsFalse(fooList.begin() != fooList.end());
			Foo foo(4);
			fooList.PushBack(foo);
			Assert::IsFalse(++(fooList.begin()) != fooList.end());
		}

		TEST_METHOD(IteratorPrefixIncrementOperatorTest)
		{
			//testing with primitive types
			SList<char> list;
			auto try1 = [&list]() { ++list.end(); };
			Assert::ExpectException<std::exception>(try1);
			char localZ = 'Z';
			char localA = 'A';
			list.PushFront(localZ);
			list.PushFront(localA);
			Assert::AreEqual('Z', *++list.begin());

			//testing with pointers
			SList<char*> ptrList;
			auto try2 = [&ptrList]() { ++ptrList.begin(); };
			Assert::ExpectException<std::exception>(try2);
			ptrList.PushFront(&localZ);
			ptrList.PushFront(&localA);
			Assert::AreEqual(&localZ, *++ptrList.begin());

			//testing with user-defined types
			SList<Foo> fooList;
			auto try3 = [&fooList]() { ++fooList.begin(); };
			Assert::ExpectException<std::exception>(try3);
			Foo foo2(2), foo1(1);
			fooList.PushFront(foo2);
			fooList.PushFront(foo1);
			Assert::IsTrue(foo2 == *++fooList.begin());
		}

		TEST_METHOD(IteratorPostfixIncrementOperatorTest)
		{
			//testing with primitive types
			SList<char> list;
			auto try1 = [&list]() { list.end()++; };
			Assert::ExpectException<std::exception>(try1);
			char localZ = 'Z';
			char localA = 'A';
			list.PushFront(localZ);
			list.PushFront(localA);
			auto it = list.begin();
			Assert::AreEqual('A', *it++);
			Assert::AreEqual('Z', *it);

			//testing with pointers
			SList<char*> ptrList;
			auto try2 = [&ptrList]() { ptrList.begin()++; };
			Assert::ExpectException<std::exception>(try2);
			ptrList.PushFront(&localZ);
			ptrList.PushFront(&localA);
			auto ptrIt = ptrList.begin();
			Assert::AreEqual(&localA, *ptrIt++);
			Assert::AreEqual(&localZ, *ptrIt);

			//testing with user-defined types
			SList<Foo> fooList;
			auto try3 = [&fooList]() { fooList.begin()++; };
			Assert::ExpectException<std::exception>(try3);
			Foo foo2(2), foo1(1);
			fooList.PushFront(foo2);
			fooList.PushFront(foo1);
			auto fooIt = fooList.begin();
			Assert::IsTrue(foo1 == *fooIt++);
			Assert::IsTrue(foo2 == *fooIt);
		}

		TEST_METHOD(IteratorDereferenceOperatorTest)
		{
			//testing with primitive types
			SList<int> list;
			list.PushFront(0);
			Assert::AreEqual(0, *list.begin());
			auto try1 = [&list]() { *list.end(); };
			Assert::ExpectException<std::exception>(try1);

			//testing with pointers
			SList<char*> ptrList;
			ptrList.PushBack(nullptr);
			Assert::AreEqual(nullptr, *ptrList.begin());
			auto try2 = [&ptrList]() { *ptrList.end(); };
			Assert::ExpectException<std::exception>(try2);

			//testing with user-defined types
			SList<Foo> fooList;
			Foo foo(24);
			fooList.PushFront(foo);
			Assert::IsTrue(*fooList.begin() == foo);
			auto try3 = [&fooList]() { *fooList.end(); };
			Assert::ExpectException<std::exception>(try3);
		}

		TEST_METHOD(BeginEndTest)
		{
			//testing with primitive types
			SList<int> list;
			list.PushFront(1);
			list.PushFront(10);
			list.PushFront(100);
			list.PushFront(1000);
			int total = 0;
			for (auto it = list.begin(); it != list.end(); ++it)
			{
				total += *it;
			}
			Assert::AreEqual(1111, total);

			//testing with pointers
			SList<int*> ptrList;
			int local1 = 1;
			int local10 = 10;
			int local100 = 100;
			int local1000 = 1000;
			ptrList.PushFront(&local1);
			ptrList.PushBack(&local10);
			ptrList.PushFront(&local100);
			ptrList.PushBack(&local1000);
			for (auto it = ptrList.begin(); it != ptrList.end(); ++it)
			{
				total += **it;
			}
			Assert::AreEqual(2222, total);

			//testing with user-defined types
			Foo foo1(1);
			Foo foo10(10);
			Foo foo100(100);
			SList<Foo> fooList;
			fooList.PushFront(foo10);
			fooList.PushFront(foo100);
			fooList.PushFront(foo1);
			for (auto it = fooList.begin(); it != fooList.end(); ++it)
			{
				total += (*it).GetValue();
			}
			Assert::AreEqual(2333, total);
		}

		TEST_METHOD(InsertAfterTest)
		{
			//testing on an empty list of primitive types
			//inserting after the beginning
			SList<int> list;
			list.InsertAfter(5, list.begin());
			Assert::AreEqual(1u, list.Size());
			Assert::AreEqual(5, *list.begin());
			Assert::AreEqual(5, list.Back());
			//inserting after the end
			list.Clear();
			list.InsertAfter(4, list.end());
			Assert::AreEqual(1u, list.Size());
			Assert::AreEqual(4, *list.begin());
			Assert::AreEqual(4, list.Back());

			//testing on a non-empty list of primitive types
			list.PushBack(6);
			list.PushBack(8);
			//inserting after the beginning
			auto it = list.begin();
			list.InsertAfter(5, it);
			Assert::AreEqual(4u, list.Size());
			Assert::AreEqual(5, *++it);
			Assert::AreEqual(6, *++it);
			//inserting in the middle
			list.InsertAfter(7, it);
			Assert::AreEqual(5u, list.Size());
			Assert::AreEqual(7, *++it);
			Assert::AreEqual(8, *++it);
			//inserting at the end
			list.InsertAfter(9, list.end());
			Assert::AreEqual(6u, list.Size());
			Assert::AreEqual(9, *++it);
			Assert::IsTrue(++it == list.end());

			//testing on an empty list of pointers
			//inserting after the beginning
			int arrayOfInts[] = { 4, 5, 6, 7, 8, 9 };
			SList<int*> ptrList;
			ptrList.InsertAfter(&arrayOfInts[0], ptrList.begin());
			Assert::AreEqual(1u, ptrList.Size());
			Assert::AreEqual(&arrayOfInts[0], *ptrList.begin());
			//inserting at the end
			ptrList.Clear();
			ptrList.InsertAfter(&arrayOfInts[1], ptrList.end());
			Assert::AreEqual(1u, ptrList.Size());
			Assert::AreEqual(&arrayOfInts[1], *ptrList.begin());
			Assert::AreEqual(&arrayOfInts[1], ptrList.Back());

			//testing on a non-empty list of pointers
			ptrList.Clear();
			for (int i = 0; i < 6; i += 2)
			{
				ptrList.PushBack(&arrayOfInts[i]);
			}
			//inserting after the beginning
			auto ptrIt = ptrList.begin();
			ptrList.InsertAfter(&arrayOfInts[1], ptrIt);
			Assert::AreEqual(4u, ptrList.Size());
			Assert::AreEqual(&arrayOfInts[1], *++ptrIt);
			Assert::AreEqual(&arrayOfInts[2], *++ptrIt);
			//inserting in the middle
			ptrList.InsertAfter(&arrayOfInts[3], ptrIt);
			Assert::AreEqual(5u, ptrList.Size());
			Assert::AreEqual(&arrayOfInts[3], *++ptrIt);
			Assert::AreEqual(&arrayOfInts[4], *++ptrIt);
			//inserting at the end
			ptrList.InsertAfter(&arrayOfInts[5], ptrList.end());
			Assert::AreEqual(6u, ptrList.Size());
			Assert::AreEqual(&arrayOfInts[5], *++ptrIt);
			Assert::IsTrue(++ptrIt == ptrList.end());

			//testing on an empty list of user-defined types
			SList<Foo> fooList;
			//inserting after the beginning
			Foo foo0(0);
			fooList.InsertAfter(foo0, fooList.begin());
			Assert::AreEqual(1u, fooList.Size());
			Assert::IsTrue(foo0 == fooList.Front());
			Assert::IsTrue(foo0 == fooList.Back());
			//inserting at the end
			fooList.Clear();
			Foo foo1(1);
			fooList.InsertAfter(foo1, fooList.end());
			Assert::AreEqual(1u, fooList.Size());
			Assert::IsTrue(foo1 == fooList.Front());
			Assert::IsTrue(foo1 == fooList.Back());

			//testing on a non-empty list of user-defined types
			fooList.Clear();
			fooList.PushFront(foo0);
			Foo foo2(2);
			fooList.PushBack(foo2);
			Foo foo4(4);
			fooList.PushBack(foo4);
			//inserting after the beginning
			fooList.InsertAfter(foo1, fooList.begin());
			Assert::AreEqual(4u, fooList.Size());
			auto fooIt = fooList.begin();
			Assert::IsTrue(*++fooIt == foo1);
			Assert::IsTrue(*++fooIt == foo2);
			//inserting in the middle
			Foo foo3(3);
			fooList.InsertAfter(foo3, fooIt);
			Assert::AreEqual(5u, fooList.Size());
			Assert::IsTrue(*++fooIt == foo3);
			Assert::IsTrue(*++fooIt == foo4);
			//inserting at the end
			Foo foo5(5);
			fooList.InsertAfter(foo5, fooList.end());
			Assert::AreEqual(6u, fooList.Size());
			Assert::IsTrue(*++fooIt == foo5);
			Assert::IsTrue(++fooIt == fooList.end());
		}

		TEST_METHOD(FindTest)
		{
			//testing on an empty list of primitive types
			SList<int> list;
			auto try1 = [&list]() { list.Find(22); };
			Assert::ExpectException<std::exception>(try1);

			//testing on a non-empty list of primitive types
			list.PushFront(1);
			list.PushBack(2);
			list.PushBack(3);
			auto it = list.begin();
			Assert::IsTrue(list.Find(1) == it);
			Assert::IsTrue(list.Find(2) == ++it);
			Assert::IsTrue(list.Find(3) == ++it);
			Assert::ExpectException<std::exception>(try1);

			//testing on an empty list of pointers
			SList<int*> ptrList;
			auto try2 = [&ptrList] { ptrList.Find(nullptr); };
			Assert::ExpectException<std::exception>(try2);
			
			//testing on a non-empty list of pointers
			int local0 = 0, local1 = 1, local2 = 2;
			ptrList.PushBack(&local0);
			ptrList.PushBack(&local1);
			ptrList.PushBack(&local2);
			auto ptrIt = ptrList.begin();
			Assert::IsTrue(ptrList.Find(&local0) == ptrIt);
			Assert::IsTrue(ptrList.Find(&local1) == ++ptrIt);
			Assert::IsTrue(ptrList.Find(&local2) == ++ptrIt);
			Assert::ExpectException<std::exception>(try2);

			//testing on an empty list of user-defined types
			Foo foo(373);
			SList<Foo> fooList;
			auto try3 = [&fooList, &foo]() { fooList.Find(foo); };
			Assert::ExpectException<std::exception>(try3);

			//testing on a non-empty list of user-defined types
			Foo foo0(0), foo1(1), foo2(2);
			fooList.PushBack(foo0);
			fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			auto fooIt = fooList.begin();
			Assert::IsTrue(fooIt == fooList.Find(foo0));
			Assert::IsTrue(++fooIt == fooList.Find(foo1));
			Assert::IsTrue(++fooIt == fooList.Find(foo2));
			Assert::ExpectException<std::exception>(try3);
		}

		TEST_METHOD(RemoveTest)
		{
			//testing on an empty list of primitive types
			SList<int> list;
			Assert::IsFalse(list.Remove(43));

			//testing on a non-empty list of primitive types
			list.PushBack(0);
			list.Remove(0);
			Assert::IsTrue(list.IsEmpty());
			list.PushBack(0);
			list.PushBack(1);
			list.PushBack(2);
			Assert::IsFalse(list.Remove(234));
			Assert::IsTrue(list.Remove(0));
			Assert::AreEqual(2u, list.Size());
			Assert::AreEqual(1, list.Front());
			Assert::AreEqual(2, list.Back());
			Assert::IsTrue(list.Remove(2));
			Assert::AreEqual(1u, list.Size());
			Assert::AreEqual(1, list.Front());
			Assert::AreEqual(1, list.Back());

			//testing on an empty list of pointers
			SList<int*> ptrList;
			Assert::IsFalse(ptrList.Remove(nullptr));
			
			//testing on a non-empty list of pointers
			int local0 = 0, local1 = 1, local2 = 2;
			ptrList.PushBack(&local0);
			ptrList.Remove(&local0);
			Assert::IsTrue(ptrList.IsEmpty());
			ptrList.PushBack(&local0);
			ptrList.PushBack(&local1);
			ptrList.PushBack(&local2);
			Assert::IsTrue(ptrList.Remove(&local0));
			Assert::AreEqual(2u, ptrList.Size());
			Assert::AreEqual(&local1, ptrList.Front());
			Assert::AreEqual(&local2, ptrList.Back());
			Assert::IsTrue(ptrList.Remove(&local2));
			Assert::AreEqual(1u, ptrList.Size());
			Assert::AreEqual(&local1, ptrList.Front());
			Assert::AreEqual(&local1, ptrList.Back());
			Assert::IsFalse(ptrList.Remove(&local0));
			

			//testing on an empty list of user-defined types
			SList<Foo> fooList;
			Foo foo0(0);
			Assert::IsFalse(fooList.Remove(foo0));

			//testing on a non-empty list of user-defined types
			Foo foo1(1), foo2(2);
			fooList.PushBack(foo0);
			fooList.Remove(foo0);
			Assert::IsTrue(fooList.IsEmpty());
			fooList.PushBack(foo0);
			fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			Assert::IsTrue(fooList.Remove(foo0));
			Assert::IsTrue(fooList.Front() == foo1);
			Assert::IsTrue(fooList.Back() == foo2);
			Assert::IsTrue(fooList.Remove(foo2));
			Assert::IsTrue(fooList.Front() == foo1);
			Assert::IsTrue(fooList.Back() == foo1);
			Assert::IsFalse(fooList.Remove(foo2));
		}
	private:
		_CrtMemState initialMemoryState;
	};
}