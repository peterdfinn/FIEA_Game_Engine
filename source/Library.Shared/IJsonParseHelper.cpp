#include "pch.h"
#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"

using namespace std;

namespace LibraryShared
{
	RTTI_DEFINITIONS(IJsonParseHelper)

	IJsonParseHelper::IJsonParseHelper(IJsonParseHelper&& other) :
		mParseMaster(other.mParseMaster)
	{
		other.mParseMaster = nullptr;
	}

	IJsonParseHelper& IJsonParseHelper::operator=(IJsonParseHelper&& other)
	{
		if (this != &other)
		{
			mParseMaster = other.mParseMaster;
			other.mParseMaster = nullptr;
		}
		return *this;
	}

	void IJsonParseHelper::Initialize() {}

	JsonParseMaster* IJsonParseHelper::GetParseMaster() const
	{
		return mParseMaster;
	}
	
	void IJsonParseHelper::SetParseMaster(JsonParseMaster* master)
	{
		mParseMaster = master;
	}
}