#pragma once
#include <utility>
#include "Vector.h"
#include "SList.h"
#include "CompareFunctor.h"
#include "HashFunctor.h"

namespace LibraryShared
{
	/// <summary>
	/// This class implements a hash table with keys of type TKey and buckets that
	/// contain elements of type TData.
	/// </summary>
	template <typename TKey, typename TData, typename TFunctor = HashFunctor<TKey>, typename TCompare = CompareFunctor<TKey>>
	class HashMap final
	{
	public:
	using PairType = std::pair<TKey, TData>;
	using ChainIterator = typename SList<PairType>::Iterator;
	using HashMapT = HashMap<TKey, TData, TFunctor, TCompare>;

		/// <summary>
		/// This class is an iterator through the buckets.
		/// </summary>
		class Iterator final
		{
			friend class HashMap;
		public:
			/// <summary>
			/// This is the default constructor for HashMap::Iterator. It sets
			/// mOwner to nullptr and both indices to 0.
			/// </summary>
			Iterator();
			
			/// <summary>
			/// This is the destructor for Iterator. It has been set to the
			/// default.
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// This is the copy constructor for Iterator. It has been set to
			/// the default.
			/// </summary>
			/// <param name="other">- A reference to the Iterator to be copied.
			/// </param>
			Iterator(const Iterator& other);

			/// <summary>
			/// This is the move constructor for Iterator. It has been set to
			/// the default.
			/// </summary>
			/// <param name="other">- A reference to the Iterator to be moved.
			/// </param>
			Iterator(Iterator&& other);

			/// <summary>
			/// This is an overloading of the prefix increment operator. It
			/// throws an exception if already pointed past the last entry in
			/// the last non-empty bucket in the list.
			/// </summary>
			/// <returns>A reference to this newly incremented Iterator.
			/// </returns>
			Iterator& operator++();

			/// <summary>
			/// This is an overloading of the postfix increment operator. See
			/// prefix increment operator documentation.
			/// </summary>
			/// <param name="">- Dummy parameter to distinguish postfix from
			/// prefix versions of this operator.</param>
			/// <returns>A copy of the newly incremented Iterator.</returns>
			Iterator operator++(int);

			/// <summary>
			/// This is an overloading of the dereference operator. It returns
			/// a reference to the key-value pair to which this Iterator
			/// refers.
			/// </summary>
			/// <returns>A reference to the key-value pair to which this
			/// Iterator refers.</returns>
			PairType& operator*();

			/// <summary>
			/// This is an overloading of the dereference operator that returns
			/// a pointer to the key-value pair to which this Iterator refers.
			/// </summary>
			/// <returns>A pointer to the key-value pair to which this Iterator
			/// refers.</returns>
			PairType* operator->();

			/// <summary>
			/// This overloading of the equality operator returns true if the
			/// two Iterators' owners are the same, if their indices into the
			/// vector of buckets are equal, and if their iterators within said
			/// bucket are equal.
			/// </summary>
			/// <param name="other">- A reference to the Iterator to be
			/// compared with this one.</param>
			/// <returns>True if both Iterators refer to the same key-value
			/// pair in the same bucket within the same HashMap; false
			/// otherwise.</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// This overloading of the inquality operator returns false if the
			/// two Iterators' owners are the same, if their indices into the
			/// vector of buckets are equal, and if their iterators within said
			/// bucket are equal.
			/// </summary>
			/// <param name="other">- A reference to the Iterator to be
			/// compared with this one.</param>
			/// <returns>False if both Iterators refer to the same key-value
			/// pair in the same bucket within the same HashMap; true
			/// otherwise.</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// The overloading of the copy assignment operator has been set to
			/// set to the default.
			/// </summary>
			/// <param name="other">- A reference to the Iterator to be copied.
			/// </param>
			/// <returns>A reference to this Iterator.</returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// The overloading of the move assignment operator has been set to
			/// the default.
			/// </summary>
			/// <param name="other">- A reference to the Iterator to be moved.
			/// </param>
			/// <returns>A reference to this Iterator.</returns>
			Iterator& operator=(Iterator&& other);
		private:
			/// <summary>
			/// This is a pointer to the HashMap that contains this Iterator.
			/// </summary>
			HashMap* mOwner;

