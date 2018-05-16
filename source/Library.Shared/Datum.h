#pragma once

#include <cstdint>
#include <string>

#define GLM_FORCE_CXX98
#include "RTTI.h"
#include "glm/glm.hpp"

namespace LibraryShared
{
	class Scope;

	/// <summary>
	/// This enum will be used to denote what type of data Datum's member
	/// variable 'mValues' contains a pointer to.
	/// </summary>
	enum class DatumType
	{
		UNKNOWN,
		INTEGER,
		FLOAT,
		VECTOR,
		MATRIX,
		TABLE,
		STRING,
		POINTER
	};

	/// <summary>
	/// The 'Datum' class represents a single value of an integer,
	/// floating-point, 4D vector, 4X4 matrix, Scope pointer, string, or RTTI
	/// pointer type.
	/// </summary>
	class Datum final
	{
		friend class Attributed;
	public:
		/// <summary>
		/// This union has been created to be the type for the 'values' array. It
		/// is a pointer to one of a number of primitive types and also
		/// std::string.
		/// </summary>
		union ValuesType
		{
			int32_t* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			Scope** scope;
			std::string* s;
			RTTI** p;
			std::byte* bp; /*This exists only in the event we want to avoid using
							pointer arithmetic.*/
		};

		/// <summary>
		/// This is the default constructor for Datum. It does not allocate any
		/// memory for mValues to point to, and it sets mType to UNKNOWN.
		/// </summary>
		Datum();

		/// <summary>
		/// This is the copy constructor for Datum. It performs a deep copy of its
		/// mValues, if the other Datum stores its mValues interally. Otherwise, it
		/// performs a shallow copy.
		/// </summary>
		/// <param name="other">- A reference to the Datum to copy.</param>
		Datum(const Datum& other);

		/// <summary>
		/// This is the move constructor for Datum. It performs a shallow copy of
		/// its mValues.
		/// </summary>
		/// <param name="other">- A reference to the Datum to be moved.</param>
		Datum(Datum&& other);

		/// <summary>
		/// This constructor supports the use of initializer lists to set up a
		/// Datum of integers.
		/// </summary>
		/// <param name="list">- The list from which to initialize this Datum.
		/// </param>
		Datum(const std::initializer_list<int32_t>& list); //not explicit because I would like implicit conversion of initializer lists

		/// <summary>
		/// This constructor supports the use of initializer lists to set up a
		/// Datum of floats.
		/// </summary>
		/// <param name="list">- The list from which to initialize this Datum.
		/// </param>
		Datum(const std::initializer_list<float>& list);

		/// <summary>
		/// This constructor supports the use of initializer lists to set up a
		/// Datum of 4D vectors.
		/// </summary>
		/// <param name="list">- The list from which to initialize this Datum.
		/// </param>
		Datum(const std::initializer_list<glm::vec4>& list);

		/// <summary>
		/// This constructor supports the use of initializer lists to set up a
		/// Datum of 4 by 4 matrices.
		/// </summary>
		/// <param name="list">- The list from which to initialize this Datum.
		/// </param>
		Datum(const std::initializer_list<glm::mat4>& list);

		/// <summary>
		/// This constructor supports the use of initializer lists to set up a
		/// Datum of pointers to Scopes.
		/// </summary>
		/// <param name="list">- The list from which to initialize this Datum.
		/// </param>
		Datum(const std::initializer_list<Scope*>& list);

		/// <summary>
		/// This constructor supports the use of initializer lists to set up a
		/// Datum of strings.
		/// </summary>
		/// <param name="list">- The list from which to initialize this Datum.
		/// </param>
		Datum(const std::initializer_list<std::string>& list);

		/// <summary>
		/// This constructor supports the use of initializer lists to set up a
		/// Datum of pointers.
		/// </summary>
		/// <param name="list">- The list from which to initialize this Datum.
		/// </param>
		Datum(const std::initializer_list<RTTI*>& list);

		/// <summary>
		/// This is the destructor for Datum. It frees the memory pointed to by
		/// mValues, but only if the memory is internal.
		/// </summary>
		~Datum();

		/// <summary>
		/// This overloading of the assignment operator allows users to assign
		/// to a Datum from an initializer list. However, it only works if
		/// this Datum either does not yet have a type or has a type that
		/// matches that of the list.
		/// </summary>
		/// <param name="list">- The list to assign to this Datum.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const std::initializer_list<int32_t>& list);

		/// <summary>
		/// This overloading of the assignment operator allows users to assign
		/// to a Datum from an initializer list. However, it only works if
		/// this Datum either does not yet have a type or has a type that
		/// matches that of the list.
		/// </summary>
		/// <param name="list">- The list to assign to this Datum.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const std::initializer_list<float>& list);

