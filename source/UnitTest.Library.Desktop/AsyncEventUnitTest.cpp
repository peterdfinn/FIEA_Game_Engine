#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(AsyncEventUnitTest)
	{
	private:
		_CrtMemState initialMemoryState;
		const int32_t numberOfSubscribers = 500;
		const int32_t numberOfEvents = 400;
	public:
		TEST_METHOD_INITIALIZE(AsyncEventUnitTestInit)
		{
#ifdef _DEBUG
			Event<float> dummyFloatEvent(0.0f);
			Event<int32_t> dummyIntEvent(0);
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&initialMemoryState);
#endif
		}

		TEST_METHOD_CLEANUP(AsyncEventUnitTestCleanup)
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

		TEST_METHOD(AsyncEventSubscriberEnqueueTest)
		{
			/*This method tests whether EventSubscribers that then subscribe
			events can successfully do what they need to do without causing
			data race problems.*/
			/*Let's start by putting a large number of Event<int>s and
			Event<float>s in the queue.*/
			const int32_t finalNumberOfEvents = numberOfSubscribers * numberOfEvents;

			EventQueue eventQueue;
			eventQueue.Reserve(finalNumberOfEvents);
			GameTime gameTime;
			GameClock clock;
			clock.UpdateGameTime(gameTime);
			for (int32_t i = 0; i < numberOfEvents; ++i)
			{
				eventQueue.Enqueue(Event<float>(static_cast<float>(-i)), gameTime);
				eventQueue.Enqueue(Event<int32_t>(-i), gameTime);
			}

			/*Now, let's create some EventSubscriberEnqueue objects, and
			subscribe them to Event<float>s.*/
			std::vector<EventSubscriberEnqueue> subs;
			subs.reserve(numberOfSubscribers);
			for (int32_t i = 0; i < numberOfSubscribers; ++i)
			{
				subs.emplace_back(&gameTime, &eventQueue);
			}
			for (EventSubscriber& sub : subs)
			{
				Event<float>::Subscribe(sub);
			}


			/*Now, if we call Update on the EventQueue, what should happen is
			that the Event<int>s will simply be removed, as there are no
			subscribers to Event<int>s, and for each of the 500 Event<float>s,
			it will be received by one of 1000 subscribers, each of which
			enqueues one Event<float>. There should be a total of 500,000
			Event<float>s in the queue after a call to Update.*/
			clock.UpdateGameTime(gameTime);
			eventQueue.Update(gameTime);

			Assert::AreEqual(static_cast<size_t>(finalNumberOfEvents), eventQueue.Size());

			Event<float>::UnsubscribeAll();
			Event<int32_t>::UnsubscribeAll();
		}

		TEST_METHOD(AsyncEventSelfUnsubscribeTest)
		{
			/*We need to create a bunch of different Events of various types
			whose subscribers unsubscribe themselves upon being Notified.*/
			/*Let's start by creating a bunch of events of type float and
			int32_t and enqueueing them.*/
			EventQueue eventQueue;
			eventQueue.Reserve(numberOfEvents);
			GameTime gameTime;
			GameClock clock;
			clock.UpdateGameTime(gameTime);
			eventQueue.Enqueue(Event<float>(-1.0f), gameTime);
			eventQueue.Enqueue(Event<int32_t>(-1), gameTime);

			/*Now let's create a bunch of EventSubscriberUnsubscribe objects
			and subscribe them to either Event<float> or Event<int32_t>.*/
			std::vector<EventSubscriberUnsubscribe> subs;
			subs.reserve(numberOfSubscribers);
			
			for (int32_t i = 0; i < numberOfSubscribers; ++i)
			{
				subs.emplace_back(i % 2 == 0);
			}
			for (EventSubscriberUnsubscribe& sub : subs)
			{
				sub.SubscribeSelf();
			}

			/*Now, if we update the EventQueue, then both Event<int32_t> and
			Event<float> should have no subscribers, and memory shouldn't leak,
			event if we don't call UnsubscribeAll before exiting.*/
			clock.UpdateGameTime(gameTime);
			eventQueue.Update(gameTime);
		}
	};
}