/// <summary>
/// Author:	Peter Finn
/// Date:	3-4-2018
/// This file contains the declaration for the class JsonScopeSharedData, which
/// inherits from JsonSharedData. An object of this class is intended to be used
/// in parsing a JSON file to a Scope object. It contains a boolean indicating
/// whether it has been initialized yet, a Scope object into which a JSON value
/// shall be parsed, and a Json::Value object that contains all the data parsed
/// from the text of the JSON file.
/// </summary>

#pragma once

#include "JsonParseMaster.h"
#include "Scope.h"
#include "json/json.h"

namespace LibraryShared {

	/// <summary>
	/// This class is a concrete subclass of JsonSharedData. It is designed to
	/// be used alongside JsonScopeParseHelper and JsonParseMaster to parse JSON
	/// files that recognizably contain Scopes and adhere to the grammar I am
	/// using for this assignment.
	/// </summary>
	class JsonScopeSharedData final : public JsonSharedData
	{
		RTTI_DECLARATIONS(JsonScopeSharedData, JsonSharedData)
	public:
		/// <summary>
		/// This method will be called by ParseMaster's Parse method.
		/// Alternatively, the user may call this method explicitly. This method
		/// will, if this object has never been initialized, parse the input
		/// stream into this object's Json::Value object.
		/// </summary>
		virtual void Initialize() override;

		/// <summary>
		/// This is a const accessor for the reference to the current Scope,
		/// which is the Scope contained in this object, or one of its
		/// descendants. This reference points to the Scope in the Scope tree
		/// that is currently being populated with Datums. There is also a
		/// non-const version of this method that returns a non-const reference.
		/// </summary>
		/// <returns>A const reference to the Scope that is currently being
		/// populated with Datums.</returns>
		const Scope* GetCurrentScope() const;

		/// <summary>
		/// See const version of this method.
		/// </summary>
		/// <returns></returns>
		Scope* GetCurrentScope();

		/// <summary>
		/// This method returns a const reference to the back of the Vector that
		/// represents the stack of Json::Values to be parsed.
		/// </summary>
		/// <returns>- A const reference to the top of the stack; this is the
		/// Json::Value that should be parsed next, and it will be removed if a
		/// helper can parse it.</returns>
		const std::string& GetTopOfStackName() const;

		/// <summary>
		/// This is an accessor for the DatumType stored in the top item of the
		/// stack.
		/// </summary>
		/// <returns>The type of the top item in the stack.</returns>
		DatumType GetTopOfStackType() const;

		/// <summary>
		/// Accessor for the Json::Value pointer currently stored in the top
		/// item of the stack.
		/// </summary>
		/// <returns>The pointer to a Json::Value in the top element of the
		/// stack. </returns>
		const Json::Value* GetTopOfStackValue() const;

		/// <summary>
		/// Sets the type in the top element of the stack.
		/// </summary>
		/// <param name="type">- The type to which to set to top element of the
		/// stack.</param>
		void SetTopOfStackType(DatumType type);

		/// <summary>
		/// Sets the name in the top element of the stack.
		/// </summary>
		/// <param name="name">- The name to which to set the top element of the
		/// stack.</param>
		void SetTopOfStackName(const std::string& name);

		/// <summary>
		/// Sets the pointer in the top element of the stack.
		/// </summary>
		/// <param name="value">- A pointer to the Json::Value object that
		/// should be associated with the top element of the stack.</param>
		void SetTopOfStackValue(const Json::Value* value);

		/// <summary>
		/// Is the stack empty?
		/// </summary>
		/// <returns>True if the stack has no elements; false otherwise.
		/// </returns>
		bool IsStackEmpty() const;

		/// <summary>
		/// Pops the topmost element off the stack. If the stack is empty, an
		/// exception is thrown.
		/// </summary>
		void PopStack();

		/// <summary>
		/// Pushes a new element onto the stack, with its name set to the empty
		/// string, its value set to UNKNOWN, and its pointer set to nullptr.
		/// </summary>
		void PushStack();

		/// <summary>
		/// This method sets the Scope to which names and values are currently
		/// being appended.
		/// </summary>
		/// <param name="newScope">- A reference to the Scope that should now
		/// accept new string-Datum pairs.</param>
		void SetCurrentScope(Scope& newScope);

		/// <summary>
		/// This method returns a reference to the ultimate Scope stored in the
		/// associated JsonScopeSharedData, the Scope that contains the entirety
		/// of a parsed JSON file. There is also a const version of this method
		/// that returns a const reference.
		/// </summary>
		/// <returns>A reference to the Scope that, once parsing is done,
		/// contains every key-value pair in the JSON file.</returns>
		Scope& GetScope();

		/// <summary>
		/// See non-const version of this method.
		/// </summary>
		/// <returns></returns>
		const Scope& GetScope() const;

		/// <summary>
		/// This method dynamically allocates a new JsonScopeSharedData and
		/// returns a pointer to it. This returned pointer IS NOW PRESUMED TO BE
		/// OWNED BY THE CALLER, and it is their responsibility to ensure it is
		/// deleted.
		/// </summary>
		/// <returns>A pointer to a newly dynamically allocated
		/// JsonScopeSharedData.</returns>
		virtual JsonSharedData* const Clone() const override;

	private:
		class NameTypeValue
		{
		public:
			std::string mName{ "" };
			DatumType mType{ DatumType::UNKNOWN };
			const Json::Value* mValue{ nullptr };
		};

		Scope mScope;
		Vector<NameTypeValue> mStack;
		Scope* mCurrentScope{ &mScope };
		bool mHasBeenInitialized{ false };
	};
}
