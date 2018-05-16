/// <summary>
/// This file contains all method implementations for the templated class
/// Vector<T> and its inner class Iterator.
/// </summary>

#include <iostream>

namespace LibraryShared
{
	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mIndex == rhs.mIndex);
	}

	template <typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !operator==(rhs);
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		/*If this iterator does not have an owning Vector, complain.*/
		if (mOwner == nullptr)
		{
			throw std::exception("This iterator cannot be incremented: it does \
				not belong to an extant Vector!");
		}

		/*Sanity check on index.*/
		assert(mIndex <= mOwner->mSize);

		/*Now we must increment the index, throwing an exception if this would
		cause this Iterator to go past the end Iterator.*/
		if (++mIndex > mOwner->mSize)
		{
			throw std::exception("This iterator cannot be incremented: it \
				already points past the end of the Vector!");
		}

		/*Finally, we can return.*/
		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator copyOfThis = *this;
		operator++();
		return copyOfThis;
	}

	template <typename T>
	const T& Vector<T>::Iterator::operator*() const
	{
		/*If the owner of this Iterator is not defined, complain.*/
		if (mOwner == nullptr)
		{
			throw std::exception("Cannot dereference this Iterator: it does \
				not have a defined owning Vector!");
		}

		/*Sanity checking the mIndex.*/
		assert(mIndex <= mOwner->mSize);

		/*If this is the end Iterator, complain.*/
		if (mIndex == mOwner->mSize)
		{
			throw std::exception("Cannot dereference this Iterator: it is the \
				end iterator!");
		}

		/*Finally, we can return.*/
		return mOwner->mData[mIndex];
	}

	template <typename T>
	T& Vector<T>::Iterator::operator*()
	{
		return const_cast<T&>(const_cast<const Vector<T>::Iterator*>(this)->
			operator*());
	}

	template <typename T>
	Vector<T>::Iterator::Iterator() : mOwner(nullptr), mIndex(0u)
	{
		std::cout << "Hello from Vector::Iterator default constructor!" << std::endl;
	}

	template <typename T>
	Vector<T>::Iterator::Iterator(const Vector* owner, uint32_t index) :
		mOwner(owner), mIndex(index)
	{}

	template <typename T>
	Vector<T>::Vector(uint32_t capacity) : mCapacity(capacity), mSize(0u)
	{
		if (mCapacity > 0u)
		{
			uint32_t sizeOfT = sizeof(T);
			mData = static_cast<T*>(malloc(mCapacity * sizeOfT));
		}
	}

	template <typename T>
	T& Vector<T>::operator[](uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("This index is invalid!");
		}

		return mData[index];
	}

	template <typename T>
	const T& Vector<T>::operator[](uint32_t index) const
	{
		return const_cast<const T&>(const_cast<Vector<T>*>(this)->operator[](index));
	}

	template <typename T>
	void Vector<T>::PopBack()
	{
		/*If the vector is empty, complain.*/
		if (mSize == 0u)
		{
			throw std::exception("Cannot PopBack: this vector is empty!");
		}

		/*Otherwise, simply destroy the last element and decrement the size.*/
		mData[--mSize].~T();
	}

	template <typename T>
	bool Vector<T>::IsEmpty() const
	{
		return (mSize == 0u);
	}

	template <typename T>
	T& Vector<T>::Front()
	{
		return operator[](0);
	}

	template <typename T>
	const T& Vector<T>::Front() const
	{
		return operator[](0);
	}

	template <typename T>
	T& Vector<T>::Back()
	{
		if (mSize == 0u)
		{
			throw std::exception("This Vector has no back; it is empty!");
		}
		return operator[](mSize - 1);
	}

	template <typename T>
	const T& Vector<T>::Back() const
	{
		if (mSize == 0u)
		{
			throw std::exception("This Vector has no back; it is empty!");
		}
		return operator[](mSize - 1);
	}

	template <typename T>
	uint32_t Vector<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	uint32_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template <typename T>
	T& Vector<T>::At(uint32_t index)
	{
		return operator[](index);
	}

	template <typename T>
	const T& Vector<T>::At(uint32_t index) const
	{
		return operator[](index);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::begin() const
	{
		return Iterator(this);
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::end() const
	{
		return Iterator(this, mSize);
	}

	template <typename T>
	void Vector<T>::PushBack(const T& item)
	{
		/*If the size is equal to the capacity, then before the new item can be
		pushed, the capacity must be increased.*/
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
			mCapacityIncreaseAmount *= 2;
		}

		/*Now that we know the vector is large enough to store the new item, we
		simply put it at the required memory address and increment the size.*/
		new (mData + mSize++) T(item);
	}

	template <typename T>
	Vector<T>::Vector(const Vector& rhs) : mSize(rhs.mSize),
		mCapacity(rhs.mCapacity),
		mData(static_cast<T*>(malloc(rhs.mCapacity * sizeof(T))))
	{
		uint32_t i = 0u;
		for (T& item : rhs)
		{
			new (mData + i++) T(item);
		}
	}

	template <typename T>
	Vector<T>::Vector(Vector&& rhs) : mSize(rhs.mSize),
		mCapacity(rhs.mCapacity), mData(rhs.mData)
	{
		rhs.mData = nullptr;
		rhs.mSize = 0u;
		rhs.mCapacity = 0u;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		Clear();
		Reserve(rhs.mCapacity);
		mSize = rhs.mSize;
		uint32_t i = 0u;
		for (T& item : rhs)
		{
			new (mData + i++) T(item);
		}
		return *this;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(Vector&& rhs)
	{
		Clear();
		free(mData);
		mCapacity = rhs.mCapacity;
		mSize = rhs.mSize;
		mData = rhs.mData;
		rhs.mData = nullptr;
		rhs.mSize = 0u;
		rhs.mCapacity = 0u;

		return *this;
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		Clear();
		free(mData);
	}

	template <typename T>
	bool Vector<T>::Reserve(uint32_t capacity)
	{
		/*Determining the capacity of the newly resized vector.*/
		uint32_t actualNewCapacity = capacity;
		bool returnValue = true;
		if (capacity < mSize)
		{
			actualNewCapacity = mSize;
			returnValue = false;
		}
		
		/*Moving over all members.*/
		T* newData = static_cast<T*>(malloc(actualNewCapacity * sizeof(T)));
		uint32_t bytesToMove = sizeof(T) * mSize;
		errno_t memmove_return = memmove_s(newData, bytesToMove, mData, bytesToMove);
		memmove_return;

		/*Freeing the old memory, pointing mData at the new memory, and
		updating mCapacity.*/
		if (mCapacity > 0u)
		{
			free(mData);
		}
		mData = newData;
		mCapacity = actualNewCapacity;

		return returnValue;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& item)
	{
		auto it = begin();
		for (; it != end(); ++it)
		{
			if (*it == item)
			{
				break;
			}
		}
		return it;
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (T& item : *this)
		{
			item.~T();
		}
		mSize = 0u;
	}

	template <typename T>
	bool Vector<T>::Remove(const T& item)
	{
		/*First, we must find the item.*/
		auto foundIt = Find(item);

		/*If it was not found, we can simply return immediately.*/
		if (foundIt == end())
		{
			return false;
		}

		/*Otherwise, we must destruct the item, move all subsequent elements
		in the vector back one space, and decrement mSize.*/
		(*foundIt).~T();

		rsize_t bytesToMove = (--mSize - foundIt.mIndex) * sizeof(T);
		memmove_s(mData + foundIt.mIndex, bytesToMove,
			mData + foundIt.mIndex + 1, bytesToMove);

		return true;
	}
}