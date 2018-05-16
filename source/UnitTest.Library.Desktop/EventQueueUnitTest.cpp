#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(EventQueueUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
	public:
		TEST_METHOD_INITIALIZE(EventQueueUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(EventQueueUnitTestCleanup)
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

		TEST_METHOD(EventQueueEnqueueTest)
		{
			/*After enqueuing an Event, I expect that its size will have
			increased by 1 and that causing all events in the queue to be
			delivered will result in all subscribers being notified.*/
			DatumSubscriber sub;
			Datum datum;
			Event<Datum> event(datum);
			GameTime gameTime;
			EventQueue eq;
			shared_ptr<const EventPublisher> ptrToEvent =
				eq.Enqueue<Datum>(event, gameTime);
			Assert::AreEqual(static_cast<size_t>(1), eq.Size());
			Event<Datum>::Subscribe(sub);
			eq.Send(*ptrToEvent);
			Assert::IsTrue(sub.GetEvent() == ptrToEvent.get());

			Event<Datum>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueCapacityTest)
		{
			EventQueue eq;
			eq.Reserve(10u);
			Assert::AreEqual(static_cast<size_t>(10), eq.Capacity());
		}

		TEST_METHOD(EventQueueUpdateTest)
		{
			/*When I update the EventQueue, I expect that all expired Events be
			delivered, and that all non-expired Events remain there.*/
			EventQueue eq;
			Datum datum1, datum2;
			DatumSubscriber sub1;
			Event<Datum> event1(datum1), event2(datum2);
			Event<Datum>::Subscribe(sub1);
			GameClock clock;
			GameTime gameTime;
			clock.UpdateGameTime(gameTime);
			shared_ptr<const EventPublisher> ptrToEvent1 = eq.Enqueue<Datum>(
				event1, gameTime);
			eq.Enqueue<Datum>(event2, gameTime,
				chrono::milliseconds(100000000));
			clock.UpdateGameTime(gameTime);
			eq.Update(gameTime);
			Assert::AreEqual(static_cast<size_t>(1), eq.Size());
			Assert::IsTrue(sub1.GetEvent() == ptrToEvent1.get());

			clock.UpdateGameTime(gameTime);
			eq.Update(gameTime);
			Assert::AreEqual(static_cast<size_t>(1), eq.Size());
			Assert::IsTrue(sub1.GetEvent() == ptrToEvent1.get());

			eq.Clear();
			Event<Datum>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueIsEmptyTest)
		{
			EventQueue eq;
			Assert::IsTrue(eq.IsEmpty());
			Datum datum;
			Event<Datum> event(datum);
			GameTime gameTime;
			eq.Enqueue(event, gameTime);
			Assert::IsFalse(eq.IsEmpty());
			eq.Clear();
		}
	};
}