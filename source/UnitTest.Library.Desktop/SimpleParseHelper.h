#pragma once
#include "pch.h"
#include "Datum.h"

namespace UnitTest
{
	/// <summary>
	/// This class is a trivial parse helper for unit testing my implementation
	/// of the Chain of Responsibility pattern for parsing JSON.
	/// </summary>
	class SimpleParseHelper : public LibraryShared::IJsonParseHelper
	{
	public:
		/// <summary>
		/// This is the destructor for SimpleParseHelper. It has been set to the
		/// default, as there aren't even any data members in this class.
		/// </summary>
		virtual ~SimpleParseHelper() override = default;

		/// <summary>
		/// This method exists only as a virtual method that overrides the
		/// method of the same name from IParseHelper. It allocates a
		/// SimpleParseHelper object onto the heap and returns a pointer to the
		/// newly allocated object. The onus is then on the user to ensure this
		/// pointer is deleted, ensuring there is no memory leak.
		/// </summary>
		/// <returns>A pointer to a newly heap-allocated SimpleParseHelper
		/// object.</returns>
		virtual LibraryShared::IParseHelper* Create() override;

		/// <summary>
		/// This overloading of the equality operator doesn't do much here: it
		/// simply returns true no matter what. In a real use of the Parse chain
		/// of responsibility, this method could be overridden to compare two
		/// ParseHelpers. In this simple scenario, this method only exists to
		/// aid unit testing.
		/// </summary>
		/// <param name="other">- A reference to the IParseHelper to compare
		/// with this object.</param>
		/// <returns>True.</returns>
		//virtual bool operator==(const IParseHelper& other) const override;

		/// <summary>
		/// This method only exists to be called by operator==(const IParseHelper&).
		/// See documentation for that method.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool operator==(const SimpleParseHelper& other) const;

		/// <summary>
		/// This method handles everything related to parsing my simple JSON
		/// file. It calls itself recursively.
		/// </summary>
		/// <param name="bufferIn">- A reference to the stringstream that
		/// contains the characters from the JSON file.</param>
		/// <returns>- True if this method is able to handle the first character
		/// in the stringstream and parse it meaningfully; false otherwise.</returns>
		virtual bool Handle(std::stringstream& bufferIn) override;
	};
}