#pragma once

#include <stdexcept>

namespace LibraryShared
{
	/// <summary>
	/// This class is a singly linked list.
	/// </summary>
	template <typename T>
	class SList final
	{
	private:
		/// <summary>
		/// This class is a private class of the class SList. It represents a
		/// node in the singly linked list.
		/// </summary>
		class Node final
		{
		public:
			/// <summary>
			/// The default constructor for this class has been deleted.
			/// There should never be a Node created that doesn't have a
			/// specified data value.
			/// </summary>
			Node() = delete;

			/// <summary>
			/// The copy constructor for Node has been deleted.
			/// </summary>
			/// <param name="other">- A reference to the Node to be copied.
			/// </param>
			Node(const Node& other) = delete;

			/// <summary>
			/// This constructor creates a new node in the only way I could
			/// conceivably need; it takes a reference to what the new node
			/// should contain, and possibly a pointer to the next node in
			/// the linked list. It dynamically allocates the new data and
			/// assigns the next pointer the value of newNext.
			/// </summary>
			/// <param name="newData">- The data to store in this new Node.
			/// </param>
			/// <param name="newNext">- A pointer to the Node that should
			/// come next in the linked list.</param>
			explicit Node(const T& newData, Node* newNext = nullptr);

			/// <summary>
			/// This destructor deallocates the dynamically allocated data.
			/// </summary>
			~Node() = default;

			/// <summary>
			/// This is the data stored in this node of the linked list.
			/// </summary>
			T mData;

			/// <summary>
			/// This is a pointer to the next node in the linked list. If
			/// its value is nullptr, that means this node is at the end of
			/// the list.
			/// </summary>
			Node* mNext;
		};
	public:
		/// <summary>
		/// This class is a nested public class of SList. It is	an iterator
		/// that can be used to traverse the SList.
		/// </summary>
		class Iterator
		{
			friend class SList;

		public:
			/// <summary>
			/// The default constructor for Iterator sets both its owner SList
			/// pointer and its Node pointer to nullptr.
			/// </summary>
			Iterator();

			/// <summary>
			/// The destructor for Iterator is the default compiler-provided
			/// destructor.
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// The copy constructor for Iterator is the default compiler-
			/// provided copy constructor that does memberwise assignment.
			/// </summary>
			/// <param name="other">- The iterator to copy.</param>
			Iterator(const Iterator& other) = default;

			/// <summary>
			/// This is the move constructor for an SList Iterator.
			/// </summary>
			/// <param name="other"></param>
			Iterator(Iterator&& other);

			/// <summary>
			/// The copy assignment operator for Iterator is the default
			/// compiler-provided copy assignment operator that does memberwise
			/// assignment.
			/// </summary>
			/// <param name="other">- The iterator to copy.</param>
			/// <returns></returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// This is the move assignment operator for the Iterator of SList.
			/// </summary>
			/// <param name="other">- The SList to move into this one.</param>
			/// <returns>A reference to this Iterator, post-move.</returns>
			Iterator& operator=(Iterator&& other);

			/// <summary>
			/// This overloading of the equality operator returns true if both
			/// iterators point to the same node in the same linked list.
			/// </summary>
			/// <param name="other">- A reference to the iterator to which this
			/// one is being compared.</param>
			/// <returns>True if both iterators point to the same node in the
			/// same list; false otherwise.</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// This overloading of the inequality operator returns true if the
			/// iterators are owned by different SLists, or if they point to
			/// different nodes in the same SList.
			/// </summary>
			/// <param name="other">- A reference to the iterator to which this
			/// one is being compared.</param>
			/// <returns>False if both iterators point to the same node in the
			/// same list; true otherwise.</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// This overloading of the prefix increment operator moves the
			/// iterator one item forward in the list, unless the iterator is
			/// already one past the end of the list, in which case an
			/// exception is thrown.
			/// </summary>
			/// <returns>A reference to this iterator.</returns>
			Iterator& operator++();

			/// <summary>
			/// This overloading of the postfix increment operator moves the
			/// iterator one item forward in the list or throws an exception
			/// (see note for prefix increment operator).
			/// </summary>
			/// <param name="">- Dummy parameter to distinguish postfix
			/// operator from prefix operator.</param>
			/// <returns>A copy of this iterator, in a state before
			/// incrementation.</returns>
			Iterator operator++(int);

			/// <summary>
			/// This overloading of the dereference operator returns a
			/// reference to the item in the list contained within the node
			/// being pointed to by this iterator, or it throws an exception if
			/// the iterator points past the end of the list.
			/// </summary>
			/// <returns>A reference to the item in the list contained within
			/// the node pointed to by this iterator.</returns>
			T& operator*() const;

		private:
			/// <summary>
			/// This constructor for Iterator takes in a pointer to the SList
			/// that should own it and a pointer to the 
			/// </summary>
			/// <param name="ownerList">- A pointer to the SList that should
			/// own this iterator.</param>
			/// <param name="ownedNode">- A pointer to the node that this
			/// iterator should point to.</param>
			explicit Iterator(const SList* ownerList,
				Node* ownedNode = nullptr);

			/// <summary>
			/// The node pointed to by this iterator.
			/// </summary>
			Node* mNode;

