/// <summary>
/// Header file for Vector class. Includes class declarations for Vector<T> and
/// Iterator.
/// </summary>

#pragma once

#include <cstdint>
#include <cassert>
#include <cstring>

namespace LibraryShared
{
	/// <summary>
	/// This class is a container of objects of type T. It can be dynamically
	/// resized. It has a capacity that is distinct from (and no less than) its
	/// size.
	/// </summary>
	template <typename T>
	class Vector final
	{
	public:
		/// <summary>
		/// This is an inner class of Vector that can be used to loop through a
		/// Vector. Each iterator has a pointer to an owning Vector and an
		/// index into the vector.
		/// </summary>
		class Iterator final
		{
			friend class Vector;
		public:
			/// <summary>
			/// This is the default constructor for the Iterator class. It
			/// initializes mOwner to nullptr and mIndex to 0.
			/// </summary>
			Iterator();

			/// <summary>
			/// This is the destructor for the Iterator class. It has been set
			/// to the default.
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// This is the copy constructor for the Iterator class. It has
			/// been set to the default.
			/// </summary>
			/// <param name="rhs">- A reference to the Iterator to copy.
			/// </param>
			Iterator(const Iterator& rhs) = default;

			/// <summary>
			/// This is the move constructor for the Iterator class. It has
			/// been defaulted.
			/// </summary>
			/// <param name="rhs">- A reference to the Iterator to move.
			/// </param>
			Iterator(Iterator&& rhs) = default;

			/// <summary>
			/// This is the copy assignment operator for the Iterator class.
			/// </summary>
			/// <param name="rhs">- A reference to the iterator to copy.
			/// </param>
			/// <returns>A reference to the Iterator to which we just assigned.
			/// </returns>
			Iterator& operator=(const Iterator& rhs) = default;

			/// <summary>
			/// The move assignment operator for the Iterator class has been
			/// defaulted.
			/// </summary>
			/// <param name="rhs">- A reference to the Iterator to move.
			/// </param>
			/// <returns>A reference to the Iterator to which we just assigned.
			/// </returns>
			Iterator& operator=(Iterator&& rhs) = default;

			/// <summary>
			/// This overloading of the equality operator returns true if the
			/// Iterators in question have the same owning Vector and the same
			/// index into the Vector's data.
			/// </summary>
			/// <param name="rhs">- A reference to the Iterator to compare to
			/// this one.</param>
			/// <returns>True if this Iterator and rhs have the same owning
			/// Vector and the same index; false otherwise.</returns>
			bool operator==(const Iterator& rhs) const;

			/// <summary>
			/// This overload of the inequality operator returns false if the
			/// Iterators in question have the same owning Vector and the same
			/// index into the Vector's data.
			/// </summary>
			/// <param name="rhs">- A reference to the Iterator to compare to this
			/// one.</param>
			/// <returns>False if this Iterator and rhs have the same owning Vector
			/// and the same index; true otherwise.</returns>
			bool operator!=(const Iterator& rhs) const;

			/// <summary>
			/// This overloading of the prefix increment operator increments
			/// this Iterator's index into the Vector, throwing exceptions if
			/// the new index is illegitimate or if the owning Vector has not
			/// been defined.
			/// </summary>
			/// <returns>A reference to this iterator.</returns>
			Iterator& operator++();

			/// <summary>
			/// This overloading of the postfix increment operator increments
			/// this Iterator's index into the Vector, throwing exceptions if
			/// the new index is illegitimate or if the owning Vector has not
			/// been defined.
			/// </summary>
			/// <param name="">- Dummy to distinguish postfix incrementor from
			/// prefix incrementor.</param>
			/// <returns>A copy of this Iterator.</returns>
			Iterator operator++(int);

			/// <summary>
			/// This overloading of the dereference operator returns a const
			/// reference to the item in the Vector to which this Iterator
			/// refers. This is a const method, for use with const Vectors.
			/// There is also a non-const version for use with non-const
			/// Vectors.
			/// </summary>
			/// <returns>A const reference to the item in the list to which
			/// this Iterator refers.</returns>
			const T& operator*() const;

			/// <summary>
			/// See documentation for const version of this method.
			/// </summary>
			/// <returns></returns>
			T& operator*();

		private:
			const Vector* mOwner;
			uint32_t mIndex;

			/// <summary>
			/// This constructor serves multiple purposes. First, it can serve
			/// as a constructor for an iterator that starts at the beginning
			/// of a vector (i.e., mIndex == 0). Second, it can serve as a
			/// constructor of a custom iterator for a specified vector at a
			/// specified index.
			/// </summary>
			/// <param name="owner">- A pointer to the Vector that should own
			/// this iterator.</param>
			/// <param name="index">- The index of the entry in the vector to
			/// which this iterator should refer.</param>
			explicit Iterator(const Vector<T>* owner, uint32_t index = 0u);
		};

		/// <summary>
		/// This is Vector's one and only constructor (that isn't a copy or
		/// move constructor). It can be used as a default constructor (by
		/// letting the parameter stick with its default value) or as a
		/// constructor of a vector with a custom capacity.
		/// </summary>
		/// <param name="capacity">- The capacity with which this Vector should
		/// start.</param>
		explicit Vector(uint32_t capacity = 8u);

		/// <summary>
		/// This is the copy constructor for Vector.
		/// </summary>
		/// <param name="rhs">- A reference to the Vector to be copied.</param>
		Vector(const Vector& rhs);

