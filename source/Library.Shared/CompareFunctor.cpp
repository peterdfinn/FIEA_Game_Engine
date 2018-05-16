#include "pch.h"
#include "CompareFunctor.h"
#include <cstring>
#include <string>
namespace LibraryShared
{
	bool CompareFunctor<char*>::operator()(const char* key1, const char* key2)
		const
	{
		return (strcmp(key1, key2) == 0);
	}

	bool CompareFunctor<std::string>::operator()(const std::string& key1, const std::string& key2) const
	{
		return (key1.compare(key2) == 0);
	}
}