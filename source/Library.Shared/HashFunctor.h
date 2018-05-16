#pragma once

namespace LibraryShared
{
	/// <summary>
	/// This class template represents the hash function used by the HashMap
	/// class template. Template specializations are provided for std::strings
	/// and char*s, but there is also a generic version.
	/// </summary>
	template <typename TKey>
	class HashFunctor final
	{
	public:
		uint32_t operator()(const TKey& key) const;
	};

	template <>
	class HashFunctor<char*> final
	{
	public:
		uint32_t operator()(const char* key) const;
	};

	template <>
	class HashFunctor<std::string> final
	{
	public:
		uint32_t operator()(const std::string& key) const;
	};
}

#include "HashFunctor.inl"