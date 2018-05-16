#pragma once

#include "ActionList.h"

namespace LibraryShared
{
	/// <summary>
	/// This class is designed to encapsulate "for loop" functionality in JSON
	/// scripts to be written by users of the engine. It contains a pointer to a
	/// Datum that will contain the array of data over which the loop will
	/// iterate, as well as an index into that array.
	/// </summary>
	class ActionListForEach final : public ActionList
	{
	public:
		/// <summary>
		/// This is a default constructor. It initializes the member variables
		/// as specified in their declarations.
		/// </summary>
		ActionListForEach();

		/// <summary>
		/// This constructor allows users to specify the Datum containing the
		/// array over which the for loop will iterate, as well as the name of
		/// this Action.
		/// </summary>
		/// <param name="contents"></param>
		/// <param name="name"></param>
		explicit ActionListForEach(Datum& contents,
			const std::string& name = std::string());

		/// <summary>
		/// This is the destructor for ActionListForEach.
		/// </summary>
		virtual ~ActionListForEach() override = default;

	private:
		void RegisterMyAttributes();
		Datum* mContents{ nullptr };
		uint32_t mIndex{ 0u };
	};
}