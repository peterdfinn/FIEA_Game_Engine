#pragma once

#include "json/json.h"

namespace LibraryShared
{
	class IJsonParseHelper;
	class JsonSharedData;

	/// <summary>
	/// This class is the Parse Master used for parsing a JSON file. In order
	/// to parse a JSON file, instantiate this class, add appropriate helpers to
	/// the object, and then call Parse.
	/// </summary>
	class JsonParseMaster final
	{
	public:
		/// <summary>
		/// This constructor for ParseMaster takes one optional parameter: a
		/// pointer to the SharedData object which this ParseMaster should use
		/// for output of parsed data. Its default value is nullptr.
		/// </summary>
		explicit JsonParseMaster(JsonSharedData* sharedData = nullptr);

		/// <summary>
		/// This is the destructor for ParseMaster. If this ParseMaster is a
		/// clone, then it deletes all of its helpers.
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// The copy constructor of JsonParseMaster has been deleted, as one
		/// should never call it; one should instead call Clone.
		/// </summary>
		/// <param name=""></param>
		JsonParseMaster(const JsonParseMaster&) = delete;

		/// <summary>
		/// The copy assignment operator of JsonParseMaster has been deleted, as
		/// one should never call it; one should instead call Clone.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		JsonParseMaster& operator=(const JsonParseMaster&) = delete;

		/// <summary>
		/// This is the move constructor of JsonParseMaster. It steals another
		/// JsonParseMaster's data and nulls out its member variables.
		/// </summary>
		/// <param name="other">- The JsonParseMaster to be moved from.</param>
		JsonParseMaster(JsonParseMaster&& other);

		/// <summary>
		/// This is the move assignment operator for JsonParseMaster. It steals
		/// another JsonParseMaster's data and nulls out its member variables.
		/// </summary>
		/// <param name="other">- The JsonParseMaster to be moved from.</param>
		/// <returns>A reference to this JsonParseMaster.</returns>
		JsonParseMaster& operator=(JsonParseMaster&& other);

		/// <summary>
		/// This method produces a heap-allocated copy of this JsonParseMaster
		/// and returns a pointer to it. A cloned JsonParseMaster is ready to
		/// handle a new JSON file, and unlike the original ParseMaster that was
		/// directly instantiated by the user, a clone owns its helpers' memory.
		/// IMPORTANT: once Clone has been called, it is the responsibility of
		/// the user to ensure that the returned pointer is deleted at some
		/// point.
		/// </summary>
		/// <returns>A pointer to a newly heap-allocated JsonParseMaster that is
		/// identical to this one, except that it's ready to parse a new JSON
		/// file.</returns>
		JsonParseMaster* const Clone();

		/// <summary>
		/// This method adds a helper to this JsonParseMaster's list of parse
		/// helpers if this JsonParseMaster is not a clone.
		/// </summary>
		/// <param name="helper">- A reference to the helper to add to this
		/// JsonParseMaster's list of parse helpers.</param>
		/// <returns>True if the helper was successfully added to the list;
		/// false otherwise.</returns>
		bool AddHelper(IJsonParseHelper& helper);

		/// <summary>
		/// This method removes a parse helper from this ParseMaster's list of
		/// parse helpers.
		/// </summary>
		/// <param name="helper">- A reference to the helper to remove.</param>
		/// <returns>True if the helper was found and successfully removed from
		/// the ParseMaster's list of helpers; false otherwise.</returns>
		bool RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// This method iterates through all of this ParseMaster's helpers and
		/// calls all of their Handle methods until one returns true, indicating
		/// that that helper was successfully able to handle any parsing asked
		/// of it.
		/// </summary>
		/// <param name="inputStream">- The stream containing characters from
		/// the JSON file to parse.</param>
		void Parse(std::ifstream& inputStream);

		/// <summary>
		/// This method returns true if this ParseMaster came into existence as
		/// a result of Clone(), or false otherwise.
		/// </summary>
		/// <returns>True if this ParseMaster is a clone; false otherwise.
		/// </returns>
		bool IsClone() const;

		/// <summary>
		/// This is an accessor for this ParseMaster's pointer to its
		/// SharedData.
		/// </summary>
		/// <returns>A pointer to the SharedData used by this ParseMaster.
		/// </returns>
		JsonSharedData* GetSharedData() const;

		/// <summary>
		/// This method returns a const reference to this ParseMaster's list of
		/// helpers.
		/// </summary>
		/// <returns>A const reference to this ParseMaster's list of helpers.
		/// </returns>
		const Vector<IJsonParseHelper*>& GetParseHelpers() const;

		/// <summary>
		/// This method should be used to initialize a ParseMaster. It will
		/// initialize the SharedData object and all of its helpers.
		/// </summary>
		void Initialize() const;

	private:
		// a list of pointers to this master's helpers
		Vector<IJsonParseHelper*> mParseHelpers;

		//a pointer to this master's shared data
		JsonSharedData* mSharedData;

		//Is this master a clone of another master?
		bool mIsClone{ false };

		//helper method for clones to free owned memory
		void FreeOwnedMemory();

		//recursive Parse method
		IJsonParseHelper* RecursiveParse(const std::string& key, const Json::Value& jsonValue);
	};
}