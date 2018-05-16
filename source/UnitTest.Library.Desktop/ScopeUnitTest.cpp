#include "pch.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_CXX98

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;
using namespace glm;

namespace UnitTest
{
	TEST_CLASS(ScopeUnitTest)
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

		TEST_METHOD(ScopeDefaultConstructorTest)
		{
			Scope s;
			Assert::IsTrue(s.GetParent() == nullptr);
			Assert::AreEqual(0u, s.Size());
		}

		TEST_METHOD(ScopeDestructorTest)
		{
			Scope grandparent;
			Scope& parent1 = grandparent.AppendScope("parent1");
			Scope& parent2 = grandparent.AppendScope("parent2");
			parent1.AppendScope("child1");
			parent1.AppendScope("child2");
			parent2.AppendScope("child3");
			parent2.AppendScope("child4");
			//destructor should not leak memory or cause any other problems
		}

		TEST_METHOD(ScopeCopyConstructorTest)
		{
			Scope grandparent;
			Scope& parent1 = grandparent.AppendScope("parent1");
			Scope& parent2 = grandparent.AppendScope("parent2");
			parent1.AppendScope("child1");
			parent1.AppendScope("child2");
			parent2.AppendScope("child3");
			parent2.AppendScope("child4");
			Scope grandparentCopy(grandparent);
			//should not cause memory leaks
		}

		TEST_METHOD(ScopeMoveConstructorTest)
		{
			Scope grandparent;
			grandparent.Append("name").Set(1);
			Scope& parent1 = grandparent.AppendScope("parent1");
			
			Scope& parent2 = grandparent.AppendScope("parent2");
			parent1.AppendScope("child1");
			parent1.AppendScope("child2");
			parent2.AppendScope("child3");
			parent2.AppendScope("child4");
			

			Scope grandparentMove = move(grandparent);
			//should not cause memory leaks
		}

		TEST_METHOD(ScopeCopyAssignmentOperatorTest)
		{
			Scope grandparent;
			Scope& parent1 = grandparent.AppendScope("parent1");
			Scope& parent2 = grandparent.AppendScope("parent2");
			parent1.AppendScope("child1");
			parent1.AppendScope("child2");
			parent2.AppendScope("child3");
			parent2.AppendScope("child4");

			Scope grandparentCopy;
			grandparentCopy = grandparent;
			//should not cause memory leaks
		}

		TEST_METHOD(ScopeMoveAssignmentOperatorTest)
		{
			Scope grandparent;
			Scope& parent1 = grandparent.AppendScope("parent1");
			Scope& parent2 = grandparent.AppendScope("parent2");
			parent1.AppendScope("child1");
			parent1.AppendScope("child2");
			parent2.AppendScope("child3");
			parent2.AppendScope("child4");

			Scope grandparentMove;
			grandparentMove = move(grandparent);
			//should not cause memory leaks
		}

		TEST_METHOD(ScopeFindTest)
		{
			Scope s;
			s.Append("name");
			Assert::AreEqual(1u, s.Size());
			Assert::IsTrue(s.Find("name") != nullptr);
			Assert::IsTrue(s.Find("nae") == nullptr);
		}

		TEST_METHOD(ScopeSearchTest)
		{
			Scope grandparent;
			Scope* grandparentAddr = &grandparent;
			grandparentAddr;
			Scope* parent = new Scope();
			Scope* child = new Scope();
			grandparent.Adopt("parent", *parent);
			parent->Adopt("child", *child);
			Datum& grandparentFloat = grandparent.Append("grandparentFloat");
			grandparentFloat.PushBack(-10.1021f);
			Scope* shouldPointToGrandparent;
			Assert::IsTrue(child->Search("grandparentFloat", &shouldPointToGrandparent) == grandparent.Find("grandparentFloat"));
			Assert::IsTrue(shouldPointToGrandparent == &grandparent);
			Assert::IsTrue(child->Search("badName") == nullptr);

			//delete parent;
			//delete child;
		}

		TEST_METHOD(ScopeAppendTest)
		{
			Scope s;
			auto lambda = [&s]() { s.Append(""); };
			Assert::ExpectException<exception>(lambda);

			Datum& d = s.Append("name");
			d.PushBack(1);
			Assert::AreEqual(1, s.Append("name").GetInt());
		}

		TEST_METHOD(ScopeAppendScopeTest)
		{
			Scope s;
			s.AppendScope("name");
			auto lambda = [&s]() { s.AppendScope("name"); };
			Assert::ExpectException<exception>(lambda);
		}

		TEST_METHOD(ScopeAdoptTest)
		{
			Scope parent;
			Scope* child = new Scope();
			parent.Append("var").PushBack(10);
			auto lambda = [&parent, &child]() { parent.Adopt("var", *child); };
			Assert::ExpectException<exception>(lambda);
			delete child;
		}

		TEST_METHOD(ScopeEqualityOperatorTest)
		{
			Scope s1, s2;
			s1.Append("var").PushFront(0);
			s2.Append("var").PushBack(0);
			Assert::IsTrue(s1 == s2);
			s2.Find("var")->Set(12);
			Assert::IsFalse(s1 == s2);
		}

		TEST_METHOD(ScopeInequalityOperatorTest)
		{
			Scope s1, s2;
			s1.Append("var").PushFront(0);
			s2.Append("var").PushBack(0);
			Assert::IsFalse(s1 != s2);
			s2.Find("var")->Set(12);
			Assert::IsTrue(s1 != s2);
		}

		TEST_METHOD(ScopeEqualsTest)
		{
			Scope s;
			Foo f;
			Assert::IsFalse(s.Equals(&f));
		}

		TEST_METHOD(ScopeFindNameTest)
		{
			Scope s;
			Assert::IsTrue(s.FindName(nullptr) == "");
		}

		TEST_METHOD(ScopeBracketsOperatorTest)
		{
			Scope s;
			Datum d1 = s["var1"];
			Datum d2 = s["var2"];
			Datum d3 = s["var3"];
			Assert::IsTrue(d1 == s[0]);
			Assert::IsTrue(d2 == s[1]);
			Assert::IsTrue(d3 == s[2]);
		}
	};
}