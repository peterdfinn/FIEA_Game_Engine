#pragma once
namespace UnitTest
{
	template <typename TKey>
	class BadHashFunctor
	{
	public:
		uint32_t operator()(const TKey& key) const;
	};
}
#include "BadHashFunctor.inl"