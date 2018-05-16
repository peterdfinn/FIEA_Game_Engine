#pragma once

#include <cstdint>

namespace UnitTest
{
	class AbstractBase : public LibraryShared::RTTI
	{
		RTTI_DECLARATIONS(AbstractBase, RTTI);
	public:
		virtual int32_t GetMember() const = 0;
	protected:
		int32_t mMember{ 0 };
	};
}