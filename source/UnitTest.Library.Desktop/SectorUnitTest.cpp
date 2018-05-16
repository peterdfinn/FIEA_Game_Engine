#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(SectorUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		World dummyWorld;
		Sector dummySector;
		Entity dummyEntity;
		ConcreteEntity dummyConcreteEntity;
		
	public:
		TEST_METHOD_INITIALIZE(SectorUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(SectorUnitTestCleanup)
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

		TEST_METHOD(SectorConstructorTest)
		{
			World w("w"s);
			Assert::AreEqual(0u, w.Find("Sectors"s)->Size());
			Sector* s = new Sector("goodname", &w);
			Assert::AreEqual(1u, w.Find("Sectors"s)->Size());
			Assert::IsTrue(s->Name() == "goodname");
			Assert::IsTrue(s->GetWorld() == &w);
		}

		TEST_METHOD(SectorMoveConstructorTest)
		{
			/*I expect the move constructor to move the name from the original
			and have the same World as the original.*/
			World world("world"s);
			Sector* sector = new Sector("sector"s, &world);
			Entity* entity = new Entity("entity"s, sector);
			UNREFERENCED_PARAMETER(entity);
			Sector* sectorMove = new Sector(move(*sector));
			sectorMove;
			Assert::IsTrue(sectorMove->Name() == "sector"s);
			Assert::IsTrue(sectorMove->GetWorld() == &world);
			Assert::IsTrue(sector->Name().empty());
			Assert::IsTrue(&world == sector->GetWorld());
			
			delete sector;
		}

		TEST_METHOD(SectorMoveAssignmentOperatorTest)
		{
			/*I expect the move assignment operator to behave just like the move
			constructor.*/
			World world("world"s);
			Sector* sectorOriginal = new Sector("sectorOriginal"s, &world);
			Entity* entityOriginal = new Entity("entityOriginal"s, sectorOriginal);
			//entityOriginal will now be owned by sectorMove.
			UNREFERENCED_PARAMETER(entityOriginal);
			Sector* sectorMove = new Sector("sectorMove"s, &world);
			//sectorMove will now be owned by world.
			Entity* entityMove = new Entity("entityMove"s, sectorMove);
			//entityMove is now owned by sectorMove, and it will be deleted in the move assignment operator
			UNREFERENCED_PARAMETER(entityMove);
			*sectorMove = move(*sectorOriginal);
			Assert::IsTrue(sectorMove->Name() == "sectorOriginal"s);
			Assert::IsTrue(sectorMove->GetWorld() == &world);
			string nameOfSector = sectorOriginal->Name();
			Assert::IsTrue(sectorOriginal->Name().empty());
			Assert::IsTrue(&world == sectorOriginal->GetWorld());

			delete sectorOriginal;

		}
		
		TEST_METHOD(SectorSetWorldTest)
		{
			/*I expect that a call to SetWorld will update both the Sector for
			which the method was called and its parent World. There are two
			cases to test: setting a Sector from nullptr, or setting a Sector
			from a non-null Sector pointer.*/

			Sector* s1 = new Sector("s1");

			//null to non-null
			World w1;
			s1->SetWorld(w1);
			Assert::IsTrue(&w1 == s1->GetWorld());
			Datum* w1SectorsDatum = w1.Find("Sectors"s);
			Assert::AreEqual(1u, w1SectorsDatum->Size());
			Assert::IsTrue(&w1SectorsDatum->GetScope(0u) == s1);

			//non-null to non-null
			World w2, w3;
			Sector* s2 = new Sector("s2", &w2);
			s2->SetWorld(w3);
			Assert::IsNull(w2.Find(s2->Name()));
			Assert::IsTrue(s2->GetWorld() == &w3);
			Datum* w2SectorsDatum = w2.Find("Sectors"s);
			Datum* w3SectorsDatum = w3.Find("Sectors"s);
			Assert::AreEqual(0u, w2SectorsDatum->Size());
			Assert::AreEqual(1u, w3SectorsDatum->Size());
			Assert::IsTrue(&w3SectorsDatum->GetScope(0u) == s2);
		}

		TEST_METHOD(SectorCreateEntityTest)
		{
			ConcreteEntityFactory cef;
			Factory<Entity>::Add(cef);

			Sector sector("sector"s);
			Entity* const concreteEntity1 = sector.CreateEntity("ConcreteEntity", "concreteEntity1");
			Assert::IsTrue(concreteEntity1->Is(ConcreteEntity::TypeIdClass()));
			Assert::IsTrue(concreteEntity1->GetSector() == &sector);

			Factory<Entity>::Remove(cef);
		}
	};
}