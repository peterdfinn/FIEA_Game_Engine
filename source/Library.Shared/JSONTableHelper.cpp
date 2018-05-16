#include "pch.h"
#include "JSONTableHelper.h"
using namespace std;

namespace LibraryShared
{
	bool JSONTableHelper::Handle()
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
		look at the top of the stack of Json::Values and see if it's a table -
		or, if the stack is empty, we should use the JSONScopeSharedData's
		Json::Value member, which is presumed to represent the root of the JSON
		tree.*/
		const Json::Value* valueToParse;
		bool stackWasEmpty = sharedDataPtr->IsStackEmpty();
		if (!stackWasEmpty)
		{
			if (sharedDataPtr->GetTopOfStackType() == "decrement")
			{
				sharedDataPtr->PopStack();
				sharedDataPtr->DecrementDepth();
				sharedDataPtr->SetCurrentScope(*sharedDataPtr->GetCurrentScope().GetParent());
				return true;
			}
			if (sharedDataPtr->GetTopOfStackType() != "table")
			{
				return false;
			}
			valueToParse = &sharedDataPtr->GetTopOfStackValue();
		}
		else
		{
			valueToParse = &sharedDataPtr->GetJsonValue();
		}

		/*Let's check to make sure the associated Json::Value is an "object".*/
		if (!valueToParse->isObject())
		{
			throw exception("Invalid syntax when parsing a table!");
		}

		/*Now that we've done all the important checks, we can finally start
		doing the meat of the handling of this table.*/
		/*The first step is to increment the depth.*/
		sharedDataPtr->IncrementDepth();

		/*The second step is to pop this table off the stack, if it came from
		the stack (as opposed to being the root Json::Value.*/
		string thisScopeName = sharedDataPtr->GetTopOfStackName();
		if (!stackWasEmpty)
		{
			sharedDataPtr->PopStack();
		}

		/*The third step is to push onto the stack a notifier that the depth
		should be decremented.*/
		sharedDataPtr->PushStack("decrement", "decrement", Json::Value());

		/*The fourth step is to push every element in this table onto the
		stack.*/
		for (const string& memberName : valueToParse->getMemberNames())
		{
			const string& type = (*valueToParse)[memberName]["type"].asString();
			const Json::Value& value = (*valueToParse)[memberName]["value"];
			sharedDataPtr->PushStack(memberName, type, value);
		}

		/*The final step is to Append a new Scope, with the appropriate name, if
		this is not the root Scope, and set the "current Scope" in the
		SharedData to this new child Scope.*/
		if (!stackWasEmpty)
		{
			Scope& childScope = sharedDataPtr->GetCurrentScope().AppendScope(thisScopeName);
			sharedDataPtr->SetCurrentScope(childScope);
		}

		return true;
	}

	IJsonParseHelper* JSONTableHelper::Create() const
	{
		//TEMPORARY
		return nullptr;
	}
}