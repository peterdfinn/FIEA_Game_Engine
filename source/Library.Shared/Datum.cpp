#include "pch.h"
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_CXX98
#include "Datum.h"
#include "Scope.h"
#include "glm\gtx\string_cast.hpp"

using namespace std;
using namespace glm;

namespace LibraryShared
{
	Datum::Datum() : mType(DatumType::UNKNOWN), mCapacity(0u), mSize(0u), mInternal(true)
	{
		mValues.i = nullptr;
	}

	Datum::Datum(const Datum& other) : mType(other.mType), mCapacity(0u)
	{
		operator=(other);
	}

	Datum::Datum(Datum&& other) : mSize(other.mSize), mType(other.mType),
		mInternal(other.mInternal), mCapacity(other.mCapacity)
	{
		other.mInternal = false;
		mValues.bp = other.mValues.bp;
	}

	Datum::Datum(const initializer_list<int32_t>& list) : mCapacity(0u),
		mType(DatumType::INTEGER)
	{
		operator=(list);
	}

	Datum::Datum(const initializer_list<float>& list) : mCapacity(0u),
		mType(DatumType::FLOAT)
	{
		operator=(list);
	}

	Datum::Datum(const initializer_list<vec4>& list) : mCapacity(0u),
		mType(DatumType::VECTOR)
	{
		operator=(list);
	}

	Datum::Datum(const initializer_list<mat4>& list) : mCapacity(0u),
		mType(DatumType::MATRIX)
	{
		operator=(list);
	}

	Datum::Datum(const initializer_list<Scope*>& list) : mCapacity(0u),
		mType(DatumType::TABLE)
	{
		operator=(list);
	}

	Datum::Datum(const initializer_list<string>& list) : mCapacity(0u),
		mType(DatumType::STRING)
	{
		operator=(list);
	}

	Datum::Datum(const initializer_list<RTTI*>& list) : mCapacity(0u),
		mType(DatumType::POINTER)
	{
		operator=(list);
	}

	Datum::~Datum()
	{
		if (HasInternalData())
		{
			FreeData();
		}
	}

	Datum& Datum::operator=(const initializer_list<int32_t>& list)
	{
		/*We can only assign to this Datum if either it has never been filled
		with data or if it is already of type INTEGER.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::INTEGER))
		{
			throw exception("Cannot assign to this Datum; it must be empty or of\
				the same type as the initializer list.");
		}

		/*If there is any internal data already in this Datum, it must be freed.*/
		if (HasInternalData())
		{
			FreeData();
		}

		/*Now that we know this Datum no longer has any internal data hanging
		around in memory, we can assign to mValues.i and update other member
		variables accordingly.*/
		mInternal = true;
		mSize = static_cast<uint32_t>(list.size());
		mCapacity = static_cast<uint32_t>(list.size());
		mType = DatumType::INTEGER;
		mValues.i = static_cast<int32_t*>(malloc(list.size() * sizeof(int32_t)));
		uint32_t index = 0u;
		for (int32_t entry : list)
		{
			mValues.i[index++] = entry;
		}

		return *this;
	}

	Datum& Datum::operator=(const initializer_list<float>& list)
	{
		/*We can only assign to this Datum if either it has never been filled
		with data or if it is already of type FLOAT.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::FLOAT))
		{
			throw exception("Cannot assign to this Datum; it must be empty or of\
				the same type as the initializer list.");
		}

		/*If there is any internal data already in this Datum, it must be freed.*/
		if (HasInternalData())
		{
			FreeData();
		}

		/*Now that we know this Datum no longer has any internal data hanging
		around in memory, we can assign to mValues.f and update other member
		variables accordingly.*/
		mInternal = true;
		mSize = static_cast<uint32_t>(list.size());
		mCapacity = static_cast<uint32_t>(list.size());
		mType = DatumType::FLOAT;
		mValues.f = static_cast<float*>(malloc(list.size() * sizeof(float)));
		uint32_t index = 0u;
		for (float entry : list)
		{
			mValues.f[index++] = entry;
		}

		return *this;
	}

	Datum& Datum::operator=(const initializer_list<vec4>& list)
	{
		/*We can only assign to this Datum if either it has never been filled
		with data or if it is already of type VECTOR.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::VECTOR))
		{
			throw exception("Cannot assign to this Datum; it must be empty or of\
				the same type as the initializer list.");
		}

		/*If there is any internal data already in this Datum, it must be
		destructed and freed.*/
		if (HasInternalData())
		{
			FreeData();
		}

		/*Now that we know this Datum no longer has any internal data hanging
		around in memory, we can assign to mValues.v and update other member
		variables accordingly.*/
		mInternal = true;
		mSize = static_cast<uint32_t>(list.size());
		mCapacity = static_cast<uint32_t>(list.size());
		mType = DatumType::VECTOR;
		mValues.v = static_cast<vec4*>(malloc(list.size() * sizeof(vec4)));
		uint32_t index = 0u;
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			new (mValues.v + index++) vec4(*it);
		}

