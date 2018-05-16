#pragma once
#include "pch.h"
#include "ParseMaster.h"

namespace UnitTest
{
	/// <summary>
	/// This class is a subclass of SharedData, used only for testing my
	/// implementation of the Chain of Responsibility pattern.
	/// </summary>
	class SimpleSharedData : public LibraryShared::ParseMaster::SharedData
	{
	public:
		/// <summary>
		/// This method allocates a new SimpleSharedData object to the heap.
		/// This method exists essentially to act as a constructor, except that,
		/// since it's not technically a constructor, it can be made virtual and
		/// called polymorphically. The onus is on the user to delete the
		/// pointer returned by this method.
		/// </summary>
		/// <returns>A pointer to a newly heap-allocated SimpleSharedData
		/// object.</returns>
		virtual SharedData* Clone() const override;

		/// <summary>
		/// This method only exists to aid my unit tests. It returns true if and
		/// only if dummy and other.dummy are equal.
		/// </summary>
		/// <param name="other">- A const reference to a SharedData object to
		/// which this object should be compared.</param>
		/// <returns>- True if dummy and other.dummy are equal; false otherwise.
		/// </returns>
		virtual bool operator==(const SharedData& other) const override;

		/// <summary>
		/// This overloading of the equality operator is called by
		/// operator==(const SharedData&). It compares dummy and other.dummy.
		/// See documentation for operator==(const SharedData&).
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool operator==(const SimpleSharedData& other) const;

		/// <summary>
		/// An integer that represents data read from a JSON file.
		/// </summary>
		int32_t dummy{ 0 };
	};
}