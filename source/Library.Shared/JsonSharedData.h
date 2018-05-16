#pragma once

#include "RTTI.h"
#include "JsonParseMaster.h"

namespace LibraryShared
{
	/// <summary>
	/// This is an abstract class from which other classes should inherit. A
	/// shared data object is an object used as storage for and shared between
	/// parse helpers when parsing a grammar-specific JSON file.
	/// </summary>
	class JsonSharedData : public RTTI
	{
		RTTI_DECLARATIONS(JsonSharedData, RTTI)
	public:
		/// <summary>
		/// The only constructor for SharedData takes one optional argument:
		/// a pointer to its parse master.
		/// </summary>
		explicit JsonSharedData(JsonParseMaster* parseMaster = nullptr);

		/// <summary>
		/// In my design, SharedData has no memory that it owns aside from
		/// one pointer (but not the memory pointed to) and one unsigned
		/// integer; both of these will be deallocated in the destruction of
		/// this object no matter what, so the destructor has nothing else
		/// special it needs to do; therefore, it has been set to the
		/// default. 
		/// </summary>
		virtual ~JsonSharedData() override = default;

		/// <summary>
		/// The copy constructor for JsonSharedData has been defaulted.
		/// </summary>
		/// <param name="other">- A reference to the JsonSharedData to copy.
		/// </param>
		JsonSharedData(const JsonSharedData& other) = default;

		/// <summary>
		/// The move constructor for JsonSharedData copies the pointer to the
		/// JsonParseMaster and the depth from the other JsonSharedData, and
		/// then it sets the other JsonSharedData's depth to 0 and its pointer
		/// to its JsonParseMaster to nullptr.
		/// </summary>
		/// <param name="other">- A reference to the JsonSharedData to move.
		/// </param>
		JsonSharedData(JsonSharedData&& other);

		/// <summary>
		/// The copy assignment operator for the JsonSharedData has been
		/// defaulted.
		/// </summary>
		/// <param name="other">- A reference to the JsonSharedData to copy.
		/// </param>
		/// <returns>A reference to this JsonSharedData.</returns>
		JsonSharedData& operator=(const JsonSharedData& other) = default;

		/// <summary>
		/// The move assignment operator for JsonSharedData copies the pointer
		/// to the JsonParseMaster and the depth from the other JsonSharedData,
		/// and then it sets the other JsonSharedData's depth to 0 and its
		/// pointer to its JsonParseMaster to nullptr.
		/// </summary>
		/// <param name="other">- A reference to the JsonSharedData to copy.
		/// </param>
		/// <returns>A reference to this JsonSharedData.</returns>
		JsonSharedData& operator=(JsonSharedData&& other);

		/// <summary>
		/// This is a virtual method that is meant to be overridden by derived
		/// classes as a user sees fit. The implementation in this base class
		/// dynamically allocates a new JsonSharedData and returns a pointer to
		/// it. Its depth is 0, and its pointer to a JsonParseMaster is the same
		/// as this object's pointer to a JsonParseMaster.
		/// </summary>
		/// <returns>A pointer to a new SharedData object, allocated on the
		/// heap.</returns>
		virtual JsonSharedData* const Clone() const;

		/// <summary>
		/// This method takes a pointer to a ParseMaster and assigns it as
		/// the ParseMaster that supposedly uses this SharedData.
		/// </summary>
		/// <param name="parseMaster">- A pointer to the ParseMaster that
		/// this object should consider its owner.</param>
		void SetParseMaster(JsonParseMaster* parseMaster);

		/// <summary>
		/// This method returns a pointer to this object's ParseMaster.
		/// </summary>
		/// <returns>A pointer to this object's ParseMaster.</returns>
		JsonParseMaster* GetParseMaster() const;

		/// <summary>
		/// This method increments the depth in the current JSON tree by 1.
		/// </summary>
		/// <returns>The newly incremented depth.</returns>
		uint32_t IncrementDepth();

		/// <summary>
		/// This method decrements the depth in the current JSON tree by 1.
		/// </summary>
		/// <returns>The newly decremented depth.</returns>
		uint32_t DecrementDepth();

		/// <summary>
		/// This method returns the current depth in the JSON tree.
		/// </summary>
		/// <returns></returns>
		uint32_t Depth() const;

		/// <summary>
		/// This method can be called to initialize a JsonSharedData object,
		/// specifically an object of a class that derives from JsonSharedData.
		/// </summary>
		virtual void Initialize();

	private:
		JsonParseMaster* mParseMaster;
		uint32_t mDepth{ 0u };
	};
}