			/// <summary>
			/// This is the index into the list of buckets.
			/// </summary>
			uint32_t mBucketIndex;

			/// <summary>
			/// This is an iterator through the linked list contained within a
			/// bucket.
			/// </summary>
			ChainIterator mIteratorWithinBucket;

			/// <summary>
			/// This is the standard constructor for HashMap::Iterator. It sets
			/// this Iterator to point at the element pointed to by
			/// 'iteratorWithinBucket' in bucket number 'bucketIndex' in the
			/// HashMap '*owner'.
			/// </summary>
			/// <param name="owner">- A pointer to the HashMap that contains
			/// this Iterator.</param>
			/// <param name="bucketIndex">- The index into the list of buckets
			/// at which to point this Iterator.</param>
			/// <param name="iteratorWithinBucket">- An iterator that refers to
			/// the entry in the bucket to which this iterator should refer.
			/// </param>
			Iterator(HashMap* owner, uint32_t bucketIndex = 0u,
				ChainIterator iteratorWithinBucket = ChainIterator());
		};

		/// <summary>
		/// This is the standard constructor for HashMap. It creates a number
		/// of buckets and puts an empty linked list in each one.
		/// </summary>
		/// <param name="numberOfBuckets">- The number of buckets this new hash
		/// map should contain.</param>
		HashMap(uint32_t numberOfBuckets = 7u);

		/// <summary>
		/// This is the copy constructor for HashMap.
		/// </summary>
		/// <param name="other">- A reference to the HashMap to copy.</param>
		HashMap(const HashMap& other);

		/// <summary>
		/// This is the move constrcutor for HashMap.
		/// </summary>
		/// <param name="other">- A reference to the HashMap to move.</param>
		HashMap(HashMap&& other);

		/// <summary>
		/// This is the destructor for HashMap. It destructs its one and only
		/// data member, mBuckets.
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// This is the copy assignment operator for HashMap.
		/// </summary>
		/// <param name="other">- The HashMap to copy into this one.</param>
		/// <returns>A reference to this HashMap, post-copy.</returns>
		HashMap& operator=(const HashMap& other);

		/// <summary>
		/// This is the move assignment operator for HashMap.
		/// </summary>
		/// <param name="other">- The HashMap to move into this one.</param>
		/// <returns>A reference to this HashMap, post-move.</returns>
		HashMap& operator=(HashMap&& other);

		/// <summary>
		/// This method tests two HashMaps for memberwise equality. It returns
		/// true if and only if both HashMaps have the same number of entries
		/// and all entries in this HashMap can also be found in the other
		/// HashMap.
		/// </summary>
		/// <param name="other">- A reference to the HashMap to be compared to
		/// this one.</param>
		/// <returns>True if the HashMaps contain exactly the same entries as
		/// each other; false otherwise.</returns>
		bool operator==(const HashMap& other) const;

		/// <summary>
		/// This method iterates through this HashMap until it finds a
		/// key-value pair with a key equal to 'key' and returns an Iterator
		/// that refers to it. If no such pair is found, the end iterator is
		/// returned instead.
		/// </summary>
		/// <param name="key">- The key for which to search.</param>
		/// <returns>An iterator that refers to the first key-value pair found
		/// that has a key equal to 'key', or the end iterator if no such pair
		/// exists in the HashMap.</returns>
		Iterator Find(const TKey& key) const;

		/// <summary>
		/// This method inserts a pair into the HashMap, if it doesn't already
		/// exist. In either case, it returns an Iterator that refers to the
		/// entry in the HashMap with the key in pair 'toInsert'.
		/// </summary>
		/// <param name="toInsert">- The key-value pair to insert.</param>
		/// <returns>An Iterator that refers to the entry in the HashMap with
		/// pair 'toInsert'.</returns>
		Iterator Insert(const PairType& toInsert);

