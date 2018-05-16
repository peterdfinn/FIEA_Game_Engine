#include "pch.h"

#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

using namespace std;

namespace LibraryShared
{
	JsonParseMaster::JsonParseMaster(JsonSharedData* sharedData) :
		mSharedData(sharedData)
	{
		if (sharedData != nullptr)
		{
			sharedData->SetParseMaster(this);
		}
	}

	JsonParseMaster::~JsonParseMaster()
	{
		FreeOwnedMemory();
	}

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& other)
	{
		operator=(move(other));
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& other)
	{
		if (&other != this)
		{
			FreeOwnedMemory();

			mParseHelpers = move(other.mParseHelpers);
			mSharedData = other.mSharedData;
			mIsClone = other.mIsClone;

			other.mParseHelpers.Clear();
			other.mSharedData = nullptr;
			other.mIsClone = false;
		}

		return *this;
	}

	JsonParseMaster* const JsonParseMaster::Clone()
	{
		/*We need to allocate a new ParseMaster on the heap, then we make sure
		all of this ParseMaster's helpers have been duplicated in the new
		ParseMaster, we need to clone this ParseMaster's SharedData and store it
		in the new ParseMaster, and then we set the new ParseMaster's mIsClone
		to true and return a pointer to it.*/
		JsonParseMaster* newParseMaster;
		if (mSharedData != nullptr)
		{
			newParseMaster = new JsonParseMaster(mSharedData->Clone());
			newParseMaster->GetSharedData()->SetParseMaster(this);
		}
		else
		{
			newParseMaster = new JsonParseMaster;
		}

		for (IJsonParseHelper* helper : mParseHelpers)
		{
			newParseMaster->AddHelper(*helper->Create());
		}
		for (IJsonParseHelper* helper : newParseMaster->GetParseHelpers())
		{
			helper->SetParseMaster(this);
		}

		newParseMaster->mIsClone = true;
		return newParseMaster;
	}

	const Vector<IJsonParseHelper*>& JsonParseMaster::GetParseHelpers() const
	{
		return mParseHelpers;
	}

	JsonSharedData* JsonParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	bool JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		if (mIsClone)
		{
			return false;
		}

		mParseHelpers.PushBack(&helper);
		helper.SetParseMaster(this);
		return true;
	}

	bool JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
	{
		if (mIsClone)
		{
			return false;
		}

		helper.SetParseMaster(nullptr);
		return mParseHelpers.Remove(&helper);
	}

	void JsonParseMaster::Parse(ifstream& inputStream)
	{
		Initialize();
		Json::Value jsonValue;
		inputStream >> jsonValue;
		RecursiveParse("", jsonValue);
	}

	bool JsonParseMaster::IsClone() const
	{
		return mIsClone;
	}

	void JsonParseMaster::Initialize() const
	{
		if (mSharedData != nullptr)
		{
			mSharedData->Initialize();
		}
		for (auto helper : mParseHelpers)
		{
			helper->Initialize();
		}
	}

	void JsonParseMaster::FreeOwnedMemory()
	{
		if (mIsClone)
		{
			for (IJsonParseHelper* helper : mParseHelpers)
			{
				delete helper;
			}
			if (mSharedData != nullptr)
			{
				delete mSharedData;
			}
		}
	}

	IJsonParseHelper* JsonParseMaster::RecursiveParse(const string& key, const Json::Value& jsonValue)
	{
		/*If the jsonValue passed in is an Object, then that means we have to
		recursively call RecursiveParse for each of its members. If not, then we
		must pass the input key and value to each helper until one of them
		indicates that it can handle them.*/
		if (jsonValue.isObject())
		{
			mSharedData->IncrementDepth();
			IJsonParseHelper* correctHelper = nullptr;
			for (IJsonParseHelper* helper : mParseHelpers)
			{
				if (helper->HandleBeginning(key))
				{
					correctHelper = helper;
					break;
				}
			}
			for (const string& memberName : jsonValue.getMemberNames())
			{
				correctHelper = RecursiveParse(memberName, jsonValue[memberName]);
			}
			if (correctHelper != nullptr)
			{
				correctHelper->HandleEndOfObject();
				mSharedData->DecrementDepth();
			}
		}
		else
		{
			for (IJsonParseHelper* helper : mParseHelpers)
			{
				if (helper->Handle(key, jsonValue))
				{
					return helper;
				}
			}
		}

		return nullptr;
	}
}