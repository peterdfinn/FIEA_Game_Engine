#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(EntityUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		Entity dummyEntity;
		Sector dummySector;
	public:
		TEST_METHOD_INITIALIZE(EntityUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(EntityUnitTestCleanup)
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

		TEST_METHOD(EntityConstructorTest)
		{
			Sector s;
			Assert::AreEqual(0u, s.Find("Entities"s)->Size());
			Entity *e2 = new Entity("goodname"s, &s);
			uint32_t numberOfEntitiesInSector = s.Find("Entities"s)->Size();
			Assert::AreEqual(1u, numberOfEntitiesInSector);
			Assert::IsTrue(e2->GetParent()->Is(Sector::TypeIdClass()));
			Assert::IsTrue(e2->Name() == "goodname");
			Assert::IsTrue(e2->GetSector() == &s);
		}

		TEST_METHOD(EntityMoveConstructorTest)
		{
			/*I expect the move constructor to move the name from the original
			and have the same Sector as the original.*/
			Sector s;
			Entity* e = new Entity("e", &s);
			Entity* copy = new Entity(move(*e));
			Assert::IsTrue(copy->Name() == "e"s);
			Assert::IsTrue(&s == copy->GetSector());
			Assert::IsTrue(e->Name().empty());
			Assert::IsTrue(&s == e->GetSector());
			delete e;
		}

		TEST_METHOD(EntityMoveAssignmentOperatorTest)
		{
			/*I expect the move assignment operator to behave just like the move
			constructor.*/
			Sector s1, s2;
			Entity *e1 = new Entity("e1", &s1), *e2 = new Entity("e2", &s2);

			*e1 = move(*e2);

			Assert::IsTrue(e1->Name() == "e2");
			Assert::IsTrue(&s2 == e1->GetSector());
			Assert::IsTrue(e2->Name().empty());
			Assert::IsTrue(&s2 == e2->GetSector());

			delete e2;
		}

		TEST_METHOD(EntitySetNameTest)
		{
			Entity e("name1");
			e.SetName("name2");
			Assert::IsTrue(e.Name() == "name2");
		}
		
		TEST_METHOD(EntitySetSectorTest)
		{
			/*I expect that a call to SetSector will update both the Entity for
			which the method was called and its parent Sector. There are two
			cases to test: setting a Sector from nullptr, or setting a Sector
			from a non-null Sector pointer.*/
			
			Entity* e1 = new Entity("e1");

			//null to non-null
			Sector s1;
			e1->SetSector(s1);
			Assert::IsTrue(&s1 == e1->GetSector());
			Datum& s1EntitiesDatum = s1.Entities();
			Assert::AreEqual(1u, s1EntitiesDatum.Size());
			Assert::IsTrue(&s1EntitiesDatum.GetScope(0u) == e1);

			//non-null to non-null
			Sector s2, s3;
			Entity* e2 = new Entity("e2", &s2);
			e2->SetSector(s3);
			Assert::IsNull(s2.Find(e2->Name()));
			Assert::IsTrue(e2->GetSector() == &s3);
			Datum& s2EntitiesDatum = s2.Entities();
			Datum& s3EntitiesDatum = s3.Entities();
			Assert::AreEqual(0u, s2EntitiesDatum.Size());
			Assert::AreEqual(1u, s3EntitiesDatum.Size());
			Assert::IsTrue(&s3EntitiesDatum.GetScope(0u) == e2);
		}
	};
}