#include "pch.h"

#include "Scope.h"

using namespace std;

namespace LibraryShared
{
	RTTI_DEFINITIONS(Scope);
	Scope::Scope(uint32_t initialCapacity) : mSymbolTable(initialCapacity)
	{}

	Scope::~Scope()
	{
		Clear();
	}

	Scope::Scope(const Scope& other)
	{
		operator=(other);
	}

	Scope::Scope(Scope&& other)
	{
		operator=(move(other));
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (&other != this)
		{
			Clear();
			DetachFromParent();

			// deep copying all nested Scopes, as well as copying Datums.
			for (auto& pair : other.mSymbolTable)
			{
				Datum& datumInOther = pair.second;
				Datum& datumInThis = Append(pair.first);
				if (datumInOther.Type() == DatumType::TABLE)
				{
					for (uint32_t i = 0u; i < datumInOther.Size(); ++i)
					{
						Scope* s = new Scope(datumInOther.GetScope(i));
						s->mParent = this;
						datumInThis.PushBack(*s);
					}
				}
				else
				{
					datumInThis = datumInOther;
				}
			}
		}

		return *this;
	}

	Scope& Scope::operator=(Scope&& other)
	{
		if (&other != this)
		{
			/*Before copying other's mParent pointer, we must ensure our
			current parent no longer contains a reference to us.*/
			if (mParent != nullptr)
			{
				if (Is(Entity::TypeIdClass()))
				{
					Datum* entitiesDatum = mParent->Find("Entities"s);
					assert(entitiesDatum != nullptr);
					for (uint32_t i = 0u; i < entitiesDatum->Size(); ++i)
					{
						if (&entitiesDatum->GetScope(i) == this)
						{
							entitiesDatum->RemoveAtIndex(i);
							break;
						}
					}
				}
				else if (Is(Sector::TypeIdClass()))
				{
					Datum* sectorsDatum = mParent->Find("Sectors"s);
					assert(sectorsDatum != nullptr);
					for (uint32_t i = 0u; i < sectorsDatum->Size(); ++i)
					{
						if (&sectorsDatum->GetScope(i) == this)
						{
							sectorsDatum->RemoveAtIndex(i);
							break;
						}
					}
				}
				else
				{
					DetachFromParent();
				}
			}

			/*Now, before moving a Scope into this Scope, we must ensure that
			any children of this Scope are dealt with.*/
			for (auto& pair : mSymbolTable)
			{
				Datum& datumInThisPair = pair.second;
				if ((datumInThisPair.Type() == DatumType::TABLE) && datumInThisPair.IsInternal())
				{
					uint32_t datumSize = datumInThisPair.Size();
					for (uint32_t i = 0u; i < datumSize; ++i)
					{
						delete &datumInThisPair.GetScope(i);
					}
				}
			}

			/*standard copying/moving of members*/
			mParent = other.mParent;
			mSymbolTable = move(other.mSymbolTable);
			mAppendOrder = move(other.mAppendOrder);

			/*We must now ensure the parent (if there is one) points to this
			object instead of 'other'.*/
			if (mParent != nullptr)
			{
				for (auto& pairInParent : mParent->mSymbolTable)
				{
					Datum& datumInParent = pairInParent.second;
					if (datumInParent.Type() != DatumType::TABLE)
					{
						continue;
					}
					for (uint32_t i = 0u; i < datumInParent.Size(); ++i)
					{
						Scope* ptrToOther = &(datumInParent.GetScope(i));
						if (ptrToOther == &other)
						{
							datumInParent.Set(*this, i);
						}
					}
				}
			}

			ReparentAllChildren();
		}

		return *this;
	}

	Scope* Scope::GetParent() const
	{
		return mParent;
	}

	bool Scope::operator==(const Scope& other) const
	{
		return (mSymbolTable == other.mSymbolTable);
 	}

	bool Scope::operator!=(const Scope& other) const
	{
		return !operator==(other);
	}

	Datum* Scope::Find(const string& name) const
	{
		auto iterator = mSymbolTable.Find(name);
		if (iterator == mSymbolTable.end())
		{
			return nullptr;
		}
		
		return &iterator->second;
	}

	Datum* Scope::Search(const string& name, Scope** outScopeAddr) const
	{
		Datum* findResult = Find(name);
		if (findResult != nullptr)
		{
			if (outScopeAddr != nullptr)
			{
				*outScopeAddr = const_cast<Scope*>(this);
			}
			return findResult;
		}

		if (mParent == nullptr)
		{
			return nullptr;
		}

		return mParent->Search(name, outScopeAddr);
	}

	Datum& Scope::Append(const string& name)
	{
		if (name == "")
		{
			throw exception("Empty string is not a valid name!");
		}
		auto hashMapInsertResult = mSymbolTable.Insert(make_pair(name, Datum()));
		auto hashMapInsertResultPointsTo = *hashMapInsertResult;
		auto endIter = mAppendOrder.end();
		auto iter = mAppendOrder.Find(&(*hashMapInsertResult));
		bool areEqual = (iter == endIter);
		areEqual;
		if (mAppendOrder.Find(&(*hashMapInsertResult)) == mAppendOrder.end())
		{
			mAppendOrder.PushBack(&(*hashMapInsertResult));
		}
		return hashMapInsertResult->second;
	}

