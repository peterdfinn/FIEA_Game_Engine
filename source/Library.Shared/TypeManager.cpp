#include "pch.h"

using namespace std;

namespace LibraryShared
{
	HashMap<uint64_t, Vector<Attributed::Signature>>
		TypeManager::mPrescribedMap;

	HashMap<uint64_t, uint64_t> TypeManager::mParentTypeMap;
}