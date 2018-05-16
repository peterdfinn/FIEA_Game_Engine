#pragma once
/// <summary>
/// Author:	Peter Finn
/// Date:	2/24/2018
/// This file contains the declaration of class 'Attributed', which will
/// represent an object/class created by a user of our game engine.
/// </summary>
#include "Scope.h"

namespace LibraryShared
{
	/// <summary>
	/// The Attributed class represents an object that can have attributes,
	/// whether they be prescribed or auxiliary. For example, an enemy might be
	/// an object of the Attributed class, and its attributes could be a string
	/// for a name and an integer or float for its health.
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:

		/// <summary>
		/// This nested class represents a signature of a prescribed attribute.
		/// A signature is a representation of a prescribed attribute's place in
		/// an Attributed object, including its name, its offset in an
		/// Attributed object, its size, and its type.
		/// </summary>
		class Signature
		{
		public:
			/// <summary>
			/// This constructor for Signature initializes the member variables
			/// of Signature.
			/// </summary>
			/// <param name="name">- The name of the attribute with this
			/// signature.</param>
			/// <param name="type">- The type of the attribute with this
			/// signature.</param>
			/// <param name="size">- The size of the attribute with this
			/// signature, the number of entries of type 'type'. This will be 1
			/// in the case the attribute is not an array, or greater than 1 if
			/// the attribute is an array.</param>
			/// <param name="offset">- The offset in bytes of the attribute into
			/// the object.</param>
			Signature(const std::string& name, DatumType type, uint32_t size, uint32_t offset);

			/// <summary>
			/// Accessor for the name of the attribute with this signature.
			/// </summary>
			/// <returns>The name of the attribute with this signature.
			/// </returns>
			std::string GetName() const;

			/// <summary>
			/// Accessor for the byte offset of the attribute with this
			/// signature.
			/// </summary>
			/// <returns>The offset in bytes of the attribute into the object.
			/// </returns>
			uint32_t GetOffset() const;

			/// <summary>
			/// Accessor for the size of the array that is the attribute with
			/// this signature.
			/// </summary>
			/// <returns>The size of the array that is the attribute with this
			/// signature.</returns>
			uint32_t GetSize() const;

			/// <summary>
			/// Accessor for the type of the attribute with this signature.
			/// </summary>
			/// <returns>The type of the attribute with this signature.
			/// </returns>
			DatumType GetType() const;

			/// <summary>
			/// This method compares two Signatures by their names.
			/// </summary>
			/// <param name="other">- The Signature to compare to this one.
			/// </param>
			/// <returns>True if this Signature and 'other' have the same name;
			/// false otherwise.</returns>
			bool operator==(const Signature& other) const;

		private:
			std::string mName;
			DatumType mType;
			uint32_t mSize;
			uint32_t mOffset;
		};

	public:
		/// <summary>
		/// This is the standard constructor for an 'Attributed' object. It
		/// initializes its private Vector and HashMap to have a capacity of
		/// 'initialCapacity', and it inserts an RTTI* representing this object
		/// into the table. It also inserts an integer-Vector pair into the
		/// static HashMap that maps type IDs to lists of prescribed attributes
		/// and makes sure "this" is included in that list.
		/// </summary>
		/// <param name="initialCapacity">- The initial capacity of the HashMap
		/// contained in this object (initialized in Scope constructor).</param>
		explicit Attributed(uint32_t initialCapacity = 8u);

		/// <summary>
		/// This is the copy constructor for Attributed. It calls the copy
		/// constructor of Scope, which deep copies the HashMap, and then it
		/// assigns the pointers in those Datums with external storage to point
		/// into this object instead of the other object.
		/// </summary>
		/// <param name="other">- A reference to the Attributed object to copy.
		/// </param>
		Attributed(const Attributed& other);

		/// <summary>
		/// This is the move constructor for Attributed. It simply calls the
		/// move constructor of Scope.
		/// </summary>
		/// <param name="other">- A reference to the Attributed object to move.
		/// </param>
		Attributed(Attributed&& other);

		/// <summary>
		/// This is the destructor for Attributed. It has been set to the
		/// default.
		/// </summary>
		virtual ~Attributed() override = default;

		/// <summary>
		/// This is the copy assignment operator for Attributed. It simply calls
		/// the copy assignment operator for Scope.
		/// </summary>
		/// <param name="other">- A reference to the Attributed object to copy.
		/// </param>
		/// <returns>A reference to this object.</returns>
		Attributed& operator=(const Attributed& other);

		/// <summary>
		/// This is the move assignment operator for Attributed. It simply calls
		/// the move assignment operator for Scope.
		/// </summary>
		/// <param name="other">- A reference to the Attributed object to move.
		/// </param>
		/// <returns>A reference to this object.</returns>
		Attributed& operator=(Attributed&& other);

		/// <summary>
		/// This method checks to see if this object has an attribute of name
		/// 'name'.
		/// </summary>
		/// <param name="name">- The name of the attribute for which to search.
		/// </param>
		/// <returns>True if this object contains an attribute with name 'name',
		/// whether it's a prescribed attribute or an auxiliary attribute; false
		/// otherwise.</returns>
		bool IsAttribute(const std::string& name) const;

		/// <summary>
		/// This method determines whether an input name is used by this object
		/// as a prescribed attribute.
		/// </summary>
		/// <param name="name">- The name of the attribute to search for.
		/// </param>
		/// <returns>True if this object contains a prescribed attribute with
		/// name 'name'; false otherwise.</returns>
		bool IsPrescribedAttribute(const std::string& name) const;

		/// <summary>
		/// This method determines whether an input name belongs to one of this
		/// object's auxiliary attributes.
		/// </summary>
		/// <param name="name">- The name of the attribute to search for.
		/// </param>
		/// <returns>True if this object contains an auxiliary attribute with
		/// name 'name'; false otherwise.</returns>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// This method adds a new auxiliary attribute with the provided name to
		/// this object, if it doesn't already exist as an auxiliary attribute.
		/// In either case, this method returns a reference to an auxiliary
		/// Datum with this name. If the name is already being used for a
		/// prescribed attribute, however, an exception is thrown.
		/// </summary>
		/// <param name="name">- The name of the Datum that is now an auxiliary
		/// attribute with name 'name'.</param>
		/// <returns>A reference to the Datum that represents the auxiliary
		/// attribute with name 'name'.</returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

	private:

		Vector<std::pair<std::string, Datum>*> Attributes(bool prescribed,
			bool auxiliary);

		void UpdateExternalStorage(const Attributed& other);

	protected:
		/// <summary>
		/// This method should be called in every constructor for a class that
		/// inherits from Attributed, for each member of that class, in order to
		/// register that member as a prescribed attribute.
		/// </summary>
		/// <param name="signature">- The signature of the attribute that is
		/// being registered.</param>
		/// <param name="idOfSelf">- The type ID of the class that contains the
		/// attribute being registered.</param>
		/// <param name="prescribed">- Whether the attribute being registered is
		/// a prescribed or auxiliary attribute.</param>
		/// <param name="external">- Whether the attribute being registered will
		/// be stored in a Datum as external or internal storage.</param>
		void RegisterAttribute(const Signature& signature, uint64_t idOfSelf,
			bool prescribed = true, bool external = true);

		/// <summary>
		/// This method should be called to pair a type to the type from which
		/// it inherits.
		/// </summary>
		/// <param name="idOfSelf">- The type ID of the child type.</param>
		/// <param name="baseId">- The type ID of the parent type.</param>
		static void RegisterBaseType(uint64_t idOfSelf, uint64_t baseId);
	};
}