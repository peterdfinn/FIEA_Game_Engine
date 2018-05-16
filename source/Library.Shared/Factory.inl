namespace LibraryShared
{
	template <typename T>
	Factory<T>::Iterator::Iterator(typename HashMap<std::string, Factory*>::Iterator hashMapIterator) : mHashMapIterator(hashMapIterator)
	{}

	template <typename T>
	typename Factory<T>::Iterator&
		Factory<T>::Iterator::operator=(const Iterator& other)
	{
		if (this != &other)
		{
			mHashMapIterator = other.mHashMapIterator;
		}
		return *this;
	}

	template <typename T>
	typename Factory<T>::Iterator&
		Factory<T>::Iterator::operator=(Iterator&& other)
	{
		if (this != &other)
		{
			mHashMapIterator = move(other.mHashMapIterator);
		}
		return *this;
	}

	template <typename T>
	typename Factory<T>::Iterator& Factory<T>::Iterator::operator++()
	{
		++mHashMapIterator;
		return *this;
	}

	template <typename T>
	typename Factory<T>::Iterator Factory<T>::Iterator::operator++(int)
	{
		Iterator copy = *this;
		operator++();
		return copy;
	}

	template <typename T>
	Factory<T>& Factory<T>::Iterator::operator*()
	{
		if (mHashMapIterator == mConcreteFactories.end())
		{
			throw exception("Cannot dereference the end iterator!");
		}
		return *mHashMapIterator->second;
	}

	template <typename T>
	const Factory<T>& Factory<T>::Iterator::operator*() const
	{
		return const_cast<Factory::Iterator*>(this)->operator*();
	}

	template <typename T>
	bool Factory<T>::Iterator::operator==
		(const typename Factory<T>::Iterator& other) const
	{
		return mHashMapIterator == other.mHashMapIterator;
	}

	template <typename T>
	bool Factory<T>::Iterator::operator!=
		(const typename Factory<T>::Iterator& other) const
	{
		return !operator==(other);
	}
	
	template <typename T>
	HashMap<std::string, Factory<T>*> Factory<T>::mConcreteFactories;

	template <typename T>
	bool Factory<T>::Add(Factory& factoryToAdd)
	{
		bool alreadyPresent = true;
		mConcreteFactories.Insert(
			make_pair(factoryToAdd.ClassName(), &factoryToAdd), alreadyPresent);
		return !alreadyPresent;
	}

	template <typename T>
	bool Factory<T>::Remove(Factory& factoryToRemove)
	{
		return mConcreteFactories.Remove(factoryToRemove.ClassName());
	}

	template <typename T>
	Factory<T>* Factory<T>::Find(const std::string& className)
	{
		typename HashMap<string, Factory*>::Iterator hashMapIt = mConcreteFactories.Find(className);
		Factory* returnPtr = nullptr;
		if (hashMapIt != mConcreteFactories.end())
		{
			returnPtr = hashMapIt->second;
		}
		return returnPtr;
	}

	template <typename T>
	T* Factory<T>::Create(const std::string& className)
	{
		Factory<T>* ptrToConcreteFactory = Find(className);
		if (ptrToConcreteFactory == nullptr)
		{
			return nullptr;
		}
		return ptrToConcreteFactory->Create();
	}

	template <typename T>
	typename Factory<T>::Iterator Factory<T>::begin()
	{
		Iterator i(mConcreteFactories.begin());
		return i;
	}

	template <typename T>
	typename Factory<T>::Iterator Factory<T>::end()
	{
		return Iterator(mConcreteFactories.end());
	}
}