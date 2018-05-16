#include "pch.h"
using namespace std;
namespace LibraryShared
{
	RTTI_DEFINITIONS(JsonSharedData)

	JsonSharedData::JsonSharedData(JsonParseMaster* parseMaster) :
		mParseMaster(parseMaster)
	{}

	JsonSharedData::JsonSharedData(JsonSharedData&& other) :
		mDepth(other.mDepth), mParseMaster(other.mParseMaster)
	{
		other.mDepth = 0u;
		other.mParseMaster = nullptr;
	}

	JsonSharedData& JsonSharedData::operator=(JsonSharedData&& other)
	{
		if (&other != this)
		{
			mDepth = other.mDepth;
			mParseMaster = other.mParseMaster;
			other.mDepth = 0u;
			other.mParseMaster = nullptr;
		}
		return *this;
	}

	JsonSharedData* const JsonSharedData::Clone() const
	{
		return new JsonSharedData(mParseMaster);
	}

	void JsonSharedData::SetParseMaster(JsonParseMaster* parseMaster)
	{
		mParseMaster = parseMaster;
	}

	JsonParseMaster* JsonSharedData::GetParseMaster() const
	{
		return mParseMaster;
	}

	uint32_t JsonSharedData::IncrementDepth()
	{
		return ++mDepth;
	}

	uint32_t JsonSharedData::DecrementDepth()
	{
		return --mDepth;
	}

	uint32_t JsonSharedData::Depth() const
	{
		return mDepth;
	}

	void JsonSharedData::Initialize()
	{
		mDepth = 0u;
	}
}