			/// <summary>
			/// The SList that owns this iterator.
			/// </summary>
			const SList* mOwner;

		};
	public:
		/// <summary>
		/// This is the default constructor for SList. It creates an empty
		/// list.
		/// </summary>
		SList();

		/// <summary>
		/// This constructor produces a deep copy of the input SList.
		/// </summary>
		/// <param name="other">- The list to be copied.</param>
		SList(const SList& other);

		/// <summary>
		/// This is the move constructor for SList.
		/// </summary>
		/// <param name="other">- The SList to be moved into this one.</param>
		SList(SList&& other);

		/// <summary>
		/// The destructor destroys each member of the linked list, starting
		/// from the front and going back.
		/// </summary>
		~SList();

		/// <summary>
		/// This method puts a new item at the front of the linked list.
		/// </summary>
		/// <param name="newItem">- A reference to the item that gets inserted
		/// by copy at the front of the list.</param>
		void PushFront(const T& newItem);

		/// <summary>
		/// This method removes the front - most item from the list and returns
		/// a copy of it. If the list is empty, this method throws an
		/// exception.
		/// </summary>
		/// <returns>A copy of the popped item.</returns>
		void PopFront();

		/// <summary>
		/// This method pushes a new item onto the back of the list.
		/// </summary>
		/// <param name="newItem">- A reference to the item that gets inserted
		/// by copy at the back of the list.</param>
		void PushBack(const T& newItem);

		/// <summary>
		/// This method indicates whether the list is empty.
		/// </summary>
		/// <returns>True if the list is empty; false otherwise.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// This method returns a reference to the front item in the list. If
		/// the list is empty, it throws an exception.
		/// </summary>
		/// <returns>A refernce to the front item in the list.</returns>
		T& Front();

		/// <summary>
		/// This method returns a reference to the front item in the list. If
		/// the list is empty, it throws an exception.
		/// </summary>
		/// <returns>A refernce to the front item in the list.</returns>
		const T& Front() const;

		/// <summary>
		/// This method returns a reference to the back item in the list. If
		/// the list is empty, it throws an exception.
		/// </summary>
		/// <returns>A refernce to the back item in the list.</returns>
		T& Back();

		/// <summary>
		/// This method returns a reference to the back item in the list. If
		/// the list is empty, it throws an exception.
		/// </summary>
		/// <returns>A refernce to the back item in the list.</returns>
		const T& Back() const;

		/// <summary>
		/// This method returns the number of items in the list.
		/// </summary>
		/// <returns>The number of items in the list.</returns>
		unsigned int Size() const;

		/// <summary>
		/// This overloaded operator performs a deep copy of the input SList,
		/// unless self-assignment is being performed.
		/// </summary>
		/// <param name="other">- A reference to the list to be assigned from.
		/// </param>
		/// <returns>A reference to the list that was assigned to.</returns>
		SList& operator=(const SList& other);

		/// <summary>
		/// This method removes all items from the list.
		/// </summary>
		void Clear();

		/// <summary>
		/// This method returns an iterator that points to the front of the
		/// list.
		/// </summary>
		/// <returns>An iterator that points to the front of the list.
		/// </returns>
		Iterator begin() const;

		/// <summary>
		/// This method returns an iterator that points one space past the end
		/// of the list.
		/// </summary>
		/// <returns>An iterator that points just past the end of the list.
		/// </returns>
		Iterator end() const;

		/// <summary>
		/// This method inserts a new item into the list after the input
		/// iterator, unless the iterator points past the end of the list, in
		/// which case an exception is thrown.
		/// </summary>
		/// <param name="newItem">- A reference to the new item to insert in
		/// the list.</param>
		/// <param name="afterThis">- A reference to iterator after which the
		/// new item should be inserted.</param>
		void InsertAfter(const T& newItem, const Iterator& afterThis);

		/// <summary>
		/// This method returns an iterator that points to the first element in
		/// the list that matches the input value, or the iterator that points
		/// past the end of the list if the value is not found.
		/// </summary>
		/// <param name="value">- The value for which to search.</param>
		/// <returns>An iterator pointing to the first element of the list
		/// equal to 'value', or pointing past the end of the list if no such
		/// element was found.</returns>
		Iterator Find(const T& value) const;

		/// <summary>
		/// This method removes an item of value 'value' from the list and
		/// returns a bool indicating success.
		/// </summary>
		/// <param name="value">- The value to be removed.</param>
		/// <returns>True if the value was found and removed from the list;
		/// false otherwise.</returns>
		bool Remove(const T& value);

		private:
			/// <summary>
			/// This private helper method returns a pointer to the node just
			/// before the node that contains data of value 'value', or the
			/// node that actually contains the value, if it is the first node
			/// in the list.
			/// </summary>
			/// <param name="value">- The value for which to search.</param>
			/// <returns>Nullptr if the item is not found in the list, a
			/// pointer to a node containing 'value' if it is the very first
			/// node in the list, or the node just before the node contiaining
			/// 'value', otherwise.</returns>
			Node* FindBefore(const T& value) const;

			/// <summary>
			/// A pointer to the front of the list.
			/// </summary>
			typename Node* mFront;

			/// <summary>
			/// A pointer to the back of the list.
			/// </summary>
			typename Node* mBack;

			/// <summary>
			/// The number of items in the list.
			/// </summary>
			unsigned int mSize;
	};
}

#include "SList.inl"