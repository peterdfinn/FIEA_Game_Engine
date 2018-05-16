#pragma once

#include "IJsonParseHelper.h"

namespace LibraryShared
{
	class JSONTableHelper : public IJsonParseHelper
	{
		/// <summary>
		/// This method attempts to handle a Json::Value, stored in a
		/// SharedData.
		/// </summary>
		/// <returns>True if this helper could handle the Json::Value stored in
		/// the SharedData, at the top of the stack; false otherwise.</returns>
		virtual bool Handle() override;

		virtual IJsonParseHelper* const Create() const override;
	};
}