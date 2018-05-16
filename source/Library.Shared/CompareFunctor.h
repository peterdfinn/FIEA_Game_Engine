#pragma once

#include <string>
namespace LibraryShared
{
	/// <summary>
	/// The class template CompareFunctor is to be used with the HashMap class
	/// template. Specializations of this template are provided for std::strings
	/// and char*s, but there is also a generic template.
	/// </summary>
	template <typename TKey>
	class CompareFunctor final
	{
	public:
		bool operator()(const TKey& key1, const TKey& key2) const;
	};

	template <>
	class CompareFunctor<char*> final
	{
	public:
		bool operator()(const char* key1, const char* key2) const;
	};

	template<>
	class CompareFunctor<std::string> final
	{
	public:
		bool operator()(const std::string& key1, const std::string& key2) const;
	};
}

#include "CompareFunctor.inl"