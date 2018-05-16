#pragma once
/// <summary>
/// Author:	Peter Finn
/// Date:	2/12/2018
/// This file contains the declaration of class 'Scope' and all its members.
/// </summary>

#include "Hashmap.h"
#include "Datum.h"
#include <string>

namespace LibraryShared {
	/// <summary>
	/// The class Scope 
	/// </summary>
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);
	public:
		/// <summary>
		/// This is the primary constructor for Scope. It takes one optional
		/// parameter: an unsigned integer that represents the initial capacity
		/// of the hash map this Scope represents.
		/// </summary>
		/// <param name="initialCapacity">- The initial capacity of the hash map
		/// stored in this Scope.</param>
		explicit Scope(uint32_t initialCapacity = 8u);

		/// <summary>
		/// This is the copy constructor for Scope. It has been set to the
		/// default.
		/// </summary>
		/// <param name="other">- A reference to the Scope that should be
		/// copied.</param>
		Scope(const Scope& other);

		/// <summary>
		/// This is the move constructor for Scope. It has been set to the
		/// default.
		/// </summary>
		/// <param name="other">- A reference to the Scope that should be moved.
		/// </param>
		Scope(Scope&& other);

		/// <summary>
		/// This is the copy assignment operator for Scope. It has been set to
		/// the default.
		/// </summary>
		/// <param name="other">- A reference to the Scope that should be
		/// copied.</param>
		/// <returns>A reference to this Scope.</returns>
		Scope& operator=(const Scope& other);

		/// <summary>
		/// This is the move assignment operator for Scope. It has been set to
		/// the default.
		/// </summary>
		/// <param name="other">- A reference to the Scope that should be moved.
		/// </param>
		/// <returns>A reference to this Scope.</returns>
		Scope& operator=(Scope&& other);

		/// <summary>
		/// This is the destructor for Scope. It deallocates all child Scopes,
		/// which were previously allocated on the Heap.
		/// </summary>
		virtual ~Scope() override;

		/// <summary>
		/// This method returns a pointer to the Scope that is the parent of
		/// this Scope.
		/// </summary>
		/// <returns>A pointer to this Scope's parent. If this Scope has no
		/// parent, nullptr is returned.</returns>
		Scope* GetParent() const;

		/// <summary>
		/// This overloading of the equality operator returns true if and only
		/// if its HashMaps are equal and if they have the same parent.
		/// </summary>
		/// <param name="other">- A reference to the Scope with which this one
		/// will be compared.</param>
		/// <returns>True if their parents are the same and if their hash maps
		/// are equal; false otherwise.</returns>
		bool operator==(const Scope& other) const;
		
		/// <summary>
		/// This overloading of the inequality operator simply returns the
		/// opposite of the equality operator.
		/// </summary>
		/// <param name="other">- A reference to the Scope with which this one
		/// will be compared.</param>
		/// <returns>False if their parents are the same and if their hash maps
		/// are equal; true otherwise.</returns>
		bool operator!=(const Scope& other) const;

		/// <summary>
		/// This method returns a pointer to the Datum with the input name, if
		/// it exists in this Scope. Otherwise, nullptr is returned.
		/// </summary>
		/// <param name="name">- The name of the Datum to find.</param>
		/// <returns>If a Datum exists in this Scope with name 'name', its
		/// address; otherwise, nullptr.</returns>
		Datum* Find(const std::string& name) const;

		/// <summary>
		/// This method searches for a Datum in this Scope, or any of its
		/// parents, with name 'name'. If no such Datum is found, nullptr is
		/// returned; otherwise, the address of this Datum is returned, and the
		/// value pointed to by outScopeAddr (if it's not nullptr) will be set
		/// to the address of the Scope that directly contains the desired
		/// Datum.
		/// </summary>
		/// <param name="name">- The name of the Datum for which to search.
		/// </param>
		/// <param name="outScopeAddr">- A pointer to a variable that will,
		/// after this method executes, contain the address of the Scope that
		/// directly contains the Datum that was found, if such a Datum was
		/// found.</param>
		/// <returns>A pointer to the Datum with name 'name', or nullptr if no
		/// such Datum was found.</returns>
		Datum* Search(const std::string& name, Scope** outScopeAddr = nullptr) const;

		/// <summary>
		/// This method ensures this Scope has a Datum with name 'name', either
		/// by inserting one into the table or by finding one that already
		/// exists.
		/// </summary>
		/// <param name="name">- The name of the Datum to insert (or find).
		/// </param>
		/// <returns>A reference to the Datum with name 'name'.</returns>
		Datum& Append(const std::string& name);

		/// <summary>
		/// This method appends a Scope with name 'name'. It adds it to the hash
		/// map, if it's not already there, and it pushes a pointer to its pair
		/// onto the back of the ordered vector. If you attempt to replace an
		/// existing Datum that has valid data with name 'name' with a new
		/// Scope, an exception will be thrown.
		/// </summary>
		/// <param name="name">- The name of the Scope to insert (or find).
		/// </param>
		/// <returns>A reference to the Scope that was inserted (or found).
		/// </returns>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		/// This method "adopts" an input scope with an input name, meaning this
		/// scope takes on 'toAdopt' as a child with name 'name'.
		/// </summary>
		/// <param name="name">- The name to assign to the newly adopted child
		/// scope.</param>
		/// <param name="toAdopt">- The scope to be adopted.</param>
		void Adopt(const std::string& name, Scope& toAdopt);

		/// <summary>
		/// This method is syntactic sugar for Append; see documentation for
		/// that method.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		Datum& operator[](const std::string& name);

		/// <summary>
		/// This method returns a reference to the Datum that was appended
		/// index-th in this Scope. If there is no such Datum, an exception is
		/// thrown.
		/// </summary>
		/// <param name="index">- The index of the Datum to which to get a
		/// reference.</param>
		/// <returns>A reference to the Datum at index 'index'.</returns>
		Datum& operator[](uint32_t index);

		/// <summary>
		/// This method finds the name of a Scope at a provided address and
		/// returns its name, if the address provided points at a valid Scope in
		/// this Scope. Otherwise, an empty string is returned.
		/// </summary>
		/// <param name="scopeAddress">- A pointer to a Scope whose name to
		/// find.</param>
		/// <returns>The name of the Scope at address 'scopeAddress', if it
		/// exists in this Scope; otherwise, an empty string.</returns>
		const std::string FindName(const Scope* scopeAddress) const;

		/// <summary>
		/// This method returns the number of string-Datum pairs this Scope
		/// contains.
		/// </summary>
		/// <returns>The number of string-Datum pairs this Scope contains.
		/// </returns>
		uint32_t Size() const;

		/// <summary>
		/// This method ensures that this Scope has its parent pointer set to
		/// nullptr and that the parent, if there is one, no longer has a
		/// reference to this Scope.
		/// </summary>
		void DetachFromParent();

	protected:
		/// <summary>
		/// Accessor for the symbol table, to be used by derived classes. There
		/// is also a const version of this method that returns a pointer to a
		/// const HashMap.
		/// </summary>
		/// <returns>A pointer to the symbol table in this Scope.</returns>
		HashMap<std::string, Datum>* GetTable();

		/// <summary>
		/// See non-const version of this method.
		/// </summary>
		/// <returns></returns>
		const HashMap<std::string, Datum>* GetTable() const;
	private:

		HashMap<std::string, Datum> mSymbolTable;

		Scope* mParent{ nullptr };

		Vector<std::pair<std::string, Datum>*> mAppendOrder;

		void Clear();

		void ReparentAllChildren();
	};
}