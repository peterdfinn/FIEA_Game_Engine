namespace LibraryShared
{
	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>::Iterator::Iterator() : mOwner(nullptr),
		mBucketIndex(0u), mIteratorWithinBucket(SList<std::pair<TKey, TData>>::Iterator())
	{}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>::Iterator::Iterator(const Iterator& other)
		: mBucketIndex(other.mBucketIndex), mOwner(other.mOwner),
		mIteratorWithinBucket(other.mIteratorWithinBucket)
	{}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>::Iterator::Iterator(Iterator&& other)
		: mOwner(other.mOwner), mBucketIndex(other.mBucketIndex),
		mIteratorWithinBucket(std::move(other.mIteratorWithinBucket))
	{
		other.mOwner = nullptr;
		other.mBucketIndex = 0u;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator&
		HashMap<TKey, TData, TFunctor, TCompare>::Iterator::operator=(Iterator&& other)
	{
		if (this != &other)
		{
			mOwner = other.mOwner;
			mBucketIndex = other.mBucketIndex;
			mIteratorWithinBucket = std::move(other.mIteratorWithinBucket);
			other.mOwner = nullptr;
			other.mBucketIndex = 0u;
		}
		return *this;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>::Iterator::Iterator(HashMap* owner, uint32_t bucketIndex,
		ChainIterator iteratorWithinBucket) : mOwner(owner),
		mBucketIndex(bucketIndex), mIteratorWithinBucket(iteratorWithinBucket)
	{}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator& HashMap<TKey, TData, TFunctor, TCompare>::Iterator::operator++()
	{
		/*If this Iterator does not have an owning HashMap, complain.*/
		if (!mOwner)
		{
			throw std::exception("This iterator cannot be incremented: its\
				owner has not been initialized!");
		}

		/*If this iterator is the end iterator, complain.*/
		if (*this == mOwner->end())
		{
			throw std::exception("This iterator cannot be incremented; it is\
				the end iterator!");
		}

		/*If this Iterator is currently pointed to the last item in this
		bucket, then set this Iterator to point at the first item in the next
		non-empty bucket, or make it the end Iterator. Otherwise, simply point
		this Iterator at the next item in its current bucket.*/
		if (++mIteratorWithinBucket == mOwner->mBuckets[mBucketIndex].end())
		{
			//iterate through buckets, stopping if there are no more buckets or
			//if a non-empty bucket is encountered
			while ((++mBucketIndex < mOwner->mBuckets.Size()) &&
				(mOwner->mBuckets[mBucketIndex].IsEmpty()));

			if (mBucketIndex == mOwner->mBuckets.Size())
			{//reached the end of the vector of buckets
				mIteratorWithinBucket = ChainIterator();
			}
			else
			{//found a non-empty bucket
				mIteratorWithinBucket = mOwner->mBuckets[mBucketIndex].begin();
			}
		}

		return *this;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator HashMap<TKey, TData, TFunctor, TCompare>::Iterator::operator++(int)
	{
		Iterator copy = *this;
		operator++();
		return copy;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	std::pair<TKey, TData>& HashMap<TKey, TData, TFunctor, TCompare>::Iterator::operator*()
	{
		return *mIteratorWithinBucket;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	std::pair<TKey, TData>* HashMap<TKey, TData, TFunctor, TCompare>::Iterator::operator->()
	{
		return &(*mIteratorWithinBucket);
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	bool HashMap<TKey, TData, TFunctor, TCompare>::Iterator::operator==(const Iterator& other) const
	{
		return (mOwner == other.mOwner) && (mBucketIndex == other.mBucketIndex)
			&& (mIteratorWithinBucket == other.mIteratorWithinBucket);
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	bool HashMap<TKey, TData, TFunctor, TCompare>::Iterator::operator!=
		(const Iterator& other) const
	{
		return !operator==(other);
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>::HashMap(uint32_t numberOfBuckets) :
		mBuckets(numberOfBuckets)
	{
		if (numberOfBuckets == 0u)
		{
			throw std::exception("Cannot create a HashMap with 0 buckets!");
		}
		for (uint32_t i = 0u; i < numberOfBuckets; ++i)
		{
			mBuckets.PushBack(SList<PairType>());
		}
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>::HashMap(const HashMap& other)
	{
		operator=(other);
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>& HashMap<TKey, TData, TFunctor, TCompare>::operator=(const HashMap& other)
	{
		/*First thing we must do is check for self-assignment.*/
		if (&other != this)
		{
			/*Next, we must ensure this HashMap doesn't have any data hanging
			around.*/
			Clear();

			/*Now that we know this HashMap is empty, we can hash each of 
			other's objects into this one.*/
			for (auto& pair : other)
			{
				Insert(pair);
			}
		}

		return *this;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>::HashMap(HashMap&& other)
	{
		operator=(std::move(other));
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	HashMap<TKey, TData, TFunctor, TCompare>& HashMap<TKey, TData, TFunctor, TCompare>::operator=(HashMap&& other)
	{
		if (&other != this)
		{
			mBuckets = std::move(other.mBuckets);
			other.mBuckets.Reserve(mBuckets.Capacity());
			for (uint32_t i = 0u; i < mBuckets.Size(); ++i)
			{
				other.mBuckets.PushBack(SList<PairType>());
			}
		}
		return *this;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	bool HashMap<TKey, TData, TFunctor, TCompare>::
		operator==(const HashMap<TKey, TData, TFunctor, TCompare>& other) const
	{
		/*If the two HashMaps have different numbers of elements, return false.*/
		if (other.Size() != Size())
		{
			return false;
		}

		/*Otherwise, we must go through each pair in this HashMap and try to
		find it in the other HashMap. If this fails at any point, return false.
		Otherwise, return true.*/
		for (std::pair<TKey, TData> pair : *this)
		{
			auto findResult = other.Find(pair.first);
			if (findResult == end())
			{
				return false;
			}
			else
			{
				if (pair.second != findResult->second)
				{
					return false;
				}
			}
		}

		return true;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator HashMap<TKey, TData, TFunctor, TCompare>::Find(const TKey& key, uint32_t& hash) const
	{
		/*First, we must generate the hash value.*/
		TFunctor hashObject;
		auto numberOfBuckets = mBuckets.Size();
		hash = (hashObject(key) % numberOfBuckets);

		/*Assuming the hash is now valid, we can now iterate through the pairs
		in the appropriate bucket until we find one that contains the same
		key.*/
		auto& bucket = mBuckets[hash];
		TCompare compareObject;
		for (auto pairIt = bucket.begin(); pairIt != bucket.end(); ++pairIt)
		{
			if (compareObject((*pairIt).first, key))
			{
				Iterator it(const_cast<HashMap*>(this), hash, pairIt);
				return it;
			}
		}
		
		/*If we reach this point, that means there was no pair was found in
		the bucket. We must return the end iterator of this HashMap.*/
		return end();
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator HashMap<TKey, TData, TFunctor, TCompare>::Find(const TKey& key) const
	{
		uint32_t h;
		return Find(key, h);
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator HashMap<TKey, TData, TFunctor, TCompare>::Insert(const PairType& toInsert, bool& alreadyPresent)
	{
		uint32_t hash;
		Iterator iteratorWithinBucket = Find(toInsert.first, hash);

		/*Now that we have an Iterator that either refers to an existent pair
		with the same key or is the end Iterator, we can take action based on
		it.*/
		/*If the call to Find returned the end iterator, we push 'toInsert'
		into the bucket and return an Iterator to it.*/
		if (iteratorWithinBucket == end())
		{
			SList<PairType>& bucket = mBuckets[hash];
			bucket.PushBack(toInsert);
			uint32_t bucketSize = bucket.Size();
			alreadyPresent = false;
			auto tempIterator = bucket.begin();
			for (uint32_t i = 0u; i < bucketSize - 1u; ++i)
			{
				++tempIterator;
			}
			return Iterator(this, hash, tempIterator);
		}
		/*Otherwise, we return the iterator that refers to the pair we found.*/
		alreadyPresent = true;
		return iteratorWithinBucket;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator HashMap<TKey, TData, TFunctor, TCompare>::Insert(const PairType& toInsert)
	{
		bool ap;
		return Insert(toInsert, ap);
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	TData& HashMap<TKey, TData, TFunctor, TCompare>::operator[](const TKey& key)
	{
		uint32_t hash;
		Iterator it = Find(key, hash);
		
		/*After we've called Find, we have either an iterator that refers to
		the pair with key 'key' or the end iterator.*/
		/*If it's the end iterator, that means we need to insert a new pair
		that contains key 'key'.*/
		if (it == end())
		{
			auto& bucket = mBuckets[hash];
			bucket.PushFront(std::make_pair(key, TData()));
			return bucket.Front().second;
		}
		
		/*Otherwise, we have found the data value the user (presumably) wanted
		to find. We return it.*/
		return it->second;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	const TData& HashMap<TKey, TData, TFunctor, TCompare>::At(const TKey& key) const
	{
		return (const_cast<HashMap<TKey, TData, TFunctor, TCompare>*>(this))->At(key);
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	TData& HashMap<TKey, TData, TFunctor, TCompare>::At(const TKey& key)
	{
		uint32_t hash;
		Iterator it = Find(key, hash);
		if (it == end())
		{
			throw std::exception("There is no data entry in this HashMap with\
				this key!");
		}
		
		return it->second;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	bool HashMap<TKey, TData, TFunctor, TCompare>::Remove(const TKey& key)
	{
		uint32_t hash;
		Iterator it = Find(key, hash);
		if (it == end())
		{
			return false;
		}
		return mBuckets[hash].Remove(*it);
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	void HashMap<TKey, TData, TFunctor, TCompare>::Clear()
	{
		for (uint32_t i = 0u; i < mBuckets.Size(); ++i)
		{
			mBuckets[i].Clear();
		}
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	uint32_t HashMap<TKey, TData, TFunctor, TCompare>::Size() const
	{
		uint32_t sum = 0u;
		for (uint32_t i = 0u; i < mBuckets.Size(); ++i)
		{
			sum += mBuckets[i].Size();
		}
		return sum;
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	bool HashMap<TKey, TData, TFunctor, TCompare>::ContainsKey(const TKey& key) const
	{
		uint32_t hash;
		Iterator it = Find(key, hash);
		return (it != end());
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator HashMap<TKey, TData, TFunctor, TCompare>::begin() const
	{
		if (Size() == 0u)
		{
			return end();
		}
		else
		{
			/*Iterate through buckets to find first non-empty one, then return
			an Iterator that refers to its first element.*/
			uint32_t i = 0u;
			for (; i < mBuckets.Size(); ++i)
			{
				if (!mBuckets[i].IsEmpty())
				{
					break;
				}
			}
			Iterator it(const_cast<HashMap*>(this), i, mBuckets[i].begin());
			return it;
		}
	}

	template <typename TKey, typename TData, typename TFunctor, typename TCompare>
	typename HashMap<TKey, TData, TFunctor, TCompare>::Iterator HashMap<TKey, TData, TFunctor, TCompare>::end() const
	{
		return Iterator(const_cast<HashMap*>(this), mBuckets.Size());
	}
}