		/// <summary>
		/// This overloading of the assignment operator allows users to assign
		/// to a Datum from an initializer list. However, it only works if
		/// this Datum either does not yet have a type or has a type that
		/// matches that of the list.
		/// </summary>
		/// <param name="list">- The list to assign to this Datum.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const std::initializer_list<glm::vec4>& list);

		/// <summary>
		/// This overloading of the assignment operator allows users to assign
		/// to a Datum from an initializer list. However, it only works if
		/// this Datum either does not yet have a type or has a type that
		/// matches that of the list.
		/// </summary>
		/// <param name="list">- The list to assign to this Datum.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const std::initializer_list<glm::mat4>& list);

		/// <summary>
		/// This overloading of the assignment operator allows users to assign
		/// to a Datum from an initializer list. However, it only works if
		/// this Datum either does not yet have a type or has a type that
		/// matches that of the list.
		/// </summary>
		/// <param name="list">- The list to assign to this Datum.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const std::initializer_list<Scope*>& list);

		/// <summary>
		/// This overloading of the assignment operator allows users to assign
		/// to a Datum from an initializer list. However, it only works if
		/// this Datum either does not yet have a type or has a type that
		/// matches that of the list.
		/// </summary>
		/// <param name="list">- The list to assign to this Datum.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const std::initializer_list<std::string>& list);

		/// <summary>
		/// This overloading of the assignment operator allows users to assign
		/// to a Datum from an initializer list. However, it only works if
		/// this Datum either does not yet have a type or has a type that
		/// matches that of the list.
		/// </summary>
		/// <param name="list">- The list to assign to this Datum.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const std::initializer_list<RTTI*>& list);

		/// <summary>
		/// This is the copy assignment operator. It produces a deep copy if the
		/// other Datum has internal storage, or a shallow copy otherwise. This
		/// method throws an exception if this Datum has previously had data
		/// assigned to it and is of a type that isn't other's type.
		/// </summary>
		/// <param name="other">- The Datum to copy.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const Datum& other);

		/// <summary>
		/// This is the move assignment operator. It makes other's data internal
		/// to this Datum if it was internal to 'other', and it then makes
		/// other's data external. Otherwise, it simply copies other's mValues
		/// data member. Also, this method throws an exception if this Datum has
		/// previously had data assigned to it and is of a type that isn't
		/// other's type.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		Datum& operator=(Datum&& other);

		/// <summary>
		/// This overloading of the assignment operator treats this Datum as if
		/// it were a scalar. It stores the input value at beginning of the
		/// array of data if doing so would not cause a resizing of external
		/// data. Also, an exception is thrown if this Datum has been used to
		/// store data of a different type than the input value.
		/// </summary>
		/// <param name="value">- An integer to store at the front of this
		/// Datum's array of data.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(int32_t input);

		/// <summary>
		/// This overloading of the assignment operator treats this Datum as if
		/// it were a scalar. It stores the input value at beginning of the
		/// array of data if doing so would not cause a resizing of external
		/// data. Also, an exception is thrown if this Datum has been used to
		/// store data of a different type than the input value.
		/// </summary>
		/// <param name="value">- A float to store at the front of this Datum's
		/// array of data.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(float input);

		/// <summary>
		/// This overloading of the assignment operator treats this Datum as if
		/// it were a scalar. It stores the input value at beginning of the
		/// array of data if doing so would not cause a resizing of external
		/// data. Also, an exception is thrown if this Datum has been used to
		/// store data of a different type than the input value.
		/// </summary>
		/// <param name="value">- A 4D vector to store at the front of this
		/// Datum's array of data.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(glm::vec4 input);

		/// <summary>
		/// This overloading of the assignment operator treats this Datum as if
		/// it were a scalar. It stores the input value at beginning of the
		/// array of data if doing so would not cause a resizing of external
		/// data. Also, an exception is thrown if this Datum has been used to
		/// store data of a different type than the input value.
		/// </summary>
		/// <param name="value">- A 4 by 4 matrix to store at the front of this
		/// Datum's array of data.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(glm::mat4 input);

		/// <summary>
		/// This overloading of the assignment operator treats this Datum as if
		/// it were a scalar. It stores the input value at beginning of the
		/// array of data if doing so would not cause a resizing of external
		/// data. Also, an exception is thrown if this Datum has been used to
		/// store data of a different type than the input value.
		/// </summary>
		/// <param name="value">- A pointer to store at the front of this
		/// Datum's array of data.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(Scope& input);

		/// <summary>
		/// This overloading of the assignment operator treats this Datum as if
		/// it were a scalar. It stores the input value at beginning of the
		/// array of data if doing so would not cause a resizing of external
		/// data. Also, an exception is thrown if this Datum has been used to
		/// store data of a different type than the input value.
		/// </summary>
		/// <param name="value">- A string to store at the front of this Datum's
		/// array of data.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(const std::string& input);

