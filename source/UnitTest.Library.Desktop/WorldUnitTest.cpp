#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(WorldUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		World dummyWorld;
		Sector dummySector;
		Entity dummyEntity;
		ConcreteEntity dummyConcreteEntity;


	public:
		TEST_METHOD_INITIALIZE(WorldUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(WorldUnitTestCleanup)
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

		TEST_METHOD(WorldConstructorTest)
		{
			World world("world"s);
			Assert::AreEqual("world"s, world.Name());
			Assert::AreEqual(0u, world.Find("Sectors"s)->Size());
		}

		TEST_METHOD(WorldMoveConstructorTest)
		{
			/*I expect the move constructor to move the name from the original
			and have stolen all the data of the original, meaning its list of
			Sectors is empty.*/
			World original("original"s);
			Sector* sector = new Sector("sector"s, &original);
			UNREFERENCED_PARAMETER(sector);
			Assert::IsFalse(original.Sectors().IsEmpty());
			World worldMove(move(original));
			Assert::IsTrue(worldMove.Name() == "original"s);
			Assert::IsTrue(original.Name().empty());
			Assert::IsFalse(worldMove.Sectors().IsEmpty());
			auto lambda = [&original]() { original.Sectors(); };
			Assert::ExpectException<exception>(lambda);
		}

		TEST_METHOD(WorldMoveAssignmentOperatorTest)
		{
			/*I expect that this behave more or less like the move constructor;
			however, I also expect that the World on the left side of the
			assignment relinquish ownership of its children, if it has any.*/
			World lhsWorld("lhsWorld"s), rhsWorld("rhsWorld"s);
			Sector *lhsSector = new Sector("lhsSector"s, &lhsWorld),
				*rhsSector = new Sector("rhsSector"s, &rhsWorld);
			UNREFERENCED_PARAMETER(lhsSector);

			lhsWorld = move(rhsWorld);
			Assert::AreEqual("rhsWorld"s, lhsWorld.Name());
			Assert::IsTrue(rhsWorld.Name().empty());
			Assert::IsTrue(rhsSector->GetWorld() == &lhsWorld);
			auto lambda = [&rhsWorld]() { rhsWorld.Sectors(); };
			Assert::ExpectException<exception>(lambda);
			Assert::IsTrue(&lhsWorld.Sectors().GetScope(0u) == rhsSector);
		}

		TEST_METHOD(WorldCreateSectorTest)
		{

			World world("world"s);
			Sector* const sector = world.CreateSector("sector"s);
			Assert::IsTrue(sector->GetWorld() == &world);
			Assert::IsTrue(&world.Sectors().GetScope() == sector);
		}

		TEST_METHOD(ParsingWorldSectorEntityTest)
		{
			EntityFactory entityFactory;
			Factory<Entity>::Add(entityFactory);

			ifstream fileStream("Content\\WorldSectorEntityTestFile.txt");
			JsonScopeSharedData sharedData;
			JsonParseMaster master(&sharedData);
			JsonScopeParseHelper helper;
			master.AddHelper(helper);

			master.Parse(fileStream);

			fileStream.close();
			
			Scope& populatedScope = sharedData.GetScope();
			Datum* worldDatum = populatedScope.Find("testWorld");
			Scope& worldScope = worldDatum->GetScope(0u);
			Assert::IsTrue(worldScope.Is(World::TypeIdClass()));

			Factory<Entity>::Remove(entityFactory);
			master.RemoveHelper(helper);
		}
	};
}