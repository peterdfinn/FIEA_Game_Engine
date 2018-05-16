#include "pch.h"

#include "HashFunctor.h"

namespace LibraryShared
{
	uint32_t HashFunctor<char*>::operator()(const char* key) const
	{
		uint32_t sumOfChars = 0u;
		for (uint32_t i = 0u; key[i] != '\0'; ++i)
		{
			sumOfChars += static_cast<uint32_t>(key[i]);
		}
		return sumOfChars;
	}

	uint32_t HashFunctor<std::string>::operator()(const std::string& key) const
	{
		uint32_t sumOfChars = 0u;
		for (uint32_t i = 0u; i < static_cast<uint32_t>(key.size()); ++i)
		{
			sumOfChars += static_cast<uint32_t>(key[i]);
		}
		return sumOfChars;
	}
}