		/// <summary>
		/// This overloading of the assignment operator treats this Datum as if
		/// it were a scalar. It stores the input value at beginning of the
		/// array of data if doing so would not cause a resizing of external
		/// data. Also, an exception is thrown if this Datum has been used to
		/// store data of a different type than the input value.
		/// </summary>
		/// <param name="value">- A pointer to store at the front of this
		/// Datum's array of data.</param>
		/// <returns>A reference to this Datum.</returns>
		Datum& operator=(RTTI* input);

		/// <summary>
		/// This method shrinks the capacity so that it is equal to the size of
		/// the array of data, but only if the dara are internal. Otherwise, an
		/// exception is thrown.
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// This method stores a new value at the end of this Datum's array of
		/// data if its data is internal and it is of a compatible type.
		/// Otherwise, it throws an exception.
		/// </summary>
		/// <param name="input">- The new value to insert.</param>
		void PushBack(int32_t input);

		/// <summary>
		/// This method stores a new value at the end of this Datum's array of
		/// data if its data is internal and it is of a compatible type.
		/// Otherwise, it throws an exception.
		/// </summary>
		/// <param name="input">- The new value to insert.</param>
		void PushBack(float input);

		/// <summary>
		/// This method stores a new value at the end of this Datum's array of
		/// data if its data is internal and it is of a compatible type.
		/// Otherwise, it throws an exception.
		/// </summary>
		/// <param name="input">- The new value to insert.</param>
		void PushBack(glm::vec4 input);

		/// <summary>
		/// This method stores a new value at the end of this Datum's array of
		/// data if its data is internal and it is of a compatible type.
		/// Otherwise, it throws an exception.
		/// </summary>
		/// <param name="input">- The new value to insert.</param>
		void PushBack(glm::mat4 input);

		/// <summary>
		/// This method stores a new value at the end of this Datum's array of
		/// data if its data is internal and it is of a compatible type.
		/// Otherwise, it throws an exception.
		/// </summary>
		/// <param name="input">- The new value to insert.</param>
		void PushBack(Scope& input);

		/// <summary>
		/// This method stores a new value at the end of this Datum's array of
		/// data if its data is internal and it is of a compatible type.
		/// Otherwise, it throws an exception.
		/// </summary>
		/// <param name="input">- The new value to insert.</param>
		void PushBack(const std::string& input);

		/// <summary>
		/// This method stores a new value at the end of this Datum's array of
		/// data if its data is internal and it is of a compatible type.
		/// Otherwise, it throws an exception.
		/// </summary>
		/// <param name="input">- The new value to insert.</param>
		void PushBack(RTTI* input);

		/// <summary>
		/// This method removes the last item in this Datum's array of data, but
		/// it throws an exception if the array is empty or if the data are
		/// external.
		/// </summary>
		void PopBack();

		/// <summary>
		/// This method pushes a new item into the array of data. It throws an
		/// exception if this Datum is of an incompatible type or if its data
		/// are external.
		/// </summary>
		/// <param name="input">- The new item to be pushed.</param>
		void PushFront(int32_t input);

		/// <summary>
		/// This method pushes a new item into the array of data. It throws an
		/// exception if this Datum is of an incompatible type or if its data
		/// are external.
		/// </summary>
		/// <param name="input"> - The new item to be pushed.</param>
		void PushFront(float input);

		/// <summary>
		/// This method pushes a new item into the array of data. It throws an
		/// exception if this Datum is of an incompatible type or if its data
		/// are external.
		/// </summary>
		/// <param name="input"> - The new item to be pushed.</param>
		void PushFront(glm::vec4 input);

		/// <summary>
		/// This method pushes a new item into the array of data. It throws an
		/// exception if this Datum is of an incompatible type or if its data
		/// are external.
		/// </summary>
		/// <param name="input"> - The new item to be pushed.</param>
		void PushFront(glm::mat4 input);

		/// <summary>
		/// This method pushes a new item into the array of data. It throws an
		/// exception if this Datum is of an incompatible type or if its data
		/// are external.
		/// </summary>
		/// <param name="input"> - The new item to be pushed.</param>
		void PushFront(Scope& input);

		/// <summary>
		/// This method pushes a new item into the array of data. It throws an
		/// exception if this Datum is of an incompatible type or if its data
		/// are external.
		/// </summary>
		/// <param name="input"> - The new item to be pushed.</param>
		void PushFront(const std::string& input);

		/// <summary>
		/// This method pushes a new item into the array of data. It throws an
		/// exception if this Datum is of an incompatible type or if its data
		/// are external.
		/// </summary>
		/// <param name="input"> - The new item to be pushed.</param>
		void PushFront(RTTI* input);