		/// <summary>
		/// This method inserts a pair into the HashMap, if it doesn't already
		/// exist. In either case, it returns an Iterator that refers to the
		/// entry in the HashMap with pair 'toInsert'. Additionally, unlike the
		/// other method named 'Insert', this one outputs a bool indicating
		/// whether the pair this method attempted to insert already existed in
		/// the HashMap.
		/// </summary>
		/// <param name="toInsert">- The key-value pair to insert.</param>
		/// <param name="alreadyPresent">- An output parameter indicating
		/// whether 'toInsert' already existed in the HashMap prior to this
		/// method call.</param>
		/// <returns>An Iterator that refers to the entry in the HashMap with
		/// pair 'toInsert'.</returns>
		Iterator Insert(const PairType& toInsert, bool& alreadyPresent);

		/// <summary>
		/// This method returns a reference to the data entry in the HashMap
		/// with key 'key'. If no such pair exists, an exception is thrown.
		/// There is also an overloading of this method that is const that
		/// behaves analogously to this one.
		/// </summary>
		/// <param name="index">- The key for which to get the data.</param>
		/// <returns>A reference to the data that was found.</returns>
		TData& At(const TKey& key);

		/// <summary>
		/// See non-const version of this method.
		/// </summary>
		/// <param name="key">- The key for which to get the data.</param>
		/// <returns>A reference to the data that was found.</returns>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// This overloading of the index operator finds a key-value pair in
		/// the HashMap that has key 'key'. If no such pair exists, then one
		/// is created.
		/// </summary>
		/// <param name="key">- The key for which to get the data.</param>
		/// <returns>A reference to the data that was found (or created).
		/// </returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// This method removes the key-value pair with key 'key' from the
		/// HashMap, if one exists.
		/// </summary>
		/// <param name="key">- The key to remove from the HashMap.</param>
		/// <returns>True if a pair was successfully found and removed; false
		/// otherwise.</returns>
		bool Remove(const TKey& key);

		/// <summary>
		/// This method removes all pairs from the HashMap.
		/// </summary>
		void Clear();

		/// <summary>
		/// This method returns the number of key-value pairs currently in this
		/// HashMap.
		/// </summary>
		/// <returns>The number of key-value pairs in this HashMap.</returns>
		uint32_t Size() const;

		/// <summary>
		/// This method returns a bool indicating whether there is a key-value
		/// pair with key 'key' in this HashMap.
		/// </summary>
		/// <param name="key">- The key for which to search.</param>
		/// <returns>True if there is a pair with key 'key' in this HashMap;
		/// false otherwise.</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// This method returns an Iterator that refers to a key-value pair in
		/// the first non-empty bucket of this HashMap.
		/// </summary>
		/// <returns>An iterator that refers to the beginning of this HashMap.
		/// </returns>
		Iterator begin() const;

		/// <summary>
		/// This method returns an Iterator that is meant to represent one
		/// entry past the last entry in this HashMap.
		/// </summary>
		/// <returns>An Iterator that represents one entry past the last entry
		/// in this HashMap.</returns>
		Iterator end() const;
	private:
		/// <summary>
		/// This is how the buckets of my hash map are implemented.
		/// </summary>
		Vector<SList<PairType>> mBuckets;

		/// <summary>
		/// This is a private overloading of the Find method. It iterates
		/// through the HashMap until it finds a key-value pair with a key
		/// equal to 'key' and returns an Iterator that refers to it. If no
		/// such pair is found, the end iterator is returned instead. Unlike
		/// the public version of this method, this version outputs the hash
		/// computed via the reference parameter 'hash'.
		/// </summary>
		/// <param name="key">- The key to be searched for.</param>
		/// <param name="hash">- A variable in which the result of hashing key
		/// to an index will be stored.</param>
		/// <returns>An iterator that refers to the first key-value pair found
		/// that has a key equal to 'key', or the end iterator if there is no
		/// such pair in the HashMap.</returns>
		Iterator Find(const TKey& key, uint32_t& hash) const;
	};
}

#include "HashMap.inl"