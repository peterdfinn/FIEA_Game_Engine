#pragma once
#include "HashMap.h"
#include "JsonParseMaster.h"

namespace LibraryShared
{
	/// <summary>
	/// This abstract class exists as a base from which all parse helpers in the
	/// parsing Chain of Responsibility should derive.
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)
	public:
		/// <summary>
		/// The constructor for IJsonParseHelper has been set to the default.
		/// </summary>
		IJsonParseHelper() = default;

		/// <summary>
		/// The copy constructor for IJsonParseHelper has been set to the
		/// default.
		/// </summary>
		/// <param name="other">- A reference to the IJsonParseHelper to copy.
		/// </param>
		IJsonParseHelper(const IJsonParseHelper& other) = default;

		/// <summary>
		/// The move constructor for IJsonParseHelper copies the pointer to the
		/// JsonParseMaster and then sets the pointer in the other
		/// IJsonParseHelper to nullptr.
		/// </summary>
		/// <param name="other">- A reference to the IJsonParseHelper to move.
		/// </param>
		IJsonParseHelper(IJsonParseHelper&& other);

		/// <summary>
		/// The copy constructor for IJsonParseHelper has been set to the
		/// default.
		/// </summary>
		/// <param name="other">- A reference to the IJsonParseHelper to copy.
		/// </param>
		/// <returns>A reference to this IJsonParseHelper.</returns>
		IJsonParseHelper& operator=(const IJsonParseHelper& other) = default;

		/// <summary>
		/// The move assignment operator for IJsonParseHelper copies the pointer
		/// to the JsonParseMaster and then sets the pointer in the other
		/// IJsonParseHelper to nullptr.
		/// </summary>
		/// <param name="other">- A reference to the IJsonParseHelper to move.
		/// </param>
		/// <returns>A reference to this IJsonParseHelper.</returns>
		IJsonParseHelper& operator=(IJsonParseHelper&& other);

		/// <summary>
		/// The destructor has been set to the default, as it doesn't have to do
		/// anything.
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// This method doesn't do anything. It exists solely to be overridden
		/// by user-defined subclasses, and it is intended to do whatever work
		/// needs to be done before a file is parsed.
		/// </summary>
		virtual void Initialize();
		
		/// <summary>
		/// This method, like Initialize(), it exists solely to be overridden in
		/// user-defined subclasses. In the broadest sense, it handles all
		/// parsing to be done with the key-value pair passed in as arguments.
		/// </summary>
		/// <param name="key">- The name of the JSON key-value pair to parse.
		/// </param>
		/// <param name="value">- A reference to a Json::Value object that
		/// symbolizes the value that is paired with "key".</param>
		/// <returns>True if this object could handle the parsing requested;
		/// false otherwise.</returns>
		virtual bool Handle(const std::string& key, const Json::Value& value) = 0;

		/// <summary>
		/// This is a pure virtual method that derived classes must override. It
		/// is meant to act as a handler for the end of a JSON object.
		/// </summary>
		/// <returns>True if this handler could handle the end of a JSON object
		/// when called; false otherwise.</returns>
		virtual bool HandleEndOfObject() = 0;

		/// <summary>
		/// This is a pure virtual method that derived classes must override. It
		/// is meant to act as the first step in handling a JSON object, and
		/// should probably be used to deal with the key in a JSON key-value
		/// pair.
		/// </summary>
		/// <param name="key">- The key of the JSON object to handle.</param>
		/// <returns>True if this handler could handle the input key; false
		/// otherwise.</returns>
		virtual bool HandleBeginning(const std::string& key) = 0;

		/// <summary>
		/// This method is used in place of a constructor. Constructors cannot
		/// be virtual, but this method can. It (generally) is intended to
		/// allocate a new object onto the heap and return a pointer to it.
		/// </summary>
		/// <returns></returns>
		virtual IJsonParseHelper* const Create() const = 0;

		/// <summary>
		/// This is an accessor for a pointer to the ParseMaster associated with
		/// this ParseHelper.
		/// </summary>
		/// <returns>- A pointer to the ParseMaster associated with this
		/// ParseHelper.</returns>
		JsonParseMaster* GetParseMaster() const;

		/// <summary>
		/// This is a mutator for a pointer to the JsonParseMaster associated
		/// with this IJsonParseHelper.
		/// </summary>
		/// <param name="master">- A pointer to the JsonParseMaster to which
		/// this IJsonParseHelper should pertain.</param>
		void SetParseMaster(JsonParseMaster* master);

	private:
		JsonParseMaster* mParseMaster{nullptr};
	};
}