		/// <summary>
		/// This method removes the first item in this Datum's array of data,
		/// but it throws an exception if the array is empty or if the data are
		/// external.
		/// </summary>
		void PopFront();

		/// <summary>
		/// This method removes the value 'input' from the Datum's array of data
		/// and returns a boolean indicating whether the input was in fact found
		/// in the data at all.
		/// </summary>
		/// <param name="input">- The value to remove.</param>
		/// <returns>True if 'input' was found and successfully removed; false
		/// otherwise.</returns>
		bool Remove(int32_t input);

		/// <summary>
		/// This method removes the value 'input' from the Datum's array of data
		/// and returns a boolean indicating whether the input was in fact found
		/// in the data at all.
		/// </summary>
		/// <param name="input">- The value to remove.</param>
		/// <returns>True if 'input' was found and successfully removed; false
		/// otherwise.</returns>
		bool Remove(float input);

		/// <summary>
		/// This method removes the value 'input' from the Datum's array of data
		/// and returns a boolean indicating whether the input was in fact found
		/// in the data at all.
		/// </summary>
		/// <param name="input">- The value to remove.</param>
		/// <returns>True if 'input' was found and successfully removed; false
		/// otherwise.</returns>
		bool Remove(glm::vec4 input);

		/// <summary>
		/// This method removes the value 'input' from the Datum's array of data
		/// and returns a boolean indicating whether the input was in fact found
		/// in the data at all.
		/// </summary>
		/// <param name="input">- The value to remove.</param>
		/// <returns>True if 'input' was found and successfully removed; false
		/// otherwise.</returns>
		bool Remove(glm::mat4 input);

		/// <summary>
		/// This method removes the value 'input' from the Datum's array of data
		/// and returns a boolean indicating whether the input was in fact found
		/// in the data at all.
		/// </summary>
		/// <param name="input">- The value to remove.</param>
		/// <returns>True if 'input' was found and successfully removed; false
		/// otherwise.</returns>
		bool Remove(Scope& input);

		/// <summary>
		/// This method removes the value 'input' from the Datum's array of data
		/// and returns a boolean indicating whether the input was in fact found
		/// in the data at all.
		/// </summary>
		/// <param name="input">- The value to remove.</param>
		/// <returns>True if 'input' was found and successfully removed; false
		/// otherwise.</returns>
		bool Remove(const std::string& input);
		
		/// <summary>
		/// This method removes the value 'input' from the Datum's array of data
		/// and returns a boolean indicating whether the input was in fact found
		/// in the data at all.
		/// </summary>
		/// <param name="input">- The value to remove.</param>
		/// <returns>True if 'input' was found and successfully removed; false
		/// otherwise.</returns>
		bool Remove(RTTI* input);

		/// <summary>
		/// This method removes the value at index 'index' from this Datum's
		/// array of data. An exception is thrown if the array index is out of
		/// bounds.
		/// </summary>
		/// <param name="index">- The index of the value to remove.</param>
		void RemoveAtIndex(uint32_t index);

		/// <summary>
		/// Getter for the type enum associated with this Datum.
		/// </summary>
		/// <returns>A DatumType value that indicates what type of data is stored
		/// in mValues.</returns>
		DatumType Type() const;

		/// <summary>
		/// Accessor for the boolean indicating whether this Datum uses internal
		/// (as opposed to external) storage.
		/// </summary>
		/// <returns>True if this Datum uses internal storage, meaning it owns
		/// the memory taken up by its value(s); false otherwise.</returns>
		bool IsInternal() const;
		
		/// <summary>
		/// This method attempts to store the new type in this Datum. However, it
		/// only works if the current type is UNKNOWN, indicating that there are
		/// no stored data currently in this Datum.
		/// </summary>
		/// <param name="type">- The new type to which this Datum should be set.
		/// </param>
		/// <returns>True if SetType succeeds; false otherwise.</returns>
		bool SetType(DatumType type);

		/// <summary>
		/// This method returns the current size of this Datum's data array.
		/// </summary>
		/// <returns>The current size of this Datum's data array.</returns>
		uint32_t Size() const;

		/// <summary>
		/// This method sets a new sized for this Datum. If the new size is
		/// smaller than the current size, array entries are destructed (if
		/// necessary) from the end. If the new size of greater than the current
		/// size, new entries are default constructed to fill the void. If the
		/// type is UNKNOWN, or if the data are external, an exception is
		/// thrown.
		/// </summary>
		/// <param name="newSize">The new number of array entries this Datum
		/// should contain.</param>
		void SetSize(uint32_t newSize);

