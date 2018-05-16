#pragma once

#include "IJsonParseHelper.h"
#include "json/json.h"

namespace LibraryShared
{
	class JSONVectorHelper final : public IJsonParseHelper
	{
	public:
		/// <summary>
		/// This method attempts to handle a Json::Value, stored in a
		/// SharedData.
		/// </summary>
		/// <returns>True if this helper could handle the Json::Value stored in
		/// the SharedData, at the top of the stack; false otherwise.</returns>
		virtual bool Handle() override;

		virtual IJsonParseHelper* Create() const override;
	};
}