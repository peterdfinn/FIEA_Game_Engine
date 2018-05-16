#include "pch.h"
namespace UnitTest
{
	RTTI_DEFINITIONS(ConcreteDerivedA);
	int32_t ConcreteDerivedA::GetMember() const
	{
		return mMember;
	}
}