	Scope& Scope::AppendScope(const string& name)
	{
		Datum& appendReturn = Append(name);
		if (appendReturn.Type() != DatumType::UNKNOWN)
		{
			throw exception("Cannot append a scope with this name: it is\
				already taken!");
		}

		Scope* newScope = new Scope();
		newScope->mParent = this;
		appendReturn.PushFront(*newScope);
		return appendReturn.GetScope();
	}
	
	void Scope::Adopt(const string& name, Scope& toAdopt)
	{
		if (&toAdopt != this)
		{
			toAdopt.DetachFromParent();
			toAdopt.mParent = this;
			if (toAdopt.Is(Action::TypeIdClass()))
			{
				Datum* const actionsDatum = Find("Actions"s);
				assert(actionsDatum != nullptr);
				actionsDatum->PushBack(toAdopt);
			}
			else if (toAdopt.Is(Entity::TypeIdClass()))
			{
				assert(Is(Sector::TypeIdClass()));
				Datum* const entitiesDatum = Find("Entities"s);
				assert(entitiesDatum != nullptr);
				entitiesDatum->PushBack(toAdopt);
			}
			else if (toAdopt.Is(Sector::TypeIdClass()))
			{
				assert(Is(World::TypeIdClass()));
				Datum* const sectorsDatum = Find("Sectors"s);
				assert(sectorsDatum != nullptr);
				sectorsDatum->PushBack(toAdopt);
			}
			else
			{
				Datum& appendReturn = Append(name);
				if (appendReturn.Type() != DatumType::UNKNOWN)
				{
					throw exception("Cannot append a scope with this name: it is\
					already taken!");
				}
				appendReturn.PushBack(toAdopt);
			}
		}
	}

	Datum& Scope::operator[](const string& name)
	{
		return Append(name);
	}

	Datum& Scope::operator[](uint32_t index)
	{
		return mAppendOrder.At(index)->second;
	}

	const string Scope::FindName(const Scope* scopeAddress) const
	{
		for (pair<string, Datum>* pair : mAppendOrder)
		{
			const Datum& datumInThisPair = pair->second;
			if (datumInThisPair.Type() == DatumType::TABLE)
			{
				for (uint32_t i = 0u; i < datumInThisPair.Size(); ++i)
				{
					if (&datumInThisPair.GetScope(i) == scopeAddress)
					{
						return pair->first;
					}
				}
			}
		}

		return "";
	}

	uint32_t Scope::Size() const
	{
		uint32_t numberOfDatums = 0u;
		for (auto pair : mSymbolTable)
		{
			numberOfDatums++;
		}
		return numberOfDatums;
	}

	void Scope::Clear()
	{
		for (uint32_t i = mAppendOrder.Size(); i > 0u; --i)
		{
			auto* thisPair = mAppendOrder.At(i - 1u);
			Datum& datumInThisPair = thisPair->second;
				if (datumInThisPair.Type() == DatumType::TABLE)
				{
					for (uint32_t j = datumInThisPair.Size(); j > 0u; --j)
					{
						Scope* scopeInThisDatum = &datumInThisPair.GetScope(j - 1u);
						scopeInThisDatum->Clear();
						scopeInThisDatum->DetachFromParent();
						delete scopeInThisDatum;
					}
					
					
				}
				else
				{
					datumInThisPair.Clear();
				}
		}
		mSymbolTable.Clear();
		mAppendOrder.Clear();
	}

	void Scope::DetachFromParent()
	{
		if (mParent != nullptr)
		{
			if (Is(Action::TypeIdClass()))
			{
				Datum* actionsDatum = mParent->Find("Actions"s);
				assert(actionsDatum != nullptr);
				actionsDatum->Remove(*this);
			}
			else if (Is(Entity::TypeIdClass()))
			{
				Datum* entitiesDatum = mParent->Find("Entities"s);
				assert(entitiesDatum != nullptr);
				entitiesDatum->Remove(*this);
			}
			else if (Is(Sector::TypeIdClass()))
			{
				Datum* sectorsDatum = mParent->Find("Sectors"s);
				assert(sectorsDatum != nullptr);
				sectorsDatum->Remove(*this);
			}
			else
			{
				string name = mParent->FindName(this);
				if (!name.empty())
				{
					auto iteratorForPair = mParent->mSymbolTable.Find(name);
					pair<string, Datum>& pairContainingDatumWithScope = *iteratorForPair;
					mParent->mAppendOrder.Remove(&pairContainingDatumWithScope);
					mParent->mSymbolTable.Remove(pairContainingDatumWithScope.first);
				}
			}
			mParent = nullptr;
		}
	}

	void Scope::ReparentAllChildren()
	{
		for (auto& pair : mSymbolTable)
		{
			Datum& datum = pair.second;
			if (datum.Type() == DatumType::TABLE)
			{
				for (uint32_t i = 0u; i < datum.Size(); ++i)
				{
					datum.GetScope(i).mParent = this;
				}
			}
		}
	}

	HashMap<string, Datum>* Scope::GetTable()
	{
		return &mSymbolTable;
	}

	const HashMap<string, Datum>* Scope::GetTable() const
	{
		return &mSymbolTable;
	}
}