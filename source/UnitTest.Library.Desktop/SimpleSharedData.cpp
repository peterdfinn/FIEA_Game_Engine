#include "pch.h"
#include "SimpleSharedData.h"

using namespace LibraryShared;

namespace UnitTest
{
	bool SimpleSharedData::operator==(const ParseMaster::SharedData& other)
		const
	{
		return operator==(dynamic_cast<const SimpleSharedData&>(other));
	}

	bool SimpleSharedData::operator==(const SimpleSharedData& other) const
	{
		return dummy == other.dummy;
	}

	ParseMaster::SharedData* SimpleSharedData::Clone() const
	{
		return new SimpleSharedData;
	}
}