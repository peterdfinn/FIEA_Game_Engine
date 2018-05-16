namespace LibraryShared
{
	template <typename T>
	SList<T>::Iterator::Iterator() : mOwner(nullptr), mNode(nullptr)
	{}

	template <typename T>
	SList<T>::Iterator::Iterator(Iterator&& other) : mOwner(other.mOwner),
		mNode(other.mNode)
	{
		other.mNode = nullptr;
		other.mOwner = nullptr;
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(Iterator&& other)
	{
		if (this != &other)
		{
			mNode = other.mNode;
			mOwner = other.mOwner;
			other.mNode = nullptr;
			other.mOwner = nullptr;
		}
		return *this;
	}

	template <typename T>
	SList<T>::Node::Node(const T& newData, Node* newNext)
		: mData(newData), mNext(newNext)
	{}

	template <typename T>
	SList<T>::SList() : mFront(nullptr), mBack(nullptr), mSize(0u)
	{}

	template <typename T>
	SList<T>::SList(const SList& other) : mFront(nullptr), mBack(nullptr),
		mSize(0u)
	{
		operator=(other);
	}

	template <typename T>
	SList<T>::SList(SList&& other) : mFront(other.mFront), mBack(other.mBack),
		mSize(other.mSize)
	{
		other.mSize = 0u;
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	void SList<T>::PushFront(const T& newItem)
	{
		++mSize;
		Node* const newNodePtr = new Node(newItem, mFront);
		mFront = newNodePtr;
		if (mBack == nullptr)
		{
			mBack = newNodePtr;
		}
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		if (mSize == 0u)
		{
			throw std::exception("Linked list is empty");
		}
		if (--mSize == 0u)
		{
			mBack = nullptr;
		}
		Node* const tempFront = mFront;
		mFront = tempFront->mNext;
		delete tempFront;
	}

	template <typename T>
	void SList<T>::PushBack(const T& newItem)
	{
		++mSize;
		Node* const newNodePtr = new Node(newItem);
		if (mBack != nullptr)
		{
			mBack->mNext = newNodePtr;
		}
		mBack = newNodePtr;
		if (mFront == nullptr)
		{
			mFront = newNodePtr;
		}
	}

	template <typename T>
	bool SList<T>::IsEmpty() const
	{
		return (mSize == 0u);
	}

	template <typename T>
	const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::exception("Linked list is empty");
		}
		return mFront->mData;
	}

	template <typename T>
	T& SList<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::exception("Linked list is empty");
		}
		return mFront->mData;
	}

	template <typename T>
	const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::exception("Linked list is empty");
		}
		return mBack->mData;
	}

	template <typename T>
	T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::exception("Linked list is empty");
		}
		return mBack->mData;
	}

	template <typename T>
	unsigned int SList<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			/* Once we've established that no self-assignment is being done,
			 * our first step is to clear this SList. */
			Clear();

			/* Now we have an empty list, and we can start calling PushBack on
			 * the items in other. */
			for (Node* currentNode = other.mFront; currentNode != nullptr;
				currentNode = currentNode->mNext)
			{
				PushBack(currentNode->mData);
			}
		}
		return *this;
	}

	template <typename T>
	void SList<T>::Clear()
	{
		while (mSize > 0u)
		{
			PopFront();
		}
	}

	template <typename T>
	SList<T>::Iterator::Iterator(const SList* ownerList, Node* ownedNode)
		: mOwner(ownerList), mNode(ownedNode)
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Owner of iterator cannot be nullptr!");
		}

		bool nodeIsInThisList = false;
		for (const Node* n = mOwner->mFront; n != nullptr; n = n->mNext)
		{
			if (n == ownedNode)
			{
				nodeIsInThisList = true;
				break;
			}
		}
		if (!nodeIsInThisList && (ownedNode != nullptr))
		{
			throw std::exception("The input node is not in the input list!");
		}
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return (mOwner == other.mOwner) && (mNode == other.mNode);
	}

	template <typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return !operator==(other);
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mNode == nullptr)
		{
			throw std::exception("This iterator cannot be incremented: it is \
				already past the end of the list!");
		}
		mNode = mNode->mNext;
		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator copyOfThis = *this;
		operator++();
		return copyOfThis;
	}

	template <typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::exception("Cannot dereference an iterator that points \
				past the end of the list!");
		}
		return mNode->mData;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin() const
	{
		Iterator iter(this, mFront);
		return iter;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end() const
	{
		Iterator iter(this);
		return iter;
	}

	template <typename T>
	void SList<T>::InsertAfter(const T& newItem, const Iterator& afterThis)
	{
		if (afterThis.mNode != nullptr)
		{
			afterThis.mNode->mNext = new Node(newItem, afterThis.mNode->mNext);
			++mSize;
		}
		else
		{
			PushBack(newItem);
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value) const
	{
		Node* const nodeBefore = FindBefore(value);
		if (nodeBefore == nullptr)
		{
			throw std::exception("The value is not in the list!");
		}
		else if (nodeBefore->mData == value)
		{
			Iterator it(this, nodeBefore);
			return it;
		}
		else
		{
			Iterator it(this, nodeBefore->mNext);
			return it;
		}
	}

	template <typename T>
	bool SList<T>::Remove(const T& value)
	{
		Node* nodeBefore = FindBefore(value);
		if (nodeBefore == nullptr)
		{
			return false;
		}
		else if (nodeBefore->mData == value)
		{
			const Node* const tempFront = mFront;
			mFront = nodeBefore->mNext;
			if (mBack == tempFront)
			{
				mBack = nullptr;
			}
			delete tempFront;
			--mSize;
			return true;
		}
		else
		{
			const Node* const toRemove = nodeBefore->mNext;
			if (toRemove == mBack)
			{
				mBack = nodeBefore;
			}
			nodeBefore->mNext = toRemove->mNext;
			delete toRemove;
			--mSize;
			return true;
		}
	}

	template <typename T>
	typename SList<T>::Node* SList<T>::FindBefore(const T& value) const
	{
		if (mFront == nullptr)
		{
			return nullptr;
		}
		else if (mFront->mData == value)
		{
			return mFront;
		}
		else
		{
			Iterator it = begin();
			while ((it.mNode->mNext != nullptr) && (it.mNode->mNext->mData != value))
			{
				++it;
			}
			if (it.mNode == mBack)
			{
				return nullptr;
			}
			else
			{
				return it.mNode;
			}
		}
	}
}