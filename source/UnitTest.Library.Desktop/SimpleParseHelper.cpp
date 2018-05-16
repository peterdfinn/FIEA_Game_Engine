#include "pch.h"
#include "SimpleParseHelper.h"
#include "SimpleSharedData.h"

using namespace LibraryShared;
using namespace std;

namespace UnitTest
{
	IParseHelper* SimpleParseHelper::Create()
	{
		return new SimpleParseHelper;
	}

	/*bool SimpleParseHelper::operator==(const IParseHelper& other) const
	{
		return operator==(dynamic_cast<const SimpleParseHelper&>(other));
	}*/

	bool SimpleParseHelper::operator==(const SimpleParseHelper& other) const
	{
		UNREFERENCED_PARAMETER(other);
		return true;
	}

	bool SimpleParseHelper::Handle(stringstream& bufferIn)
	{
		int firstCharacter = bufferIn.peek();

		/*First, we need to look at the first character in the stream and deal
		with cases that cannot be parsed into a Json::Value. Let's start with
		the end-of-file.*/
		if (firstCharacter == EOF)
		{
			return true;
		}

		/*Now let's deal with a right curly brace, which indicates the end of a
		JSON object.*/
		if (firstCharacter == '}')
		{
			bufferIn.get();
			GetParseMaster()->GetSharedData()->DecrementDepth();
			return Handle(bufferIn);
		}

		/*Now let's deal with a left curly brace, which indicates the beginning
		of a JSON object.*/
		if (firstCharacter == '{')
		{
			bufferIn.get();
			ParseMaster* master = GetParseMaster();
			ParseMaster::SharedData* sharedData = master->GetSharedData();
			sharedData->IncrementDepth();
			return Handle(bufferIn);
		}

		/*Now let's deal with a comma, which indicates a separation between
		entries within a JSON object.*/
		if (firstCharacter == ',')
		{
			bufferIn.get();
			return Handle(bufferIn);
		}

		/*Now let's deal with a colon, which indicates a pairing of a string to
		a value.*/
		if (firstCharacter == ':')
		{
			bufferIn.get();
			return Handle(bufferIn);
		}

		/*Now that we've dealt with all these special cases, let's deal with the
		first non-trivial case: the first character in the stream is '"',
		indicating the beginning of a name of a string-value pair.*/
		if (firstCharacter == '"')
		{
			int get = bufferIn.get();
			assert(get == '"');
			get = bufferIn.get();
			assert(get == 'i');
			get = bufferIn.get();
			assert(get == 'n');
			get = bufferIn.get();
			assert(get == 't');
			get = bufferIn.get();
			assert(get == '"');
			return Handle(bufferIn);
		}

		/*Finally, we'll deal with the final scenario in this simple parse
		handler: we've actually found the int value in this JSON file!*/
		Json::Value jsonValue;
		bufferIn >> jsonValue;

		Json::ValueType type = jsonValue.type();

		if (type == Json::ValueType::intValue)
		{
			SimpleSharedData* sharedData = dynamic_cast<SimpleSharedData*>(GetParseMaster()->GetSharedData());
			sharedData->dummy = jsonValue.asInt();
			return true;
		}
		

		return false;
	}
}