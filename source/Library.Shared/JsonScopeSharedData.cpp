/// <summary>
/// Author:	Peter Finn
/// Date:	3-4-2018
/// This file contains the method definitions for the class JSONScopeSharedData
/// (see JSONScopeSharedData.h for a description of that class).
/// </summary>

#include "pch.h"

using namespace std;

namespace LibraryShared
{
	RTTI_DEFINITIONS(JsonScopeSharedData);

	JsonSharedData* const JsonScopeSharedData::Clone() const
	{
		return new JsonScopeSharedData;
	}

	void JsonScopeSharedData::Initialize()
	{
		mStack.PushBack(NameTypeValue());
	}

	Scope* JsonScopeSharedData::GetCurrentScope()
	{
		return mCurrentScope;
	}

	const Scope* JsonScopeSharedData::GetCurrentScope() const
	{
		return mCurrentScope;
	}

	const string& JsonScopeSharedData::GetTopOfStackName() const
	{
		return mStack.Back().mName;
	}

	DatumType JsonScopeSharedData::GetTopOfStackType() const
	{
		return mStack.Back().mType;
	}

	const Json::Value* JsonScopeSharedData::GetTopOfStackValue() const
	{
		return mStack.Back().mValue;
	}

	bool JsonScopeSharedData::IsStackEmpty() const
	{
		return mStack.IsEmpty();
	}

	void JsonScopeSharedData::PopStack()
	{
		mStack.PopBack();
	}

	void JsonScopeSharedData::PushStack()
	{
		mStack.PushBack(NameTypeValue());
	}

	void JsonScopeSharedData::SetCurrentScope(Scope& newScope)
	{
		mCurrentScope = &newScope;
	}

	Scope& JsonScopeSharedData::GetScope()
	{
		return mScope;
	}

	const Scope& JsonScopeSharedData::GetScope() const
	{
		return mScope;
	}

	void JsonScopeSharedData::SetTopOfStackType(DatumType type)
	{
		mStack.Back().mType = type;
	}

	void JsonScopeSharedData::SetTopOfStackName(const string& name)
	{
		mStack.Back().mName = name;
	}

	void JsonScopeSharedData::SetTopOfStackValue(const Json::Value* value)
	{
		mStack.Back().mValue = value;
	}
}