		/// <summary>
		/// This method returns the current capacity of this Datum's data array.
		/// </summary>
		/// <returns>The current capacity of this Datum's data array.</returns>
		uint32_t Capacity() const;

		/// <summary>
		/// This method sets this Datum's capacity and size to 0 and its type to
		/// UNKNOWN. It also frees any internal data associated with this Datum.
		/// </summary>
		void Clear();
		
		/// <summary>
		/// This method returns a bool indicating whether this Datum has no
		/// data.
		/// </summary>
		/// <returns>True if mSize is 0; false otherwise.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// This method attempts to resize the amount of memory dedicated to
		/// this Datum. It throws an exception if this would end up shrinking
		/// capacity or if the data are external.
		/// </summary>
		/// <param name="newCapacity"></param>
		void Reserve(size_t newCapacity);

		/// <summary>
		/// This method compares two Datum objects and returns true if their
		/// contents are equivalent.
		/// </summary>
		/// <param name="other">- The Datum with which this one should be
		/// compared.</param>
		/// <returns>True if the two Datum objects contain equivalent data;
		/// false otherwise.</returns>
		bool operator==(const Datum& other) const;

		/// <summary>
		/// This method compares two Datum objects and returns true if their
		/// contents are not equivalent.
		/// </summary>
		/// <param name="other">- The Datum with which this one should be
		/// compared.</param>
		/// <returns>False if the two Datum objects contain equivalent data;
		/// true otherwise.</returns>
		bool operator!=(const Datum& other) const;

		/// <summary>
		/// This method compares an integer with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The integer to which this Datum should be
		/// compared.</param>
		/// <returns>True if this Datum is non-empty, its type is INTEGER, and
		/// its first element is equal to 'other'; false otherwise.</returns>
		bool operator==(int32_t other) const;

		/// <summary>
		/// This method compares an integer with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The integer to which this Datum should be
		/// compared.</param>
		/// <returns>False if this Datum is non-empty, its type is INTEGER, and
		/// its first element is equal to 'other'; true otherwise.</returns>
		bool operator!=(int32_t other) const;

		/// <summary>
		/// This method compares a float with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The float to which this Datum should be
		/// compared.</param>
		/// <returns>True if this Datum is non-empty, its type is FLOAT, and its
		/// first element is equal to 'other'; false otherwise.</returns>
		bool operator==(float other) const;

		/// <summary>
		/// This method compares an integer with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The float to which this Datum should be
		/// compared.</param>
		/// <returns>False if this Datum is non-empty, its type is FLOAT, and
		/// its first element is equal to 'other'; true otherwise.</returns>
		bool operator!=(float other) const;

		/// <summary>
		/// This method compares a 4D vector with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The vector to which this Datum should be
		/// compared.</param>
		/// <returns>True if this Datum is non-empty, its type is VECTOR, and
		/// its first element is equal to 'other'; false otherwise.</returns>
		bool operator==(glm::vec4 other) const;

		/// <summary>
		/// This method compares a 4D vector with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The vector to which this Datum should be
		/// compared.</param>
		/// <returns>False if this Datum is non-empty, its type is VECTOR, and
		/// its first element is equal to 'other'; true otherwise.</returns>
		bool operator!=(glm::vec4 other) const;

		/// <summary>
		/// This method compares a 4 by 4 matrix with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The matrix to which this Datum should be
		/// compared.</param>
		/// <returns>True if this Datum is non-empty, its type is MATRIX, and
		/// its first element is equal to 'other'; false otherwise.</returns>
		bool operator==(glm::mat4 other) const;

		/// <summary>
		/// This method compares a 4 by 4 matrix with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The matrix to which this Datum should be
		/// compared.</param>
		/// <returns>False if this Datum is non-empty, its type is MATRIX, and
		/// its first element is equal to 'other'; true otherwise.</returns>
		bool operator!=(glm::mat4 other) const;

		/// <summary>
		/// This method compares a Scope with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- A pointer to the Scope to which this Datum
		/// should be compared.</param>
		/// <returns>True if this Datum is non-empty, its type is TABLE, and
		/// its first element is equal to 'other'; false otherwise.</returns>
		bool operator==(Scope& other) const;

		/// <summary>
		/// This method compares a Scope with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- A pointer to the Scope to which this Datum
		/// should be compared.</param>
		/// <returns>False if this Datum is non-empty, its type is TABLE, and
		/// its first element is equal to 'other'; true otherwise.</returns>
		bool operator!=(Scope& other) const;

		/// <summary>
		/// This method compares a string with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The string to which this Datum should be
		/// compared.</param>
		/// <returns>True if this Datum is non-empty, its type is STRING, and
		/// its first element is equal to 'other'; false otherwise.</returns>
		bool operator==(const std::string& other) const;

