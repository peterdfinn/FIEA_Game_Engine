#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(EventUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		
	public:
		TEST_METHOD_INITIALIZE(EventUnitTestInit)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(EventUnitTestCleanup)
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

		TEST_METHOD(EventConstructorTest)
		{
			/*When I construct an Event, I expect that the argument I pass in
			be stored in the EventArgs data member, and that it be obtainable
			again via a call to RawMessage. I also expect that the EventArgs
			member be of type EventArgs<T>.*/
			Datum rawMessage;
			Event<Datum> e(rawMessage);
			Assert::IsTrue(rawMessage == e.Message());
			Assert::IsTrue(Event<Datum>::TypeIdClass() ==
				e.TypeIdInstance());
		}

		TEST_METHOD(EventDeliverTest)
		{
			DatumSubscriber sub, sub3;
			DatumSubscriber sub2;

			Event<Datum>::Subscribe(sub);
			Event<Datum>::Subscribe(sub2);
			Datum datum;
			const Datum* const datumAddr = &datum;
			datumAddr;
			Event<Datum> event(datum);
			const Event<Datum>* const eventAddr = &event;
			eventAddr;
			event.Deliver();

			Assert::IsNotNull(sub.GetEvent());
			Assert::IsNotNull(sub2.GetEvent());
			bool sub2Check = sub2.TypeCheck();
			Assert::IsTrue(sub2Check);
			Assert::IsTrue(sub.TypeCheck());
			Assert::IsFalse(sub3.TypeCheck());
			Assert::IsNull(sub3.GetEvent());

			Event<Datum>::UnsubscribeAll();
		}

		TEST_METHOD(EventPublisherDeliverTest)
		{
			DatumSubscriber sub, sub2, sub3;
			Event<Datum>::Subscribe(sub);
			Event<Datum>::Subscribe(sub2);
			Datum datum;
			Event<Datum> event(datum);
			EventPublisher& publisher = event;
			publisher.Deliver();
			Assert::IsTrue(sub2.TypeCheck());
			Assert::IsTrue(sub.TypeCheck());
			Assert::IsFalse(sub3.TypeCheck());
			Assert::IsTrue(sub.GetEvent() == &event);
			Assert::IsTrue(sub2.GetEvent() == &event);
			Assert::IsNull(sub3.GetEvent());

			Event<Datum>::UnsubscribeAll();
		}

		TEST_METHOD(EventUnsubscribeTest)
		{
			DatumSubscriber sub, sub2;
			Event<Datum>::Subscribe(sub);
			Event<Datum>::Subscribe(sub2);
			Datum datum1, datum2;
			Event<Datum> event1(datum1), event2(datum2);
			event1.Deliver();

			Assert::IsTrue(sub.GetEvent() == &event1);
			Assert::IsTrue(sub2.GetEvent() == &event1);
			Event<Datum>::Unsubscribe(sub2);
			event2.Deliver();
			Assert::IsTrue(sub.GetEvent() == &event2);
			Assert::IsTrue(sub2.GetEvent() == &event1);

			Event<Datum>::UnsubscribeAll();
		}

		TEST_METHOD(EventMessageTest)
		{
			Datum datum;
			Event<Datum> event(datum);
			Assert::IsTrue(event.Message() == datum);
		}

		TEST_METHOD(EventPublisherRTTITest)
		{
			Event<int32_t> eint(5);
			Event<float> efloat(5.5f);
			Assert::IsTrue(eint.Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(efloat.Is("EventPublisher"s));
		}

		TEST_METHOD(EventPublisherSetTimeTest)
		{
			Event<int32_t> eint(1);
			GameTime gameTime;
			GameClock clock;
			clock.UpdateGameTime(gameTime);
			auto time1 = gameTime.CurrentTime();
			std::chrono::milliseconds delay(1000);
			eint.SetTime(gameTime, delay);
			Assert::IsTrue(time1 == eint.TimeEnqueued());
			Assert::IsTrue(eint.Delay() == delay);
		}

		TEST_METHOD(EventPublisherIsExpiredTest)
		{
			GameTime gameTime;
			GameClock clock;
			clock.UpdateGameTime(gameTime);
			Event<float> e(-1.0f);
			e.SetTime(gameTime);
			clock.UpdateGameTime(gameTime);
			Assert::IsTrue(e.IsExpired(gameTime));
			e.SetTime(gameTime, chrono::milliseconds(1000));
			clock.UpdateGameTime(gameTime);
			Assert::IsFalse(e.IsExpired(gameTime));
		}
	};
}