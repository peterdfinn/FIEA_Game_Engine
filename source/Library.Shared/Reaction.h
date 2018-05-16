#pragma once

#include "ActionList.h"
#include "EventSubscriber.h"

namespace LibraryShared
{
	/// <summary>
	/// This class is an interface that acts as a base from which any concrete
	/// Reactions should inherit. A Reaction is a list of actions that is
	/// executed upon receipt of an event; hence, this class inherits from both
	/// ActionList and EventSubscriber.
	/// </summary>
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);
	public:
		/// <summary>
		/// This is the default constructor for Reaction. It has been defaulted.
		/// </summary>
		Reaction() = default;

		/// <summary>
		/// This is the copy constructor for Reaction. It has been deleted.
		/// </summary>
		/// <param name=""></param>
		Reaction(const Reaction&) = delete;

		/// <summary>
		/// This is the copy assignment operator for Reaction. It has been
		/// deleted.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		Reaction& operator=(const Reaction&) = delete;

		/// <summary>
		/// This is the move constructor for Reaction. It has been defaulted.
		/// </summary>
		/// <param name=""></param>
		Reaction(Reaction&&) = default;

		/// <summary>
		/// This is the move assignment operator for Reaction. It has been
		/// defaulted.
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		Reaction& operator=(Reaction&&) = default;

		/// <summary>
		/// This is the destructor for Reaction. It has been defaulted.
		/// </summary>
		virtual ~Reaction() override = default;
	};
}