		/// <summary>
		/// This method compares a string with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- The string to which this Datum should be
		/// compared.</param>
		/// <returns>False if this Datum is non-empty, its type is STRING, and
		/// its first element is equal to 'other'; true otherwise.</returns>
		bool operator!=(const std::string& other) const;

		/// <summary>
		/// This method compares an RTTI with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- A pointer to the RTTI to which this Datum
		/// should be compared.</param>
		/// <returns>True if this Datum is non-empty, its type is POINTER, and
		/// its first element is equal to 'other'; false otherwise.</returns>
		bool operator==(RTTI* other) const;

		/// <summary>
		/// This method compares an RTTI with this Datum's array's first
		/// element.
		/// </summary>
		/// <param name="other">- A pointer to the RTTI to which this Datum
		/// should be compared.</param>
		/// <returns>False if this Datum is non-empty, its type is POINTER, and
		/// its first element is equal to 'other'; true otherwise.</returns>
		bool operator!=(RTTI* other) const;

		/// <summary>
		/// This method makes this Datum point to the array 'arr' as external
		/// data.
		/// </summary>
		/// <param name="arr">- The array this Datum should point to.</param>
		/// <param name="size">The number of entries in the array.</param>
		/// <returns>True if the type of this Datum allowed this method to
		/// succeed; false otherwise.</returns>
		bool SetStorage(int32_t* arr, uint32_t size);

		/// <summary>
		/// This method makes this Datum point to the array 'arr' as external
		/// data.
		/// </summary>
		/// <param name="arr">- The array this Datum should point to.</param>
		/// <param name="size">The number of entries in the array.</param>
		/// <returns>True if the type of this Datum allowed this method to
		/// succeed; false otherwise.</returns>
		bool SetStorage(float* arr, uint32_t size);

		/// <summary>
		/// This method makes this Datum point to the array 'arr' as external
		/// data.
		/// </summary>
		/// <param name="arr">- The array this Datum should point to.</param>
		/// <param name="size">The number of entries in the array.</param>
		/// <returns>True if the type of this Datum allowed this method to
		/// succeed; false otherwise.</returns>
		bool SetStorage(glm::vec4* arr, uint32_t size);

		/// <summary>
		/// This method makes this Datum point to the array 'arr' as external
		/// data.
		/// </summary>
		/// <param name="arr">- The array this Datum should point to.</param>
		/// <param name="size">The number of entries in the array.</param>
		/// <returns>True if the type of this Datum allowed this method to
		/// succeed; false otherwise.</returns>
		bool SetStorage(glm::mat4* arr, uint32_t size);

		/// <summary>
		/// This method makes this Datum point to the array 'arr' as external
		/// data.
		/// </summary>
		/// <param name="arr">- The array this Datum should point to.</param>
		/// <param name="size">The number of entries in the array.</param>
		/// <returns>True if the type of this Datum allowed this method to
		/// succeed; false otherwise.</returns>
		bool SetStorage(std::string* arr, uint32_t size);

		/// <summary>
		/// This method makes this Datum point to the array 'arr' as external
		/// data.
		/// </summary>
		/// <param name="arr">- The array this Datum should point to.</param>
		/// <param name="size">The number of entries in the array.</param>
		/// <returns>True if the type of this Datum allowed this method to
		/// succeed; false otherwise.</returns>
		bool SetStorage(RTTI** arr, uint32_t size);


		/// <summary>
		/// This method attempts to set the value at index 'index' to value
		/// 'input'. If this Datum is of an incompatible type or 'index' is out
		/// of bounds, this method fails.
		/// </summary>
		/// <param name="input">The value to which to set one of the entries in
		/// this Datum's array.</param>
		/// <param name="index">The index into this Datum's array with the value
		/// to be changed.</param>
		/// <returns>True if this method succeeds; false if it fails.</returns>
		bool Set(int32_t input, uint32_t index = 0u);

		/// <summary>
		/// This method attempts to set the value at index 'index' to value
		/// 'input'. If this Datum is of an incompatible type or 'index' is out
		/// of bounds, this method fails.
		/// </summary>
		/// <param name="input">The value to which to set one of the entries in
		/// this Datum's array.</param>
		/// <param name="index">The index into this Datum's array with the value
		/// to be changed.</param>
		/// <returns>True if this method succeeds; false if it fails.</returns>
		bool Set(float input, uint32_t index = 0u);

		/// <summary>
		/// This method attempts to set the value at index 'index' to value
		/// 'input'. If this Datum is of an incompatible type or 'index' is out
		/// of bounds, this method fails.
		/// </summary>
		/// <param name="input">The value to which to set one of the entries in
		/// this Datum's array.</param>
		/// <param name="index">The index into this Datum's array with the value
		/// to be changed.</param>
		/// <returns>True if this method succeeds; false if it fails.</returns>
		bool Set(glm::vec4 input, uint32_t index = 0u);

