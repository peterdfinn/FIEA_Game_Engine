#include "pch.h"
namespace UnitTest
{
	RTTI_DEFINITIONS(ConcreteDerivedB);
	int32_t ConcreteDerivedB::GetMember() const
	{
		return mMember;
	}
}