namespace LibraryShared
{
	template <typename T>
	std::shared_ptr<const EventPublisher> EventQueue::Enqueue(const Event<T>& event, const GameTime& gameTime,
		std::chrono::milliseconds delay)
	{
		EventPublisher* const newEvent = new Event<T>(event);
		newEvent->SetTime(gameTime, delay);
		std::shared_ptr<const EventPublisher> ptrToEventCopy(newEvent);
		{
			std::lock_guard<std::mutex> guard(mQueueMutex);
			mQueue.push_back(ptrToEventCopy);
		}
		return ptrToEventCopy;
	}
}