		/// <summary>
		/// This method attempts to set the value at index 'index' to value
		/// 'input'. If this Datum is of an incompatible type or 'index' is out
		/// of bounds, this method fails.
		/// </summary>
		/// <param name="input">The value to which to set one of the entries in
		/// this Datum's array.</param>
		/// <param name="index">The index into this Datum's array with the value
		/// to be changed.</param>
		/// <returns>True if this method succeeds; false if it fails.</returns>
		bool Set(glm::mat4 input, uint32_t index = 0u);

		/// <summary>
		/// This method attempts to set the value at index 'index' to value
		/// 'input'. If this Datum is of an incompatible type or 'index' is out
		/// of bounds, this method fails.
		/// </summary>
		/// <param name="input">The value to which to set one of the entries in
		/// this Datum's array.</param>
		/// <param name="index">The index into this Datum's array with the value
		/// to be changed.</param>
		/// <returns>True if this method succeeds; false if it fails.</returns>
		bool Set(Scope& input, uint32_t index = 0u);

		/// <summary>
		/// This method attempts to set the value at index 'index' to value
		/// 'input'. If this Datum is of an incompatible type or 'index' is out
		/// of bounds, this method fails.
		/// </summary>
		/// <param name="input">The value to which to set one of the entries in
		/// this Datum's array.</param>
		/// <param name="index">The index into this Datum's array with the value
		/// to be changed.</param>
		/// <returns>True if this method succeeds; false if it fails.</returns>
		bool Set(const std::string& input, uint32_t index = 0u);

		/// <summary>
		/// This method attempts to set the value at index 'index' to value
		/// 'input'. If this Datum is of an incompatible type or 'index' is out
		/// of bounds, this method fails.
		/// </summary>
		/// <param name="input">The value to which to set one of the entries in
		/// this Datum's array.</param>
		/// <param name="index">The index into this Datum's array with the value
		/// to be changed.</param>
		/// <returns>True if this method succeeds; false if it fails.</returns>
		bool Set(RTTI* input, uint32_t index = 0u);

		/// <summary>
		/// This method returns a reference to the entry in this Datum's array
		/// at index 'index'. An exception is thrown if the type of this Datum
		/// is incompatible or if 'index' is out of bounds. There is also a
		/// const version of this method that returns a const reference.
		/// </summary>
		/// <param name="index">The index at which to get a value.</param>
		/// <returns>A reference to the value at index 'index'.</returns>
		int32_t& GetInt(uint32_t index = 0u);

		/// <summary>
		/// See the non-const version of this method.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		const int32_t& GetInt(uint32_t index = 0u) const;

		/// <summary>
		/// This method returns a reference to the entry in this Datum's array
		/// at index 'index'. An exception is thrown if the type of this Datum
		/// is incompatible or if 'index' is out of bounds. There is also a
		/// const version of this method that returns a const reference.
		/// </summary>
		/// <param name="index">The index at which to get a value.</param>
		/// <returns>A reference to the value at index 'index'.</returns>
		float& GetFloat(uint32_t index = 0u);

		/// <summary>
		/// See the non-const version of this method.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		const float& GetFloat(uint32_t index = 0u) const;

		/// <summary>
		/// This method returns a reference to the entry in this Datum's array
		/// at index 'index'. An exception is thrown if the type of this Datum
		/// is incompatible or if 'index' is out of bounds. There is also a
		/// const version of this method that returns a const reference.
		/// </summary>
		/// <param name="index">The index at which to get a value.</param>
		/// <returns>A reference to the value at index 'index'.</returns>
		glm::vec4& GetVector(uint32_t index = 0u);

		/// <summary>
		/// See the non-const version of this method.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		const glm::vec4& GetVector(uint32_t index = 0u) const;

		/// <summary>
		/// This method returns a reference to the entry in this Datum's array
		/// at index 'index'. An exception is thrown if the type of this Datum
		/// is incompatible or if 'index' is out of bounds. There is also a
		/// const version of this method that returns a const reference.
		/// </summary>
		/// <param name="index">The index at which to get a value.</param>
		/// <returns>A reference to the value at index 'index'.</returns>
		glm::mat4& GetMatrix(uint32_t index = 0u);

		/// <summary>
		/// See the non-const version of this method.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		const glm::mat4& GetMatrix(uint32_t index = 0u) const;

		/// <summary>
		/// This method returns a reference to the entry in this Datum's array
		/// at index 'index'. An exception is thrown if the type of this Datum
		/// is incompatible or if 'index' is out of bounds. There is also a
		/// const version of this method.
		/// </summary>
		/// <param name="index">The index at which to get a value.</param>
		/// <returns>A reference to the value at index 'index'.</returns>
		Scope& GetScope(uint32_t index = 0u);

