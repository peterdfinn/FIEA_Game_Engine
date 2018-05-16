#include "pch.h"

#include "HashMap.h"
#include "BadHashFunctor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;

namespace UnitTest
{
	TEST_CLASS(HashMapUnitTest)
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
		TEST_METHOD(HMIteratorDefaultConstructorTest)
		{
			HashMap<Foo, Foo>::Iterator it;
			auto lambda1 = [&it]() { ++it; };
			Assert::ExpectException<std::exception>(lambda1);
		}

		TEST_METHOD(HMIteratorDestructorTest)
		{
			HashMap<Foo, Foo>::Iterator *it;
			it;
		}

		TEST_METHOD(HashMapMoveConstructorTest)
		{
			HashMap<int32_t, int32_t> map;
			map.Insert(std::make_pair<int32_t, int32_t>(10, 10));
			HashMap<int32_t, int32_t> moved = std::move(map);
		}

		TEST_METHOD(HMIteratorCopyConstructorTest)
		{
			HashMap<Foo, Foo> hashMap;
			hashMap.Insert(std::pair<Foo, Foo>(Foo(1), Foo(2)));
			HashMap<Foo, Foo>::Iterator it = hashMap.begin();
			HashMap<Foo, Foo>::Iterator itCopy = it;
			Assert::IsTrue(it == itCopy);
		}

		TEST_METHOD(HashMapFindTest)
		{
			//testing with default hash functor
			HashMap<Foo, Foo> goodHashMap;
			std::pair<Foo, Foo> pair(Foo(0), Foo(12));
			Assert::IsTrue(goodHashMap.Find(pair.first) == goodHashMap.end());
			goodHashMap.Insert(pair);
			auto it = goodHashMap.Find(pair.first);
			Assert::IsTrue(*it == pair);

			//testing with const char*
			HashMap<const char*, Foo> charStarHashMap;
			std::pair<const char*, Foo> pair2("now", Foo(4));
			Assert::IsTrue(charStarHashMap.Find("now") == charStarHashMap.end());
			charStarHashMap.Insert(pair2);
			auto it2 = charStarHashMap.Find("now");
			Assert::IsTrue(*it2 == pair2);

			//testing with std::string
			HashMap<std::string, Foo> stringHashMap;
			std::pair<std::string, Foo> pair3("now", Foo(44));
			Assert::IsTrue(stringHashMap.Find("now") == stringHashMap.end());
			stringHashMap.Insert(pair3);
			auto it3 = stringHashMap.Find("now");
			bool findFailed = (it3 == stringHashMap.end());
			Assert::IsFalse(findFailed);
			Assert::IsTrue(*it3 == pair3);
		}

		TEST_METHOD(HashMapInsertTest)
		{
			//standard test
			HashMap<int32_t, Foo> hashMap;
			std::pair<int32_t, Foo> pair(5, Foo(6));
			hashMap.Insert(pair);
			auto it = hashMap.Find(5);
			Assert::AreEqual(6, it->second.GetValue());
			std::pair<int32_t, Foo> pair2(5, Foo(200));
			hashMap.Insert(pair2);
			it = hashMap.Find(5);
			Assert::AreEqual(6, it->second.GetValue());

			//test with const char*
			HashMap<char*, Foo> charStarHashMap;
			std::pair<char*, Foo> pair3("I'm a rebel just for kicks now", Foo(-1));
			charStarHashMap.Insert(pair3);
			auto it2 = charStarHashMap.Find("I'm a rebel just for kicks now");
			Assert::AreEqual(-1, it2->second.GetValue());
			std::pair<char*, Foo> pair4("I'm a rebel just for kicks now", Foo(4));
			charStarHashMap.Insert(pair4);
			it2 = charStarHashMap.Find("I'm a rebel just for kicks now");
			Assert::AreEqual(-1, it2->second.GetValue());

			//test with std::string
			HashMap<std::string, Foo> stringHashMap;
			std::pair<std::string, Foo> pair5("I've been feeling it since 1966 now", Foo(-1));
			stringHashMap.Insert(pair5);
			auto it3 = stringHashMap.Find("I've been feeling it since 1966 now");
			Assert::AreEqual(-1, it3->second.GetValue());
			std::pair<std::string, Foo> pair6("I've been feeling it since 1966 now", Foo(4));
			stringHashMap.Insert(pair6);
			it3 = stringHashMap.Find("I've been feeling it since 1966 now");
			Assert::AreEqual(-1, it3->second.GetValue());
		}

