#include "pch.h"
using namespace std;
using namespace LibraryShared;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTest
{
	TEST_CLASS(MiscellaneousSimplifiedTests)
	{
	public:
		TEST_METHOD(AttributedTest)
		{
			Attributed att(1u);
			//Assert::IsTrue(att.IsPrescribedAttribute("this"));
			//AttributedFoo attfoo;
			//uint64_t attfooid = AttributedFoo::TypeIdClass();
			//Assert::IsTrue(attfoo.IsPrescribedAttribute("integerAttribute"));
			//Assert::IsTrue(attfoo.IsPrescribedAttribute("this"));
			//Assert::IsTrue(attfoo.IsPrescribedAttribute("floatAttribute"));
		}

		TEST_METHOD(RTTItest)
		{
			Attributed att;
			AttributedFoo foo;
			Assert::IsTrue(att.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(foo.Is(AttributedFoo::TypeIdClass()));
			Assert::IsFalse(att.Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(foo.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(foo.Is(Scope::TypeIdClass()));
		}

		TEST_METHOD(ScopeCopyConstructorTest)
		{
			Scope parent(1u);
			uint32_t size = parent.Size();
			parent.AppendScope("child");
			size = parent.Size();
			Scope parentCopy(parent);
			size = parent.Size();
			size = parentCopy.Size();
			size;
		}

		TEST_METHOD(AuxiliaryAttributeTest)
		{
			EventMessageAttributed ema;
			ema.AppendAuxiliaryAttribute("aux"s);
			Event<EventMessageAttributed> e(ema);
			Event<EventMessageAttributed> copy(e);
			Assert::IsTrue(copy.Message().IsAttribute("aux"));
		}

		TEST_METHOD(EventSubscriberEnqueueTest)
		{
			/*This method tests whether EventSubscribers that then subscribe
			events can successfully do what they need to do without causing
			data race problems.*/
			/*Let's start by putting a large number of Event<int>s and
			Event<float>s in the queue.*/
			const int32_t numberOfSubscribers = 50;
			const int32_t numberOfEvents = 10;
			const int32_t finalNumberOfEvents = numberOfSubscribers * numberOfEvents;

			EventQueue eventQueue;
			eventQueue.Reserve(finalNumberOfEvents);
			GameTime gameTime;
			GameClock clock;
			clock.UpdateGameTime(gameTime);
			for (int32_t i = 0; i < numberOfEvents; ++i)
			{
				eventQueue.Enqueue(Event<float>(static_cast<float>(-i)), gameTime);
			}

			/*Now, let's create 1000 EventSubscriberEnqueue objects, and
			subscribe them to Event<float>s.*/
			std::vector<EventSubscriberEnqueue> subs;
			subs.reserve(numberOfSubscribers);
			for (int32_t i = 0; i < numberOfSubscribers; ++i)
			{
				EventSubscriber& sub = subs.emplace_back(&gameTime, &eventQueue);
				const EventSubscriber* subAddr = &sub;
				subAddr = subAddr;
			}
			for (EventSubscriber& sub : subs)
			{
				const EventSubscriber* const subAddr = &sub;
				subAddr;
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

			size_t eventQueueSize = eventQueue.Size();
			Assert::AreEqual(static_cast<size_t>(finalNumberOfEvents), eventQueueSize);
		}
	};
}