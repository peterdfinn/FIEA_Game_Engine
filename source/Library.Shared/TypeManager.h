#pragma once

#include "HashMap.h"

namespace LibraryShared
{
	class Attributed::Signature;

	/// <summary>
	/// This class manages all types for the Attributed class. It contains a
	/// mapping of type IDs to lists of signatures and a mapping of type IDs to
	/// their parents' type IDs.
	/// </summary>
	class TypeManager final
	{
		friend class Attributed;
	private:
		static HashMap<uint64_t, Vector<Attributed::Signature>> mPrescribedMap;
		static HashMap<uint64_t, uint64_t> mParentTypeMap;
	};
}