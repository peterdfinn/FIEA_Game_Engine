namespace LibraryShared
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>);

	template <typename T>
	Event<T>::Event(const T& rawMessage) : EventPublisher(staticSubscribers, staticSubscribersMutex),
		mMessage(rawMessage)
	{}

	template <typename T>
	const T& Event<T>::Message() const
	{
		return mMessage;
	}

	template <typename T>
	void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> guard(staticSubscribersMutex);
		if (staticSubscribers.find(&subscriber) == staticSubscribers.end())
		{
			staticSubscribers.insert(&subscriber);
		}
	}

	template <typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> guard(staticSubscribersMutex);
		staticSubscribers.erase(&subscriber);
	}

	template <typename T>
	void Event<T>::UnsubscribeAll()
	{
		std::lock_guard<std::mutex> guard(staticSubscribersMutex);
		staticSubscribers.clear();
	}

	template <typename T>
	std::set<EventSubscriber*> Event<T>::staticSubscribers;

	template<typename T>
	std::mutex Event<T>::staticSubscribersMutex;
}