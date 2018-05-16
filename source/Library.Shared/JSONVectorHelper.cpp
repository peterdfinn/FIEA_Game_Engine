#include "pch.h"
#include "JSONVectorHelper.h"

using namespace std;

namespace LibraryShared
{
	bool JSONVectorHelper::Handle()
	{
		/*First, we need to check to see whether the SharedData associated with
		the ParseMaster is a JSONScopeSharedData. If it's not, that means this
		helper can't handle it.*/
		JSONScopeSharedData* const sharedDataPtr =
			GetParseMaster()->GetSharedData()->As<JSONScopeSharedData>();
		if (sharedDataPtr == nullptr)
		{
			return false;
		}

		/*Now that we know the SharedData is a JSONScopeSharedData, we should
		look at the top of the stack of Json::Values and see if it's a vec4.*/
		const string& type = sharedDataPtr->GetTopOfStackType();
		if (type != "vector")
		{
			return false;
		}

		/*Now that we have the type, let's check to make sure the associated
		Json::Value is of type string.*/
		const Json::Value& value = sharedDataPtr->GetTopOfStackValue();
		if (!value.isString())
		{
			throw exception("Invalid syntax when parsing a vec4!");
		}

		/*Now we know that the Value to parse is a vec4, we can start handling
		it.*/
		Datum& newlyAppendedDatum =
			sharedDataPtr->GetCurrentScope().Append(sharedDataPtr->GetTopOfStackName());
		newlyAppendedDatum.SetType(DatumType::VECTOR);
		assert(newlyAppendedDatum.Size() > 0u);

		/*Finally, we return.*/
		return newlyAppendedDatum.SetFromString(sharedDataPtr->GetTopOfStackValue().asString());
	}

	IJsonParseHelper* JSONVectorHelper::Create() const
	{
		//TEMPORARY
		return nullptr;
	}
}