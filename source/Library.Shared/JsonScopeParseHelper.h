#pragma once

#include "IJsonParseHelper.h"

namespace LibraryShared
{
	/// <summary>
	/// This class is a concrete Parse Helper that handles parsing for JSON
	/// files that are recognized as containing a Scope at its base level using
	/// the grammar I have created for this assignment.
	/// </summary>
	class JsonScopeParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonScopeParseHelper, IJsonParseHelper)
	public:
		/// <summary>
		/// This method dynamically allocates a JsonScopeParseHelper and returns
		/// a pointer to it. The newly allocated JsonScopeParseHelper's pointer
		/// to a parse master will be nullptr. IMPORTANT: as the returned
		/// pointer is allocated on the heap, it is then the user's
		/// responsibility to ensure that the pointer is deleted at some point.
		/// </summary>
		/// <returns>A pointer to a newly heap-allocated JsonScopeParseHelper.
		/// </returns>
		virtual IJsonParseHelper* const Create() const override;

		/// <summary>
		/// This method handles all parsing of an input JSON key-value pair into
		/// a Scope in the default grammar. If key is "type", value is assumed
		/// to be a string that represents the type of the Datum that will be
		/// appended to the Scope: "integer", "float", "vector", "matrix",
		/// "string", "pointer", or "table". If key is "value", then value is
		/// the value that will be stored in a Datum and appended to a Scope,
		/// assuming it is of the type indicated by the value of key "type". If
		/// key is anything else, it is assumed to be a Datum name.
		/// </summary>
		/// <param name="key">- The key of the input JSON key-value pair.
		/// </param>
		/// <param name="value">- The value in the input JSON key-value pair.
		/// </param>
		/// <returns>True if this helper can handle the input key-value pair;
		/// false otherwise.</returns>
		virtual bool Handle(const std::string& key, const Json::Value& value)
			override;

		/// <summary>
		/// This method does everything necessary for handling the end of a JSON
		/// object.
		/// </summary>
		/// <returns>True if this method could handle the end of a JSON object;
		/// false otherwise.</returns>
		virtual bool HandleEndOfObject() override;

		/// <summary>
		/// This method handles the beginning of a JSON object, treating the
		/// input argument as the key in a JSON key-value pair.
		/// </summary>
		/// <param name="key">- The name of the key-value pair this method
		/// should handle.</param>
		/// <returns>True if this method could handle the input key; false
		/// otherwise.</returns>
		virtual bool HandleBeginning(const std::string& key) override;

	private:
		static bool TypeAndValueAreCompatible(DatumType type,
			const Json::Value& value);
	};
}