		/// <summary>
		/// This is the move constructor for Vector.
		/// </summary>
		/// <param name="rhs">- A reference to the Vector whose data will be
		/// moved.</param>
		Vector(Vector&& rhs);

		/// <summary>
		/// This is the destructor for Vector.
		/// </summary>
		~Vector();

		/// <summary>
		/// This is the copy assignment operator for Vector.
		/// </summary>
		/// <param name="rhs">- A reference to the Vector to be copied.</param>
		/// <returns>A reference to the newly copied Vector.</returns>
		Vector<T>& operator=(const Vector& rhs);

		/// <summary>
		/// This is the move assignment operator for Vector.
		/// </summary>
		/// <param name="rhs">- A reference to the vector to be moved.</param>
		/// <returns></returns>
		Vector<T>& operator=(Vector&& rhs);

		/// <summary>
		/// This overloading of the [] operator throws an exception in the
		/// event of an invalid index; otherwise, it returns a reference to the
		/// entry in the vector at the specified index. There is also a const
		/// version of this method, to be used with const Vectors.
		/// </summary>
		/// <param name="index">- The index at which the desired element is
		/// stored in the vector.</param>
		/// <returns>A reference to the item at index "index" in the vector.
		/// </returns>
		T& operator[](uint32_t index);

		/// <summary>
		/// See non-const version of this method.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		const T& operator[](uint32_t index) const;

		/// <summary>
		/// This method removes the last item from the vector if the vector is
		/// non-empty. Otherwise, it throws an exception.
		/// </summary>
		void PopBack();

		/// <summary>
		/// This method indicates whether this vector is empty.
		/// </summary>
		/// <returns>True if the vector contains zero elements; false
		/// otherwise.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// This method returns a reference to the first element in this
		/// vector, unless the vector is empty, in which case an exception is
		/// thrown. There is also a const version of this method, which is to
		/// be used with const Vectors.
		/// </summary>
		/// <returns>A reference to the first element in the vector.</returns>
		T& Front();

		/// <summary>
		/// See non-const version of this method.
		/// </summary>
		/// <returns></returns>
		const T& Front() const;

		/// <summary>
		/// This method returns a reference to the last element in this vector,
		/// unless the vector is empty, in which case an exception is thrown.
		/// There is also a const version of this method, which is to be used
		/// with const Vectors.
		/// </summary>
		/// <returns></returns>
		T& Back();

		/// <summary>
		/// See non-const version of this method.
		/// </summary>
		/// <returns></returns>
		const T& Back() const;

		/// <summary>
		/// This method returns the number of elements in the vector.
		/// </summary>
		/// <returns>An unsigned int representing the number of elements in the
		/// vector.</returns>
		uint32_t Size() const;

		/// <summary>
		/// This method returns the number of elements the vector is able to
		/// store in memory, as it is now. If you need it to have a different
		/// capacity, use the Reserve method.
		/// </summary>
		/// <returns>An unsigned int representing the capacity of the vector in
		/// its current form.</returns>
		uint32_t Capacity() const;

		/// <summary>
		/// See operator[].
		/// </summary>
		/// <returns></returns>
		T& At(uint32_t index);

		/// <summary>
		/// See operator[].
		/// </summary>
		/// <returns></returns>
		const T& At(uint32_t index) const;

		/// <summary>
		/// This method returns an iterator that points at the beginning of the
		/// vector.
		/// </summary>
		/// <returns>An iterator that points at the beginning of the vector.
		/// </returns>
		Iterator begin() const;

		/// <summary>
		/// This method returns an iterator that points one space past the end
		/// of the vector.
		/// </summary>
		/// <returns>An iterator that points one space past the end of the
		/// vector.</returns>
		Iterator end() const;

		/// <summary>
		/// This method pushes a new item at the back of the vector, increasing
		/// capacity if necessary.
		/// </summary>
		/// <param name="item">- A reference to the new item to be pushed.</param>
		void PushBack(const T& item);

		/// <summary>
		/// This method resizes the vector to have the input number of entries
		/// available to store objects. If, however, the new capacity would be
		/// too small to fit all objects currently stored in the vector, then
		/// the vector is resized to fit not the input capacity, but the
		/// current number of objects.
		/// </summary>
		/// <param name="capacity">- The number of entries the newly resized
		/// vector should be able to store.</param>
		/// <returns>True if the vector now has a capacity equal to the input
		/// capacity; false otherwise (i.e., if the number of objects was
		/// larger than the input capacity).</returns>
		bool Reserve(uint32_t capacity);

		/// <summary>
		/// This method performs a linear search through the vector for the
		/// input item. When found, an iterator that refers to that item is
		/// returned; if not found, the end iterator is returned.
		/// </summary>
		/// <param name="item">- A reference to the item for which to search.
		/// </param>
		/// <returns>An iterator that points to "item", or the end iterator if
		/// "item" was not found in the vector.</returns>
		Iterator Find(const T& item);

		/// <summary>
		/// This method removes all items from the vector and sets its size to
		/// zero.
		/// </summary>
		void Clear();

		/// <summary>
		/// This method removes the first item that matches "item" from the
		/// vector, if it exists.
		/// </summary>
		/// <param name="item">- A reference to the item to be removed.</param>
		/// <returns>True if the item was found and successfully removed; false
		/// otherwise.</returns>
		bool Remove(const T& item);

	private:
		uint32_t mSize;
		uint32_t mCapacity;
		uint32_t mCapacityIncreaseAmount{ 1u };
		T* mData;
	};
}

#include "Vector.inl"