		/// <summary>
		/// See the non-const version of this method.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		const Scope& GetScope(uint32_t index = 0u) const;

		/// <summary>
		/// This method returns a reference to the entry in this Datum's array
		/// at index 'index'. An exception is thrown if the type of this Datum
		/// is incompatible or if 'index' is out of bounds. There is also a
		/// const version of this method that returns a const reference.
		/// </summary>
		/// <param name="index">The index at which to get a value.</param>
		/// <returns>A reference to the value at index 'index'.</returns>
		std::string& GetString(uint32_t index = 0u);

		/// <summary>
		/// See the non-const version of this method.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		const std::string& GetString(uint32_t index = 0u) const;

		/// <summary>
		/// This method returns a reference to the entry in this Datum's array
		/// at index 'index'. An exception is thrown if the type of this Datum
		/// is incompatible or if 'index' is out of bounds. There is also a
		/// const version of this method.
		/// </summary>
		/// <param name="index">The index at which to get a value.</param>
		/// <returns>A reference to the value at index 'index'.</returns>
		RTTI*& GetPointer(uint32_t index = 0u);

		/// <summary>
		/// See the non-const version of this method.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		RTTI*& GetPointer(uint32_t index = 0u) const;

		/// <summary>
		/// This method takes in a string and attempts to convert it to a value
		/// of the type stored in this Datum. If the conversion is unsuccessful, an exception is thrown;
		/// if the type is UNKNOWN, this method returns false. This value is
		/// then stored in the array at index 'index'. If the index is out of
		/// bounds, this method returns false.
		/// </summary>
		/// <param name="str">- A string representation of the value to store.
		/// </param>
		/// <param name="index">- The index at which to store the value obtained
		/// from 'str'.</param>
		/// <returns>True if the string conversion is successful and 'index' is
		/// within bounds; false otherwise.</returns>
		bool SetFromString(const std::string& str, uint32_t index = 0u);

		/// <summary>
		/// This method converts the array entry at index 'index' into a string.
		/// An exception is thrown if the index is out of bounds.
		/// </summary>
		/// <param name="index">- The index of the thing to convert to a string.
		/// </param>
		/// <returns>The result of the conversion to a string.</returns>
		std::string ToString(uint32_t index = 0u) const;

		/// <summary>
		/// This method is syntactic sugar. It allows a user to get the index-th
		/// Scope in this Datum. If this Datum is of a type other than TABLE, or
		/// if the index is out of bounds, an exception is thrown.
		/// </summary>
		/// <param name="index">- The index of the Scope to which a reference
		/// should be returned.</param>
		/// <returns>A reference to the Scope at index 'index'.</returns>
		Scope& operator[](uint32_t index);
		
	private:

		const uint32_t mCapacityIncreaseAmount = 8u;

		/// <summary>
		/// The values that are of type 'mType'.
		/// </summary>
		ValuesType mValues;

		/// <summary>
		/// Indicates what type of data are stored in mValues.
		/// </summary>
		DatumType mType;

		/// <summary>
		/// This is the number of elements in the array pointed to by mValues.
		/// </summary>
		uint32_t mSize;

		/// <summary>
		/// This is the number of elements that can currently be stored in the array
		/// pointed to by mValues.
		/// </summary>
		uint32_t mCapacity;

		/// <summary>
		/// A bool representing whether the data is internal (true) or external
		/// (false).
		/// </summary>
		bool mInternal;

		/// <summary>
		/// This helper method indicates whether this Datum has internal data.
		/// </summary>
		/// <returns>True if mInternal is true and the capacity is greater than
		/// zero; false otherwise.</returns>
		bool HasInternalData() const;

		/// <summary>
		/// This helper method performs a deep copy of the data in 'other'.
		/// </summary>
		/// <param name="other">- A reference to the Datum from which to deep
		/// copy data.</param>
		void DeepCopy(const Datum& other);

		/// <summary>
		/// This private helper method frees (and destructs, if necessary) this
		/// Datum's data.
		/// </summary>
		void FreeData();

		/// <summary>
		/// This method simply returns the result of sizeof() for whatever the
		/// type is.
		/// </summary>
		/// <returns>The size of this Datum's type in bytes.</returns>
		uint32_t SizeOfType() const;
	
		/// <summary>
		/// This private helper method is called by all the public overloaded
		/// versions of SetStorage. It sets mValues to 'arr' and mSize and
		/// mCapacity to 'size'.
		/// </summary>
		/// <param name="arr"></param>
		/// <param name="size"></param>
		void SetStorage(std::byte* arr, uint32_t size);
	};
}