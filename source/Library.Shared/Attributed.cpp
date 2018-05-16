#include "pch.h"

using namespace std;
using namespace glm;

namespace LibraryShared
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Signature::Signature(const string& name, DatumType type, uint32_t size, uint32_t offset) :
		mName(name), mType(type), mSize(size), mOffset(offset)
	{}

	uint32_t Attributed::Signature::GetSize() const
	{
		return mSize;
	}

	string Attributed::Signature::GetName() const
	{
		return mName;
	}

	uint32_t Attributed::Signature::GetOffset() const
	{
		return mOffset;
	}

	DatumType Attributed::Signature::GetType() const
	{
		return mType;
	}

	bool Attributed::Signature::operator==(const Signature& other) const
	{
		return mName == other.mName;
	}

	Attributed::Attributed(uint32_t initialCapacity) : Scope(initialCapacity)
	{
		RegisterAttribute(Signature("this", DatumType::POINTER, 1u, 0u),
			Attributed::TypeIdClass());
		RegisterBaseType(Attributed::TypeIdClass(), Scope::TypeIdClass());
	}

	Attributed::Attributed(const Attributed& other) : Scope(other)
	{
		operator=(other);
	}

	Attributed::Attributed(Attributed&& other) : Scope(move(other))
	{}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		if (&other != this)
		{
			Scope::operator=(other);
		}
		UpdateExternalStorage(other);
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other)
	{
		if (&other != this)
		{
			Scope::operator=(move(other));
		}
		UpdateExternalStorage(other);
		return *this;
	}

	bool Attributed::IsAttribute(const string& name) const
	{
		const Datum* const found = Find(name);
		if (found == nullptr) return false;
		return (found->Type() != DatumType::TABLE);
	}

	bool Attributed::IsPrescribedAttribute(const string& name) const
	{
		bool returnValue = false;

		auto hashMapIterator =
			TypeManager::mPrescribedMap.Find(TypeIdInstance());
		if (hashMapIterator != TypeManager::mPrescribedMap.end())
		{
			for (const auto& attribute : hashMapIterator->second)
			{
				if (name == attribute.GetName())
				{
					returnValue = true;
					break;
				}
			}
		}

		return returnValue;
	}

	bool Attributed::IsAuxiliaryAttribute(const string& name)
		const
	{
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw exception("Cannot create an auxiliary attribute with this\
				name: it already belongs to a prescribed attribute!");
		}
		return Append(name);
	}

	void Attributed::RegisterAttribute(const Signature& signature,
		uint64_t idOfSelf, bool prescribed, bool external)
	{
		//prescribed;
		const string& name = signature.GetName();
		uint32_t size = signature.GetSize();
		uint32_t offset = signature.GetOffset();

		/*We must insert the Datum into the table, if it's not
		already there. If there was a Datum already present with the same name
		as the prescribed attribute that the user is attempting to register,
		then we must throw an exception if that Datum is already being used to
		represent an attribute or scope (i.e., its type is not UNKNOWN).*/
		Datum& appendedDatum = (*this)[name];
		if ((appendedDatum.Type() != DatumType::UNKNOWN) && (name != "this"s))
		{/*This will happen if a Datum with the name in the signature already
		 exists in this Scope.*/
			throw exception("Cannot register a prescribed attribute with\
				this name; it is taken!");
		}

		/*Finally, if the storage is meant to be external, we can call
		SetStorage and SetType to appropriately populate the Datum.*/
		DatumType type = signature.GetType();
		uint8_t* ptrToData = reinterpret_cast<uint8_t*>(this) + offset;
		if (external)
		{
			switch (type)
			{
			case DatumType::INTEGER:
				appendedDatum.SetStorage(reinterpret_cast<int32_t*>(ptrToData), size);
				break;
			case DatumType::FLOAT:
				appendedDatum.SetStorage(reinterpret_cast<float*>(ptrToData), size);
				break;
			case DatumType::VECTOR:
				appendedDatum.SetStorage(reinterpret_cast<vec4*>(ptrToData), size);
				break;
			case DatumType::MATRIX:
				appendedDatum.SetStorage(reinterpret_cast<mat4*>(ptrToData), size);
				break;
			case DatumType::STRING:
				appendedDatum.SetStorage(reinterpret_cast<string*>(ptrToData), size);
				break;
			case DatumType::POINTER:
				appendedDatum.SetStorage(reinterpret_cast<RTTI**>(ptrToData), size);
				break;
			}
		}
		/*On the other hand, if the storage is meant to be internal, we must
		call Set with a Scope& argument. Internal storage is only used for child
		Scopes.*/
		else
		{
			appendedDatum.SetType(type);
			appendedDatum.SetSize(size);
			for (uint32_t i = 0u; i < size; ++i)
			{
				appendedDatum.Set(*(reinterpret_cast<Scope**>(ptrToData) + i),
					i);
			}
			
		}

		/*We must also ensure that this prescribed attribute can be found in
		the static HashMap mapping this type's type ID to its Vector of
		signatures.*/
		if (prescribed)
		{
			Vector<Signature>& namesOfAttributes =
				TypeManager::mPrescribedMap[idOfSelf];
			bool signatureAlreadyPresent = false;
			for (auto sig : namesOfAttributes)
			{
				if (sig == signature)
				{
					signatureAlreadyPresent = true;
					break;
				}
			}
			if (!signatureAlreadyPresent)
			{
				namesOfAttributes.PushBack(signature);
			}
		}
	}

	void Attributed::RegisterBaseType(uint64_t idOfSelf, uint64_t baseId)
	{
		TypeManager::mParentTypeMap[idOfSelf] = baseId;
	}

	void Attributed::UpdateExternalStorage(const Attributed& other)
	{
		/*For each prescribed attribute, we need to find the Datum representing
		that attribute and, if it has external storage, call SetStorage using
		the offset from the signature.*/

		auto hashMapIt = TypeManager::mPrescribedMap.Find(other.TypeIdInstance());
		if (hashMapIt != TypeManager::mPrescribedMap.end())
		{
			const auto& signatures = hashMapIt->second;
			for (const Signature& sig : signatures)
			{
				Datum* datum = Find(sig.GetName());
				if (datum != nullptr)
				{
					if (!datum->IsInternal())
					{
						datum->SetStorage(reinterpret_cast<std::byte*>(this) + sig.GetOffset(), sig.GetSize());
					}
				}
			}
		}
	}

	Vector<pair<string, Datum>*> Attributed::Attributes(bool prescribed,
		bool auxiliary)
	{
		Vector<pair<string, Datum>*> outVector;
		for (auto& pair : *GetTable())
		{
			if (pair.second.Type() != DatumType::TABLE &&
				pair.second.Type() != DatumType::UNKNOWN)
			{
				if ((IsPrescribedAttribute(pair.first) && prescribed) ||
					(IsAuxiliaryAttribute(pair.first) && auxiliary))
				{
					outVector.PushBack(&pair);
				}
			}
		}
		return move(outVector);
	}
}