#include "pch.h"

using namespace std;

namespace LibraryShared
{
	ActionListForEach::ActionListForEach() : ActionList()
	{
		RegisterMyAttributes();
	}

	ActionListForEach::ActionListForEach(Datum& contents, const string& name) :
		ActionList(name)
	{
		mContents = &contents;
		RegisterMyAttributes();
	}

	void ActionListForEach::RegisterMyAttributes()
	{
		const uint64_t actionListForEachId = ActionListForEach::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			actionListForEachId);
		RegisterAttribute(Signature("mContents"s, DatumType::POINTER, 1u,
			offsetof(ActionListForEach, mContents)), actionListForEachId);
		RegisterAttribute(Signature("mIndex"s, DatumType::INTEGER, 1u,
			offsetof(ActionListForEach, mIndex)), actionListForEachId);

		RegisterBaseType(actionListForEachId, ActionList::TypeIdClass());
	}
}