		return *this;
	}

	Datum& Datum::operator=(const initializer_list<mat4>& list)
	{
		/*We can only assign to this Datum if either it has never been filled
		with data or if it is already of type MATRIX.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::MATRIX))
		{
			throw exception("Cannot assign to this Datum; it must be empty or of\
				the same type as the initializer list.");
		}

		/*If there is any internal data already in this Datum, it must be
		destructed and freed.*/
		if (HasInternalData())
		{
			FreeData();
		}

		/*Now that we know this Datum no longer has any internal data hanging
		around in memory, we can assign to mValues.m and update other member
		variables accordingly.*/
		mInternal = true;
		mSize = static_cast<uint32_t>(list.size());
		mCapacity = static_cast<uint32_t>(list.size());
		mType = DatumType::MATRIX;
		mValues.m = static_cast<mat4*>(malloc(list.size() * sizeof(mat4)));
		uint32_t index = 0u;
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			new (mValues.m + index++) mat4(*it);
		}

		return *this;
	}

	Datum& Datum::operator=(const initializer_list<Scope*>& list)
	{
		/*We can only assign to this Datum if either it has never been filled
		with data or if it is already of type MATRIX.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::TABLE))
		{
			throw exception("Cannot assign to this Datum; it must be empty or of\
				the same type as the initializer list.");
		}

		/*If there is any internal data already in this Datum, it must be
		destructed and freed.*/
		if (HasInternalData())
		{
			FreeData();
		}

		/*Now that we know this Datum no longer has any internal data hanging
		around in memory, we can assign to mValues.scope and update other member
		variables accordingly.*/
		mInternal = true;
		mSize = static_cast<uint32_t>(list.size());
		mCapacity = static_cast<uint32_t>(list.size());
		mType = DatumType::TABLE;
		mValues.scope = reinterpret_cast<Scope**>(malloc(list.size() * sizeof(Scope*)));
		uint32_t index = 0u;
		for (Scope* entry : list)
		{
			mValues.scope[index++] = entry;
		}

		return *this;
	}

	Datum& Datum::operator=(const initializer_list<string>& list)
	{
		/*We can only assign to this Datum if either it has never been filled
		with data or if it is already of type STRING.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::STRING))
		{
			throw exception("Cannot assign to this Datum; it must be empty or of\
				the same type as the initializer list.");
		}

		/*If there is any internal data already in this Datum, it must be
		destructed and freed.*/
		if (HasInternalData())
		{
			FreeData();
		}

		/*Now that we know this Datum no longer has any internal data hanging
		around in memory, we can assign to mValues.s and update other member
		variables accordingly.*/
		mInternal = true;
		mSize = static_cast<uint32_t>(list.size());
		mCapacity = static_cast<uint32_t>(list.size());
		mType = DatumType::STRING;
		mValues.s = static_cast<string*>(malloc(list.size() * sizeof(string)));
		uint32_t index = 0u;
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			new (mValues.s + index++) string(*it);
		}

		return *this;
	}

	Datum& Datum::operator=(const initializer_list<RTTI*>& list)
	{
		/*We can only assign to this Datum if either it has never been filled
		with data or if it is already of type POINTER.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::POINTER))
		{
			throw exception("Cannot assign to this Datum; it must be empty or of\
				the same type as the initializer list.");
		}

		/*If there is any internal data already in this Datum, it must be freed.*/
		if (HasInternalData())
		{
			FreeData();
		}

		/*Now that we know this Datum no longer has any internal data hanging
		around in memory, we can assign to mValues.p and update other member
		variables accordingly.*/
		mInternal = true;
		mSize = static_cast<uint32_t>(list.size());
		mCapacity = static_cast<uint32_t>(list.size());
		mType = DatumType::POINTER;
		mValues.p = reinterpret_cast<RTTI**>(malloc(list.size() * sizeof(RTTI*)));
		uint32_t index = 0u;
		for (RTTI* entry : list)
		{
			mValues.p[index++] = entry;
		}

		return *this;
	}

	Datum& Datum::operator=(const Datum& other)
	{
		/*Check for self-assignment.*/
		if (&other != this)
		{
			/*If this Datum is of any type other than UNKNOWN or other's type,
			then throw an exception.*/
			if ((mType != DatumType::UNKNOWN) && (mType != other.mType))
			{
				throw exception("Cannot assign to this Datum: it must be either\
					unassigned or of the same type as the right-hand side of the\
					assignment statement.");
			}

			/*If this Datum has internal data, it must be freed, and possibly
			destructed, before we can assign other's data to it.*/
			if (HasInternalData())
			{
				FreeData();
			}

			/*Now, we can make this Datum contain other's data and set its
			member variables appropriately.*/
			mCapacity = other.mCapacity;
			mSize = other.mSize;
			mInternal = other.mInternal;
			mType = other.mType;
			if (!other.mInternal)
			{
				mValues = other.mValues;
			}
			else
			{
				DeepCopy(other);
			}
		}
		return *this;
	}

	Datum& Datum::operator=(Datum&& other)
	{
		/*Check for self-assignment.*/
		if (&other != this)
		{
			/*If this Datum has a type other than UNKNOWN or other's data type,
			throw an exception.*/
			if ((mType != DatumType::UNKNOWN) && (mType != other.mType))
			{
				throw exception("This Datum cannot be assigned with that Datum:\
					this Datum must have never had data, or it must have the\
					same type as the other Datum.");
			}

			/*If this Datum has internal data, it must first be freed (and
			in some cases destructed) before new data can be assigned.*/
			if (HasInternalData())
			{
				FreeData();
			}

			/*Now that we know this Datum has no internal data hanging around,
			we must assign other's data to this Datum and set member variables
			appropriately.*/
			mValues.bp = other.mValues.bp;
			mInternal = other.mInternal;
			mType = other.mType;
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			other.mInternal = false;
		}
		return *this;
	}

	Datum& Datum::operator=(int32_t input)
	{
		/*If this Datum is of a type other than UNKNOWN or INTEGER, throw an
		exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::INTEGER))
		{
			throw exception("Cannot assign an integer to this Datum: it has data\
				of a type other than integer!");
		}

		/*If doing this would change the size of the array of data, and the data
		is external, throw an exception.*/
		if (!mInternal && (mSize < 1u))
		{
			throw exception("Cannot assign to this Datum: it would resize\
				external data!");
		}

		mType = DatumType::INTEGER;

		/*If the size is at least 1, simply assign the value and be done with
		it.*/
		if (mSize >= 1u)
		{
			mValues.i[0] = input;
		}
		else
		{
			PushFront(input);
		}

		return *this;
	}

	Datum& Datum::operator=(float input)
	{
		/*If this Datum is of a type other than UNKNOWN or FLOAT, throw an
		exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::FLOAT))
		{
			throw exception("Cannot assign an integer to this Datum: it has data\
				of a type other than float!");
		}

		/*If doing this would change the size of the array of data, and the data
		is external, throw an exception.*/
		if (!mInternal && (mSize < 1u))
		{
			throw exception("Cannot assign to this Datum: it would resize\
				external data!");
		}

		mType = DatumType::FLOAT;

		/*If the size is at least 1, simply assign the value and be done with
		it.*/
		if (mSize >= 1u)
		{
			mValues.f[0] = input;
		}
		else
		{
			PushFront(input);
		}

		return *this;
	}

	Datum& Datum::operator=(vec4 input)
	{
		/*If this Datum is of a type other than UNKNOWN or VECTOR, throw an
		exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::VECTOR))
		{
			throw exception("Cannot assign an integer to this Datum: it has data\
				of a type other than vector!");
		}

		/*If doing this would change the size of the array of data, and the data
		is external, throw an exception.*/
		if (!mInternal && (mSize < 1u))
		{
			throw exception("Cannot assign to this Datum: it would resize\
				external data!");
		}

		mType = DatumType::VECTOR;

		if (mSize >= 1u)
		{
			mValues.v[0] = input;
		}
		else
		{
			PushFront(input);
		}

		return *this;
	}

	Datum& Datum::operator=(mat4 input)
	{
		/*If this Datum is of a type other than UNKNOWN or MATRIX, throw an
		exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::MATRIX))
		{
			throw exception("Cannot assign an integer to this Datum: it has data\
				of a type other than matrix!");
		}

		/*If doing this would change the size of the array of data, and the data
		is external, throw an exception.*/
		if (!mInternal && (mSize < 1u))
		{
			throw exception("Cannot assign to this Datum: it would resize\
				external data!");
		}

		mType = DatumType::MATRIX;

		/*If the size is at least 1, simply assign the new input and be done
		with it.*/
		if (mSize >= 1u)
		{
			mValues.m[0] = input;
		}
		else
		{
			PushFront(input);
		}

		return *this;
	}

	Datum& Datum::operator=(Scope& input)
	{
		/*If this Datum is of a type other than UNKNOWN or POINTER, throw an
		exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::TABLE))
		{
			throw exception("Cannot assign an integer to this Datum: it has data\
				of a type other than Scope pointer!");
		}

		/*If doing this would change the size of the array of data, and the data
		is external, throw an exception.*/
		if (!mInternal && (mSize < 1u))
		{
			throw exception("Cannot assign to this Datum: it would resize\
				external data!");
		}

		mType = DatumType::TABLE;

		/*If the size is at least 1, simply assign the new input and be done
		with it.*/
		if (mSize >= 1u)
		{
			mValues.scope[0] = &input;
		}
		else
		{
			PushFront(input);
		}

		return *this;
	}

	Datum& Datum::operator=(const string& input)
	{
		/*If this Datum is of a type other than UNKNOWN or STRING, throw an
		exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::STRING))
		{
			throw exception("Cannot assign an integer to this Datum: it has data\
				of a type other than string!");
		}

		/*If doing this would change the size of the array of data, and the data
		is external, throw an exception.*/
		if (!mInternal && (mSize < 1u))
		{
			throw exception("Cannot assign to this Datum: it would resize\
				external data!");
		}

		mType = DatumType::STRING;
		if (mSize >= 1u)
		{
			mValues.s[0] = input;
		}
		else
		{
			PushFront(input);
		}
		return *this;
	}

	Datum& Datum::operator=(RTTI* input)
	{
		/*If this Datum is of a type other than UNKNOWN or POINTER, throw an
		exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::POINTER))
		{
			throw exception("Cannot assign an integer to this Datum: it has data\
				of a type other than pointer!");
		}

		/*If doing this would change the size of the array of data, and the data
		is external, throw an exception.*/
		if (!mInternal && (mSize < 1u))
		{
			throw exception("Cannot assign to this Datum: it would resize\
				external data!");
		}

		mType = DatumType::POINTER;

		/*If the size is at least 1, simply assign the new input and be done
		with it.*/
		if (mSize >= 1u)
		{
			mValues.p[0] = input;
		}
		else
		{
			PushFront(input);
		}

		return *this;
	}

	void Datum::ShrinkToFit()
	{
		if (!mInternal)
		{
			throw exception("Cannot call ShrinkToFit on this Datum: its data\
				are external!");
		}

		if (mCapacity > mSize)
		{
			if (mType == DatumType::STRING)
			{
				string* temp = reinterpret_cast<string*>(malloc(sizeof(string) * mSize));
				for (uint32_t i = 0u; i < mSize; ++i)
				{
					new (&temp[i]) string(move(mValues.s[i]));
				}
				FreeData();
				mValues.s = temp;
			}
			else
			{
				mValues.bp = reinterpret_cast<std::byte*>(realloc(mValues.bp, mSize * SizeOfType()));
			}
			mCapacity = mSize;
		}
	}

	bool Datum::operator==(const Datum& other) const
	{
		if ((mType != other.mType) || (mSize != other.mSize))
		{
			return false;
		}

		if (mType == DatumType::STRING)
		{
			for (uint32_t i = 0u; i < mSize; ++i)
			{
				if (mValues.s[i] != other.mValues.s[i])
				{
					return false;
				}
			}
		}
		else if (mType == DatumType::POINTER)
		{
			for (uint32_t i = 0u; i < mSize; ++i)
			{
				if ((mValues.p[i] == nullptr) || (!mValues.p[i]->Equals(other.mValues.p[i])))
				{
					return false;
				}
			}
		}
		else if (mType == DatumType::TABLE)
		{
			for (uint32_t i = 0u; i < mSize; ++i)
			{
				if ((mValues.scope[i] == nullptr) || (!mValues.scope[i]->Equals(other.mValues.scope[i])))
				{
					return false;
				}
			}
		}
		else
		{
			uint32_t bytes = mSize * SizeOfType();
			int ret = memcmp(mValues.i, other.mValues.i, bytes);
			return (ret == 0);
		}

		return true;
	}

	bool Datum::operator!=(const Datum& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(int32_t other) const
	{
		if ((mType != DatumType::INTEGER) || (IsEmpty()))
		{
			return false;
		}
		return (other == mValues.i[0]);
	}

	bool Datum::operator!=(int32_t other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(float other) const
	{
		if ((mType != DatumType::FLOAT) || (IsEmpty()))
		{
			return false;
		}
		return (other == mValues.f[0]);
	}

	bool Datum::operator!=(float other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(vec4 other) const
	{
		if ((mType != DatumType::VECTOR) || (IsEmpty()))
		{
			return false;
		}
		return (other == mValues.v[0]);
	}

	bool Datum::operator!=(vec4 other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(mat4 other) const
	{
		if ((mType != DatumType::MATRIX) || (IsEmpty()))
		{
			return false;
		}
		return (other == mValues.m[0]);
	}

	bool Datum::operator!=(mat4 other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(Scope& other) const
	{
		if ((mType != DatumType::TABLE) || IsEmpty())
		{
			return false;
		}
		return other.Equals(mValues.scope[0]);
	}

	bool Datum::operator!=(Scope& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const string& other) const
	{
		if ((mType != DatumType::STRING) || (IsEmpty()))
		{
			return false;
		}
		return (other == mValues.s[0]);
	}

	bool Datum::operator!=(const string& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(RTTI* other) const
	{
		if ((mType != DatumType::POINTER) || (IsEmpty()))
		{
			return false;
		}
		return other->Equals(mValues.p[0]);
	}

	bool Datum::operator!=(RTTI* other) const
	{
		return !operator==(other);
	}

	void Datum::PushBack(int32_t input)
	{
		if ((mType != DatumType::INTEGER) && (mType != DatumType::UNKNOWN))
		{
			throw exception("Cannot push this input on this Datum: its type is\
				incompatible!");
		}
		if (!mInternal)
		{
			throw exception("Cannot push back on this Datum: its storage is\
				external!");
		}

		mType = DatumType::INTEGER;
		if (mCapacity == mSize)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		mValues.i[mSize++] = input;
	}

	void Datum::PushBack(float input)
	{
		if ((mType != DatumType::FLOAT) && (mType != DatumType::UNKNOWN))
		{
			throw exception("Cannot push this input on this Datum: its type is\
				incompatible!");
		}
		if (!mInternal)
		{
			throw exception("Cannot push back on this Datum: its storage is\
				external!");
		}

		mType = DatumType::FLOAT;
		if (mCapacity == mSize)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		mValues.f[mSize++] = input;
	}

	void Datum::PushBack(vec4 input)
	{
		if ((mType != DatumType::VECTOR) && (mType != DatumType::UNKNOWN))
		{
			throw exception("Cannot push this input on this Datum: its type is\
				incompatible!");
		}
		if (!mInternal)
		{
			throw exception("Cannot push back on this Datum: its storage is\
				external!");
		}

		mType = DatumType::VECTOR;
		if (mCapacity == mSize)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		mValues.v[mSize++] = input;
	}

	void Datum::PushBack(mat4 input)
	{
		if ((mType != DatumType::MATRIX) && (mType != DatumType::UNKNOWN))
		{
			throw exception("Cannot push this input on this Datum: its type is\
				incompatible!");
		}
		if (!mInternal)
		{
			throw exception("Cannot push back on this Datum: its storage is\
				external!");
		}

		mType = DatumType::MATRIX;
		if (mCapacity == mSize)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		mValues.m[mSize++] = input;
	}

	void Datum::PushBack(Scope& input)
	{
		if ((mType != DatumType::TABLE) && (mType != DatumType::UNKNOWN))
		{
			throw exception("Cannot push this input on this Datum: its type is\
				incompatible!");
		}
		if (!mInternal)
		{
			throw exception("Cannot push back on this Datum: its storage is\
				external!");
		}

		mType = DatumType::TABLE;
		if (mCapacity == mSize)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		mValues.scope[mSize++] = &input;
	}

	void Datum::PushBack(const string& input)
	{
		if ((mType != DatumType::STRING) && (mType != DatumType::UNKNOWN))
		{
			throw exception("Cannot push this input on this Datum: its type is\
				incompatible!");
		}
		if (!mInternal)
		{
			throw exception("Cannot push back on this Datum: its storage is\
				external!");
		}

		mType = DatumType::STRING;
		if (mCapacity == mSize)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		new (mValues.s + mSize++) string(input);
	}

	void Datum::PushBack(RTTI* input)
	{
		if ((mType != DatumType::POINTER) && (mType != DatumType::UNKNOWN))
		{
			throw exception("Cannot push this input on this Datum: its type is\
				incompatible!");
		}
		if (!mInternal)
		{
			throw exception("Cannot push back on this Datum: its storage is\
				external!");
		}

		mType = DatumType::POINTER;
		if (mCapacity == mSize)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		mValues.p[mSize++] = input;
	}

	void Datum::PopBack()
	{
		if (!mInternal)
		{
			throw exception("Cannot pop back of this Datum: its storage is\
				external!");
		}
		if (mSize == 0u)
		{
			throw exception("Cannot pop back of this Datum: it is empty!");
		}

		/*Now we must take different action depending on the type of this
		Datum.*/
		if (mType == DatumType::STRING)
		{
			mValues.s[--mSize].~string();
		}
		else
		{
			--mSize;
		}
	}

	void Datum::PushFront(int32_t input)
	{
		/*If the type is not UNKNOWN or INTEGER, throw an exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::INTEGER))
		{
			throw exception("Cannot push front on this Datum: its type is\
				incompatible!");
		}

		/*If the data are external, throw an exception.*/
		if (!mInternal)
		{
			throw exception("Cannot push front on this Datum: its data are\
				external!");
		}

		mType = DatumType::INTEGER;
		/*If there isn't room in memory for this new input, we must first
		reserve new memory. Then we can safely insert the new memory.*/
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		rsize_t bytesToMove = mSize * sizeof(int32_t);
		memmove_s(mValues.i + 1, bytesToMove, mValues.i, bytesToMove);
		mValues.i[0] = input;
		++mSize;
	}

	void Datum::PushFront(float input)
	{
		/*If the type is not UNKNOWN or FLOAT, throw an exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::FLOAT))
		{
			throw exception("Cannot push front on this Datum: its type is\
				incompatible!");
		}

		/*If the data are external, throw an exception.*/
		if (!mInternal)
		{
			throw exception("Cannot push front on this Datum: its data are\
				external!");
		}
		
		mType = DatumType::FLOAT;
		/*If there isn't room in memory for this new input, we must first
		reserve new memory. Then we can safely insert the new memory.*/
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		rsize_t bytesToMove = mSize * sizeof(float);
		memmove_s(mValues.f + 1, bytesToMove, mValues.f, bytesToMove);
		mValues.f[0] = input;
		++mSize;
	}

	void Datum::PushFront(vec4 input)
	{
		/*If the type is not UNKNOWN or VECTOR, throw an exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::VECTOR))
		{
			throw exception("Cannot push front on this Datum: its type is\
				incompatible!");
		}

		/*If the data are external, throw an exception.*/
		if (!mInternal)
		{
			throw exception("Cannot push front on this Datum: its data are\
				external!");
		}

		mType = DatumType::VECTOR;
		/*If there isn't room in memory for this new input, we must first
		reserve new memory. Then we can safely insert the new memory.*/
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		rsize_t bytesToMove = mSize * sizeof(vec4);
		memmove_s(mValues.v + 1, bytesToMove, mValues.v, bytesToMove);
		new (&mValues.v[0]) vec4(input);
		++mSize;
	}

	void Datum::PushFront(mat4 input)
	{
		/*If the type is not UNKNOWN or MATRIX, throw an exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::MATRIX))
		{
			throw exception("Cannot push front on this Datum: its type is\
				incompatible!");
		}

		/*If the data are external, throw an exception.*/
		if (!mInternal)
		{
			throw exception("Cannot push front on this Datum: its data are\
				external!");
		}

		mType = DatumType::MATRIX;
		/*If there isn't room in memory for this new input, we must first
		reserve new memory. Then we can safely insert the new memory.*/
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		rsize_t bytesToMove = mSize * sizeof(mat4);
		memmove_s(mValues.m + 1, bytesToMove, mValues.m, bytesToMove);
		new (&mValues.m[0]) mat4(input);
		++mSize;
	}

	void Datum::PushFront(Scope& input)
	{
		/*If the type is not UNKNOWN or TABLE, throw an exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::TABLE))
		{
			throw exception("Cannot push front on this Datum: its type is\
				incompatible!");
		}

		/*If the data are external, throw an exception.*/
		if (!mInternal)
		{
			throw exception("Cannot push front on this Datum: its data are\
				external!");
		}

		mType = DatumType::TABLE;
		/*If there isn't room in memory for this new input, we must first
		reserve new memory. Then we can safely insert the new memory.*/
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		rsize_t bytesToMove = mSize * sizeof(Scope*);
		memmove_s(mValues.scope + 1, bytesToMove, mValues.scope, bytesToMove);
		mValues.scope[0] = move(&input);
		++mSize;
	}

	void Datum::PushFront(const string& input)
	{
		/*If the type is not UNKNOWN or STRING, throw an exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::STRING))
		{
			throw exception("Cannot push front on this Datum: its type is\
				incompatible!");
		}

		/*If the data are external, throw an exception.*/
		if (!mInternal)
		{
			throw exception("Cannot push front on this Datum: its data are\
				external!");
		}

		mType = DatumType::STRING;
		/*If there isn't room in memory for this new input, we must first
		reserve new memory. Then we can safely insert the new memory.*/
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		if (mSize > 0u)
		{
			for (size_t i = mSize; i > 0; --i)
			{
				new (&mValues.s[i]) string(move(mValues.s[i - 1]));
				mValues.s[i - 1].~string();
			}
		}
		new (&mValues.s[0]) string(input);
		++mSize;
	}

	void Datum::PushFront(RTTI* input)
	{
		/*If the type is not UNKNOWN or POINTER, throw an exception.*/
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::POINTER))
		{
			throw exception("Cannot push front on this Datum: its type is\
				incompatible!");
		}

		/*If the data are external, throw an exception.*/
		if (!mInternal)
		{
			throw exception("Cannot push front on this Datum: its data are\
				external!");
		}

		mType = DatumType::POINTER;
		/*If there isn't room in memory for this new input, we must first
		reserve new memory. Then we can safely insert the new memory.*/
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacityIncreaseAmount);
		}
		rsize_t bytesToMove = mSize * sizeof(RTTI*);
		memmove_s(mValues.p + 1, bytesToMove, mValues.p, bytesToMove);
		mValues.p[0] = move(input);
		++mSize;
	}

	void Datum::PopFront()
	{
		if (!mInternal)
		{
			throw exception("Cannot pop back of this Datum: its storage is\
				external!");
		}
		if (mSize == 0u)
		{
			throw exception("Cannot pop back of this Datum: it is empty!");
		}

		/*Now we must take different action depending on the type of this
		Datum.*/
		if (mType == DatumType::STRING)
		{
			mValues.s[0].~string();
			--mSize;
			for (uint32_t i = 0u; i < mSize; ++i)
			{
				mValues.s[i] = std::move(mValues.s[i + 1]);
				mValues.s[i + 1].~string();
			}
		}
		else
		{
			rsize_t bytesToMove = --mSize * SizeOfType();
			memmove_s(mValues.bp, bytesToMove, mValues.bp + SizeOfType(), bytesToMove);
		}
	}

	DatumType Datum::Type() const
	{
		return mType;
	}

	bool Datum::SetType(DatumType type)
	{
		if ((mType == DatumType::UNKNOWN) || (mType == type))
		{
			mType = type;
			return true;
		}
		
		return false;
	}

	uint32_t Datum::Size() const
	{
		return mSize;
	}

	bool Datum::IsInternal() const
	{
		return mInternal;
	}

	void Datum::SetSize(uint32_t newSize)
	{
		/*If the data are external, throw an exception.*/
		if (!mInternal)
		{
			throw exception("Cannot call SetSize on this Datum: its data are\
				external!");
		}

		/*If the type is UNKNOWN, throw an exception.*/
		if (mType == DatumType::UNKNOWN)
		{
			throw exception("Type is undefined!");
		}

		/*If the new size is less than the current size, then we may have to
		call the destructor of each current array entry that will no longer be
		in the array.*/
		if ((newSize < mSize)/* && (mType == DatumType::STRING)*/)
		{
			for (uint32_t i = newSize; i < mSize; ++i)
			{
				if (mType == DatumType::STRING)
				{
					mValues.s[i].~string();
				}
				else if (mType == DatumType::TABLE)
				{
					delete mValues.scope[i];
				}
			}
		}

		/*If the new size is greater than the current capacity, then we must
		call Reserve.*/
		if (newSize > mCapacity)
		{
			Reserve(newSize);
		}

		/*Now we must default construct any new entries in the array.*/
		if (mType == DatumType::STRING)
		{
			for (uint32_t i = mSize; i < newSize; ++i)
			{
				new (mValues.s + i) string();
			}
		}

		/*Now, finally, we are ready to actually set the value of mSize.*/
		mSize = newSize;
	}

	uint32_t Datum::Capacity() const
	{
		return mCapacity;
	}

	void Datum::Clear()
	{
		if (HasInternalData())
		{
			FreeData();
		}
		mSize = 0u;
		mCapacity = 0u;
		mType = DatumType::UNKNOWN;
	}

	bool Datum::IsEmpty() const
	{
		return (mSize == 0u);
	}

	void Datum::Reserve(size_t newCapacity)
	{
		if (!mInternal)
		{
			throw exception("Cannot call Reserve on this Datum: its data are\
				external!");
		}
		if (mCapacity > newCapacity)
		{
			throw exception("Cannot shrink Datum memory with Reserve!");
		}

		if (mCapacity < newCapacity)
		{
			string* temp = nullptr;
			switch (mType)
			{
			case DatumType::INTEGER:
				if (mCapacity > 0u)
				{
					mValues.i = reinterpret_cast<int32_t*>(realloc(mValues.i, newCapacity * sizeof(int32_t)));
				}
				else
				{
					mValues.i = reinterpret_cast<int32_t*>(malloc(newCapacity * sizeof(int32_t)));
				}
				break;
			case DatumType::FLOAT:
				if (mCapacity > 0u)
				{
					mValues.f = reinterpret_cast<float*>(realloc(mValues.f, newCapacity * sizeof(float)));
				}
				else
				{
					mValues.f = reinterpret_cast<float*>(malloc(newCapacity * sizeof(float)));
				}
				break;
			case DatumType::VECTOR:
				if (mCapacity > 0u)
				{
					mValues.v = reinterpret_cast<vec4*>(realloc(mValues.v, newCapacity * sizeof(vec4)));
				}
				else
				{
					mValues.v = reinterpret_cast<vec4*>(malloc(newCapacity * sizeof(vec4)));
				}
				break;
			case DatumType::MATRIX:
				if (mCapacity > 0u)
				{
					mValues.m = reinterpret_cast<mat4*>(realloc(mValues.m, newCapacity * sizeof(mat4)));
				}
				else
				{
					mValues.m = reinterpret_cast<mat4*>(malloc(newCapacity * sizeof(mat4)));
				}
				break;
			case DatumType::TABLE:
				if (mCapacity > 0u)
				{
					mValues.scope = reinterpret_cast<Scope**>(realloc(mValues.scope, newCapacity * sizeof(Scope*)));
				}
				else
				{
					mValues.scope = reinterpret_cast<Scope**>(malloc(newCapacity * sizeof(Scope*)));
				}
				break;
			case DatumType::STRING:
				temp = reinterpret_cast<string*>(malloc(newCapacity * sizeof(string)));
				for (uint32_t i = 0u; i < mSize; ++i)
				{
					new (&temp[i]) string(mValues.s[i]);
					mValues.s[i].~string();
				}
				free(mValues.s);
				mValues.s = temp;
				break;
			case DatumType::POINTER:
				if (mCapacity > 0u)
				{
					mValues.p = reinterpret_cast<RTTI**>(realloc(mValues.p, newCapacity * sizeof(RTTI*)));
				}
				else
				{
					mValues.p = reinterpret_cast<RTTI**>(malloc(newCapacity * sizeof(RTTI*)));
				}
				break;
			default:
				throw exception("Cannot call Reserve on this Datum: its type is\
					invalid or undefined!");
				break;
			}

			mCapacity = static_cast<uint32_t>(newCapacity);
		}
	}

	bool Datum::Remove(int32_t input)
	{
		/*Upfront, we return false if the types don't match.*/
		if (mType != DatumType::INTEGER)
		{
			return false;
		}

		/*First, we must search through the array to find 'input'.*/
		uint32_t i;
		for (i = 0u; i < mSize; ++i)
		{
			if (mValues.i[i] == input)
			{
				break;
			}
		}

		/*If the value was found, we must move all of the following array
		entries back one space and return true.*/
		if (i != mSize)
		{
			RemoveAtIndex(i);
			return true;
		}

		/*Otherwise, we return false.*/
		return false;
	}

	bool Datum::Remove(float input)
	{
		/*Upfront, we return false if the types don't match.*/
		if (mType != DatumType::FLOAT)
		{
			return false;
		}

		/*First, we must search through the array to find 'input'.*/
		uint32_t i;
		for (i = 0u; i < mSize; ++i)
		{
			if (mValues.f[i] == input)
			{
				break;
			}
		}

		/*If the value was found, we must move all of the following array
		entries back one space and return true.*/
		if (i != mSize)
		{
			RemoveAtIndex(i);
			return true;
		}

		/*Otherwise, we return false.*/
		return false;
	}

	bool Datum::Remove(vec4 input)
	{
		/*Upfront, we return false if the types don't match.*/
		if (mType != DatumType::VECTOR)
		{
			return false;
		}

		/*First, we must search through the array to find 'input'.*/
		uint32_t i;
		for (i = 0u; i < mSize; ++i)
		{
			if (mValues.v[i] == input)
			{
				break;
			}
		}

		/*If the value was found, we must move all of the following array
		entries back one space and return true.*/
		if (i != mSize)
		{
			RemoveAtIndex(i);
			return true;
		}

		/*Otherwise, we return false.*/
		return false;
	}

	bool Datum::Remove(mat4 input)
	{
		/*Upfront, we return false if the types don't match.*/
		if (mType != DatumType::MATRIX)
		{
			return false;
		}

		/*First, we must search through the array to find 'input'.*/
		uint32_t i;
		for (i = 0u; i < mSize; ++i)
		{
			if (mValues.m[i] == input)
			{
				break;
			}
		}

		/*If the value was found, we must move all of the following array
		entries back one space and return true.*/
		if (i != mSize)
		{
			RemoveAtIndex(i);
			return true;
		}

		/*Otherwise, we return false.*/
		return false;
	}

	bool Datum::Remove(Scope& input)
	{
		/*Upfront, we return false if the types don't match.*/
		if (mType != DatumType::TABLE)
		{
			return false;
		}

		/*First, we must search through the array to find 'input'.*/
		uint32_t i;
		for (i = 0u; i < mSize; ++i)
		{
			if (mValues.scope[i] == &input)
			{
				break;
			}
		}

		/*If the value was found, we must move all of the following array
		entries back one space and return true.*/
		if (i != mSize)
		{
			RemoveAtIndex(i);
			return true;
		}

		/*Otherwise, we return false.*/
		return false;
	}
	
	bool Datum::Remove(const string& input)
	{
		/*Upfront, we return false if the types don't match.*/
		if (mType != DatumType::STRING)
		{
			return false;
		}

		/*First, we must search through the array to find 'input'.*/
		uint32_t i;
		for (i = 0u; i < mSize; ++i)
		{
			if (mValues.s[i] == input)
			{
				break;
			}
		}

		/*If the value was found, we must move all of the following array
		entries back one space and return true.*/
		if (i != mSize)
		{
			RemoveAtIndex(i);
			return true;
		}

		/*Otherwise, we return false.*/
		return false;
	}

	bool Datum::Remove(RTTI* input)
	{
		/*Upfront, we return false if the types don't match.*/
		if (mType != DatumType::POINTER)
		{
			return false;
		}

		/*First, we must search through the array to find 'input'.*/
		uint32_t i;
		for (i = 0u; i < mSize; ++i)
		{
			if (mValues.p[i] == input)
			{
				break;
			}
		}

		/*If the value was found, we must move all of the following array
		entries back one space and return true.*/
		if (i != mSize)
		{
			RemoveAtIndex(i);
			return true;
		}

		/*Otherwise, we return false.*/
		return false;
	}
	
	void Datum::RemoveAtIndex(uint32_t index)
	{
		/*If index is out of bounds, throw an exception.*/
		if (index >= mSize)
		{
			throw exception("Index out of bounds in RemoveAtIndex!");
		}

		if (mType == DatumType::STRING)
		{/*If the type is STRING, then we have to do something special.*/
			mValues.s[index].~string();
			for (uint i = index + 1u; i < mSize; ++i)
			{
				new (mValues.s + (i - 1)) string(move(mValues.s[i]));
			}
			mValues.s[mSize - 1].~string();
		}
		else
		{/*If we have to remove any of the other types, we can just call a function that takes the size of that type.*/
			rsize_t sizeOfType = SizeOfType();
			rsize_t bytesToMove = sizeOfType * (mSize - index - 1u);
			memmove_s(mValues.bp + index * sizeOfType, bytesToMove,
				mValues.bp + (index + 1) * sizeOfType, bytesToMove);
		}
		--mSize;
	}

	bool Datum::SetStorage(int32_t* arr, uint32_t size)
	{
		if ((mType != DatumType::INTEGER) && (mType != DatumType::UNKNOWN))
		{
			return false;
		}
		SetStorage(reinterpret_cast<std::byte*>(arr), size);
		mType = DatumType::INTEGER;
		return true;
	}

	bool Datum::SetStorage(float* arr, uint32_t size)
	{
		if ((mType != DatumType::FLOAT) && (mType != DatumType::UNKNOWN))
		{
			return false;
		}
		SetStorage(reinterpret_cast<std::byte*>(arr), size);
		mType = DatumType::FLOAT;
		return true;
	}

	bool Datum::SetStorage(vec4* arr, uint32_t size)
	{
		if ((mType != DatumType::VECTOR) && (mType != DatumType::UNKNOWN))
		{
			return false;
		}
		SetStorage(reinterpret_cast<std::byte*>(arr), size);
		mType = DatumType::VECTOR;
		return true;
	}

	bool Datum::SetStorage(mat4* arr, uint32_t size)
	{
		if ((mType != DatumType::MATRIX) && (mType != DatumType::UNKNOWN))
		{
			return false;
		}
		SetStorage(reinterpret_cast<std::byte*>(arr), size);
		mType = DatumType::MATRIX;
		return true;
	}

	bool Datum::SetStorage(string* arr, uint32_t size)
	{
		if ((mType != DatumType::STRING) && (mType != DatumType::UNKNOWN))
		{
			return false;
		}
		SetStorage(reinterpret_cast<std::byte*>(arr), size);
		mType = DatumType::STRING;
		return true;
	}

	bool Datum::SetStorage(RTTI** arr, uint32_t size)
	{
		if ((mType != DatumType::POINTER) && (mType != DatumType::UNKNOWN))
		{
			return false;
		}
		SetStorage(reinterpret_cast<std::byte*>(arr), size);
		mType = DatumType::POINTER;
		return true;
	}

	void Datum::SetStorage(std::byte* arr, uint32_t size)
	{
		/*If this Datum has internal data, we must release it first.*/
		if (HasInternalData())
		{
			FreeData();
		}

		/*Now we can reassign the mValues pointer and change other member
		variables.*/
		mValues.bp = arr;
		mInternal = false;
		mCapacity = size;
		mSize = size;
	}

	bool Datum::Set(int32_t input, uint32_t index)
	{
		if (index >= mSize)
		{
			return false;
		}
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::INTEGER))
		{
			return false;
		}

		mValues.i[index] = input;
		return true;
	}

	bool Datum::Set(float input, uint32_t index)
	{
		if (index >= mSize)
		{
			return false;
		}
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::FLOAT))
		{
			return false;
		}

		mValues.f[index] = input;
		return true;
	}

	bool Datum::Set(vec4 input, uint32_t index)
	{
		if (index >= mSize)
		{
			return false;
		}
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::VECTOR))
		{
			return false;
		}

		mValues.v[index] = input;
		return true;
	}

	bool Datum::Set(mat4 input, uint32_t index)
	{
		if (index >= mSize)
		{
			return false;
		}
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::MATRIX))
		{
			return false;
		}

		mValues.m[index] = input;
		return true;
	}

	bool Datum::Set(Scope& input, uint32_t index)
	{
		if (index >= mSize)
		{
			return false;
		}
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::TABLE))
		{
			return false;
		}

		mValues.scope[index] = &input;
		return true;
	}

	bool Datum::Set(const string& input, uint32_t index)
	{
		if (index >= mSize)
		{
			return false;
		}
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::STRING))
		{
			return false;
		}

		mValues.s[index] = input;
		return true;
	}

	bool Datum::Set(RTTI* input, uint32_t index)
	{
		if (index >= mSize)
		{
			return false;
		}
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::POINTER))
		{
			return false;
		}

		mValues.p[index] = input;
		return true;
	}

	int32_t& Datum::GetInt(uint32_t index)
	{
		if (mType != DatumType::INTEGER)
		{
			throw exception("Incompatible type!");
		}
		if (index >= mSize)
		{
			throw exception("Index out of bounds!");
		}

		return mValues.i[index];
	}

	const int32_t& Datum::GetInt(uint32_t index) const
	{
		return const_cast<Datum*>(this)->GetInt(index);
	}

	float& Datum::GetFloat(uint32_t index)
	{
		if (mType != DatumType::FLOAT)
		{
			throw exception("Incompatible type!");
		}
		if (index >= mSize)
		{
			throw exception("Index out of bounds!");
		}

		return mValues.f[index];
	}

	const float& Datum::GetFloat(uint32_t index) const
	{
		return const_cast<Datum*>(this)->GetFloat(index);
	}

	vec4& Datum::GetVector(uint32_t index)
	{
		if (mType != DatumType::VECTOR)
		{
			throw exception("Incompatible type!");
		}
		if (index >= mSize)
		{
			throw exception("Index out of bounds!");
		}

		return mValues.v[index];
	}

	const vec4& Datum::GetVector(uint32_t index) const
	{
		return const_cast<Datum*>(this)->GetVector(index);
	}

	mat4& Datum::GetMatrix(uint32_t index)
	{
		if (mType != DatumType::MATRIX)
		{
			throw exception("Incompatible type!");
		}
		if (index >= mSize)
		{
			throw exception("Index out of bounds!");
		}

		return mValues.m[index];
	}

	const mat4& Datum::GetMatrix(uint32_t index) const
	{
		return const_cast<Datum*>(this)->GetMatrix(index);
	}

	Scope& Datum::GetScope(uint32_t index)
	{
		if (mType != DatumType::TABLE)
		{
			throw exception("Incompatible type!");
		}
		if (index >= mSize)
		{
			throw exception("Index out of bounds!");
		}

		return *(mValues.scope[index]);
	}

	const Scope& Datum::GetScope(uint32_t index) const
	{
		return (const_cast<Datum*>(this))->GetScope(index);
	}

	string& Datum::GetString(uint32_t index)
	{
		if (mType != DatumType::STRING)
		{
			throw exception("Incompatible type!");
		}
		if (index >= mSize)
		{
			throw exception("Index out of bounds!");
		}

		return mValues.s[index];
	}

	const string& Datum::GetString(uint32_t index) const
	{
		return const_cast<Datum*>(this)->GetString(index);
	}

	RTTI*& Datum::GetPointer(uint32_t index)
	{
		if (mType != DatumType::POINTER)
		{
			throw exception("Incompatible type!");
		}
		if (index >= mSize)
		{
			throw exception("Index out of bounds!");
		}

		return mValues.p[index];
	}

	RTTI*& Datum::GetPointer(uint32_t index) const
	{
		return (const_cast<Datum*>(this))->GetPointer(index);
	}

	bool Datum::SetFromString(const string& str, uint32_t index)
	{
		float floats[16];
		switch (mType)
		{
		case DatumType::UNKNOWN:
			return false;
		case DatumType::INTEGER:
			return Set(stoi(str), index);
		case DatumType::FLOAT:
			return Set(stof(str), index);
		case DatumType::VECTOR:
			sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &floats[0], &floats[1], &floats[2], &floats[3]);
			return Set(vec4(floats[0], floats[1], floats[2], floats[3]), index);
		case DatumType::MATRIX:
			sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&floats[0], &floats[1], &floats[2], &floats[3], &floats[4], &floats[5], &floats[6], &floats[7],
				&floats[8], &floats[9], &floats[10], &floats[11], &floats[12], &floats[13], &floats[14], &floats[15]);
			return Set(mat4(floats[0], floats[1], floats[2], floats[3], floats[4], floats[5], floats[6], floats[7],
				floats[8], floats[9], floats[10], floats[11], floats[12], floats[13], floats[14], floats[15]), index);
		case DatumType::STRING:
			return Set(str, index);
		default:
			break;
		}
		return false;
	}

	string Datum::ToString(uint32_t index) const
	{
		if (index >= mSize)
		{
			return "";
		}

		switch (mType)
		{
		case DatumType::INTEGER:
			return to_string(GetInt(index));
		case DatumType::FLOAT:
			return to_string(GetFloat(index));
		case DatumType::VECTOR:
			return to_string(GetVector(index));
		case DatumType::MATRIX:
			return to_string(GetMatrix(index));
		case DatumType::STRING:
			return GetString(index);
		case DatumType::POINTER:
			return GetPointer(index)->ToString();
		case DatumType::TABLE:
			return GetScope(index).ToString();
		}
		return "";
	}

	Scope& Datum::operator[](uint32_t index)
	{
		return GetScope(index);
	}

	bool Datum::HasInternalData() const
	{
		return mInternal && (mCapacity > 0u);
	}

	void Datum::DeepCopy(const Datum& other)
	{
		rsize_t bytesToCopy;
		switch (other.mType)
		{
		case DatumType::INTEGER:
			bytesToCopy = other.mSize * sizeof(int32_t);
			mValues.i = reinterpret_cast<int32_t*>(malloc(bytesToCopy));
			memcpy_s(mValues.i, bytesToCopy, other.mValues.i,
				bytesToCopy);
			break;
		case DatumType::FLOAT:
			bytesToCopy = other.mSize * sizeof(float);
			mValues.f = reinterpret_cast<float*>(malloc(bytesToCopy));
			memcpy_s(mValues.f, bytesToCopy, other.mValues.f,
				bytesToCopy);
			break;
		case DatumType::VECTOR:
			bytesToCopy = other.mSize * sizeof(glm::vec4);
			mValues.v = reinterpret_cast<vec4*>(malloc(bytesToCopy));
			memcpy_s(mValues.v, bytesToCopy, other.mValues.v,
				bytesToCopy);
			break;
		case DatumType::MATRIX:
			bytesToCopy = other.mSize * sizeof(glm::mat4);
			mValues.m = reinterpret_cast<mat4*>(malloc(bytesToCopy));
			memcpy_s(mValues.m, bytesToCopy, other.mValues.m,
				bytesToCopy);
			break;
		case DatumType::TABLE:
			bytesToCopy = other.mSize * sizeof(Scope*);
			mValues.scope = reinterpret_cast<Scope**>(malloc(bytesToCopy));
			memcpy_s(mValues.scope, bytesToCopy, other.mValues.scope,
				bytesToCopy);
			break;
		case DatumType::STRING:
			bytesToCopy = other.mSize * sizeof(string);
			mValues.s = reinterpret_cast<string*>(malloc(bytesToCopy));
			for (uint32_t i = 0u; i < other.mSize; ++i)
			{
				new (&mValues.s[i]) string(other.mValues.s[i]);
			}
			break;
		case DatumType::POINTER:
			bytesToCopy = other.mSize * sizeof(RTTI*);
			mValues.p = reinterpret_cast<RTTI**>(malloc(bytesToCopy));
			memcpy_s(mValues.p, bytesToCopy, other.mValues.p,
				bytesToCopy);
			break;
		default:
			mValues.i = nullptr;
			break;
		}
	}

	void Datum::FreeData()
	{
		if ((mType != DatumType::UNKNOWN))
		{
			if (mType == DatumType::STRING)
			{
				for (uint32_t i = 0u; i < mSize; ++i)
				{
					mValues.s[i].~string();
				}
			}
			free(mValues.bp);
		}
	}

	uint32_t Datum::SizeOfType() const
	{
		switch (mType)
		{
		case DatumType::INTEGER:
			return sizeof(int32_t);
		case DatumType::FLOAT:
			return sizeof(float);
		case DatumType::VECTOR:
			return sizeof(vec4);
		case DatumType::MATRIX:
			return sizeof(mat4);
		case DatumType::STRING:
			return sizeof(string);
		case DatumType::POINTER:
		case DatumType::TABLE:
			return sizeof(RTTI*);
		default:
			return 0;
		}
	}
}