		TEST_METHOD(HashMapInsertBoolTest)
		{
			//standard test
			HashMap<int32_t, Foo> hashMap;
			std::pair<int32_t, Foo> pair(5, Foo(6));
			bool alreadyPresent;
			hashMap.Insert(pair, alreadyPresent);
			Assert::IsFalse(alreadyPresent);
			auto it = hashMap.Find(5);
			Assert::AreEqual(6, it->second.GetValue());
			std::pair<int32_t, Foo> pair2(5, Foo(200));
			hashMap.Insert(pair2, alreadyPresent);
			Assert::IsTrue(alreadyPresent);
			it = hashMap.Find(5);
			Assert::AreEqual(6, it->second.GetValue());

			//test with const char*
			HashMap<char*, Foo> charStarHashMap;
			std::pair<char*, Foo> pair3("I'm a rebel just for kicks now", Foo(-1));
			charStarHashMap.Insert(pair3, alreadyPresent);
			Assert::IsFalse(alreadyPresent);
			auto it2 = charStarHashMap.Find("I'm a rebel just for kicks now");
			Assert::AreEqual(-1, it2->second.GetValue());
			std::pair<char*, Foo> pair4("I'm a rebel just for kicks now", Foo(4));
			charStarHashMap.Insert(pair4, alreadyPresent);
			Assert::IsTrue(alreadyPresent);
			it2 = charStarHashMap.Find("I'm a rebel just for kicks now");
			Assert::AreEqual(-1, it2->second.GetValue());

			//test with std::string
			HashMap<std::string, Foo> stringHashMap;
			std::pair<std::string, Foo> pair5("I've been feeling it since 1966 now", Foo(-1));
			stringHashMap.Insert(pair5, alreadyPresent);
			Assert::IsFalse(alreadyPresent);
			auto it3 = stringHashMap.Find("I've been feeling it since 1966 now");
			Assert::AreEqual(-1, it3->second.GetValue());
			std::pair<std::string, Foo> pair6("I've been feeling it since 1966 now", Foo(4));
			stringHashMap.Insert(pair6, alreadyPresent);
			Assert::IsTrue(alreadyPresent);
			it3 = stringHashMap.Find("I've been feeling it since 1966 now");
			Assert::AreEqual(-1, it3->second.GetValue());
		}

		TEST_METHOD(HashMapAtTest)
		{
			//testing non-const version
			HashMap<int, Foo> hashMap;
			std::pair<int, Foo> pair1(1, Foo(1));
			hashMap.Insert(pair1);
			Foo& resultOfAt = hashMap.At(1);
			Assert::AreEqual(1, resultOfAt.GetValue());
			Foo foo2(2);
			resultOfAt = foo2;
			auto it = hashMap.Find(1);
			Assert::AreEqual(2, it->second.GetValue());
			auto lambda1 = [&hashMap]() { hashMap.At(20); };
			Assert::ExpectException<std::exception>(lambda1);
			
			//testing const version
			const HashMap<int, Foo>& hashMapConst = hashMap;
			const Foo& resultOfConstAt = hashMapConst.At(1);
			Assert::AreEqual(2, resultOfConstAt.GetValue());
			auto lambda2 = [&hashMapConst]() { hashMapConst.At(3); };
			Assert::ExpectException<std::exception>(lambda2);
		}

		TEST_METHOD(HashMapBracketsOperatorTest)
		{
			HashMap<int, Foo> hashMap;
			std::pair<int, Foo> pair1(1, Foo(1));
			hashMap.Insert(pair1);
			Foo& resultOfAt = hashMap[1];
			Assert::AreEqual(1, resultOfAt.GetValue());
			Foo foo2(2);
			resultOfAt = foo2;
			auto it = hashMap.Find(1);
			Assert::AreEqual(2, it->second.GetValue());
			
			resultOfAt = hashMap[20];
			bool testBracketInsertion;
			hashMap.Insert(std::make_pair(20, resultOfAt), testBracketInsertion);
			Assert::IsTrue(testBracketInsertion);
		}

		TEST_METHOD(HashMapRemoveTest)
		{
			HashMap<Foo, Foo, HashFunctor<Foo>, Foo> hashMap;
			std::pair<Foo, Foo> pair1(Foo(1), Foo(2));
			Assert::IsFalse(hashMap.Remove(pair1.first));
			hashMap.Insert(pair1);
			Assert::IsFalse(hashMap.Remove(pair1.second));
			Assert::IsTrue(hashMap.Remove(pair1.first));
			Assert::AreEqual(0u, hashMap.Size());
		}

		TEST_METHOD(HashMapSizeClearTest)
		{
			HashMap<int, Foo> hashMap;
			for (int i = 0; i < 13; ++i)
			{
				hashMap.Insert(std::make_pair(i, Foo(i * 3)));
			}
			Assert::AreEqual(13u, hashMap.Size());
			hashMap.Clear();
			Assert::AreEqual(0u, hashMap.Size());
		}

		TEST_METHOD(HashMapContainsKeyTest)
		{
			HashMap<int, Foo> hashMap;
			std::pair<int, Foo> pair1(1, Foo(1));
			hashMap.Insert(pair1);
			const HashMap<int, Foo>& hashMapConst = hashMap;
			bool resultOfContainsKey = hashMapConst.ContainsKey(1);
			Assert::IsTrue(resultOfContainsKey);
			resultOfContainsKey = hashMapConst.ContainsKey(3);
			Assert::IsFalse(resultOfContainsKey);
		}

		TEST_METHOD(HashMapBeginEndTest)
		{
			HashMap<int, int> hashMap;
			int numberOfEntries = 0;
			for (auto it = hashMap.begin(); it != hashMap.end(); ++it)
			{
				++numberOfEntries;
			}
			Assert::AreEqual(0, numberOfEntries);

			for (int i = 0; i < 13; ++i)
			{
				hashMap.Insert(std::make_pair(i, i));
			}
			std::pair<int, int> thisPair;
			for (auto it = hashMap.begin(); it != hashMap.end(); ++it)
			{
				thisPair = *it;
				++numberOfEntries;
			}
			Assert::AreEqual(13, numberOfEntries);
		}

		TEST_METHOD(HashMapComparisonOperatorTest)
		{
			HashMap<int, int> hashMap1, hashMap2;
			for (int i = 0; i < 5; ++i)
			{
				hashMap1.Insert(std::make_pair(i, i));
				hashMap2.Insert(std::make_pair(i, i));
			}
			Assert::IsTrue(hashMap2 == hashMap1);

			hashMap1.Remove(3);
			Assert::IsFalse(hashMap2 == hashMap1);
		}
	private:
		_CrtMemState initialMemoryState;
	};
}