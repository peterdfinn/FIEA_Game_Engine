#include "pch.h"

using namespace std;

namespace LibraryShared
{
	RTTI_DEFINITIONS(JsonScopeParseHelper)
	IJsonParseHelper* const JsonScopeParseHelper::Create() const
	{
		return new JsonScopeParseHelper;
	}

	bool JsonScopeParseHelper::HandleEndOfObject()
	{
		JsonParseMaster* const master = GetParseMaster();
		if (master == nullptr)
		{
			throw exception("Can't handle parsing without a parse master!");
		}

		JsonSharedData* data = master->GetSharedData();
		if (data == nullptr)
		{
			throw exception("Can't handle parsing without shared data!");
		}

		JsonScopeSharedData* sharedData = data->As<JsonScopeSharedData>();
		if (sharedData != nullptr)
		{
			Scope* currentScope = sharedData->GetCurrentScope();
			Scope* parent = nullptr;
			if (currentScope != nullptr)
			{
				parent = currentScope->GetParent();
			}
			if ((parent != nullptr) && (sharedData != nullptr))
			{
				sharedData->SetCurrentScope(*parent);
			}
		}
		return true;
	}

	bool JsonScopeParseHelper::HandleBeginning(const string& key)
	{
		JsonScopeSharedData* sharedData = GetParseMaster()->GetSharedData()->As<JsonScopeSharedData>();
		if (sharedData == nullptr)
		{
			return false;
		}
		if ((key != "") && (key != "type") && (key != "value"))
		{
			if (sharedData->GetTopOfStackName() != "")
			{
				sharedData->PushStack();
			}
			sharedData->SetTopOfStackName(key);
		}
		return true;
	}

	bool JsonScopeParseHelper::Handle(const string& key, const Json::Value& value)
	{
		/*Let's start by getting the JsonSharedData used by the master. If it's
		not a JsonScopeSharedData, this helper cannot handle parsing into it:
		return false.*/
		const JsonParseMaster* const master = GetParseMaster();
		if (master == nullptr)
		{
			return false;
		}
		const JsonSharedData* const rawSharedData = master->GetSharedData();
		if (rawSharedData == nullptr)
		{
			return false;
		}
		JsonScopeSharedData* sharedData = rawSharedData
			->As<JsonScopeSharedData>();
		if (sharedData == nullptr)
		{
			return false;
		}

		/*We must do different things depending on the key.*/
		if (key == "type")
		{
			/*If key is "type", then value should be a string, and it must be
			"integer", "float", "vector", "matrix", "string", "table", or
			"pointer".*/
			if (!value.isString())
			{
				return false;
			}

			/*Now we must set the type in the topmost object of the stack. If it
			has already been set, that means we must be looking at the type of
			another Datum, so a new stack item must be pushed.*/
			bool newPush = false;
			if (sharedData->GetTopOfStackType() != DatumType::UNKNOWN)
			{
				sharedData->PushStack();
				newPush = true;
			}
			const string& type = value.asString();
			if (type == "integer")
			{
				sharedData->SetTopOfStackType(DatumType::INTEGER);
			}
			else if (type == "float")
			{
				sharedData->SetTopOfStackType(DatumType::FLOAT);
			}
			else if (type == "vector")
			{
				sharedData->SetTopOfStackType(DatumType::VECTOR);
			}
			else if (type == "matrix")
			{
				sharedData->SetTopOfStackType(DatumType::MATRIX);
			}
			else if (type == "string")
			{
				sharedData->SetTopOfStackType(DatumType::STRING);
			}
			else if (type == "table")
			{
				sharedData->SetTopOfStackType(DatumType::TABLE);
				const string& nameFromStack = sharedData->GetTopOfStackName();
				if (nameFromStack != "")
				{
					Scope* currentScope = sharedData->GetCurrentScope();
					if (currentScope != nullptr)
					{
						Scope& newChildScope = currentScope->AppendScope(nameFromStack);
						sharedData->SetCurrentScope(newChildScope);
					}
					
				}
			}
			else if (type == "world"s)
			{
				sharedData->SetTopOfStackType(DatumType::TABLE);
				const string& nameFromStack = sharedData->GetTopOfStackName();
				if (nameFromStack != "")
				{
					Scope* currentScope = sharedData->GetCurrentScope();
					if (currentScope != nullptr && currentScope->GetParent() != nullptr)
					{
						throw exception("A world must be one below the root of\
							the tree!");
						/*
						Scope& newChildScope = currentScope->AppendScope(nameFromStack);
						sharedData->SetCurrentScope(newChildScope);
						*/
					}
					World* world = new World(nameFromStack);
					currentScope->Adopt(nameFromStack, *world);
					sharedData->SetCurrentScope(*world);

				}
			}
			else if (type == "sector"s)
			{
				sharedData->SetTopOfStackType(DatumType::TABLE);
				const string& nameFromStack = sharedData->GetTopOfStackName();
				if (nameFromStack != "")
				{
					Scope* currentScope = sharedData->GetCurrentScope();
					if (currentScope != nullptr)
					{
						if (!currentScope->Is(World::TypeIdClass()))
						{
							throw exception("Cannot declare a Sector without a\
								parent World!");
						}
						sharedData->SetCurrentScope(*(currentScope->As<World>()->CreateSector(nameFromStack)));
						
					}

				}
			}
			else if (type == "entity"s)
			{
				sharedData->SetTopOfStackType(DatumType::TABLE);
				const string& nameFromStack = sharedData->GetTopOfStackName();
				if (nameFromStack != "")
				{
					Scope* currentScope = sharedData->GetCurrentScope();
					if (currentScope != nullptr)
					{
						if (!currentScope->Is(Sector::TypeIdClass()))
						{
							throw exception("Cannot declare a Entity outside of\
								a parent Sector!");
						}
						sharedData->SetCurrentScope(*(currentScope->As<Sector>()->CreateEntity("Entity"s, nameFromStack)));

					}

				}
			}
			else if (type == "actionlist"s || type == "actioncreateaction"s || type == "actiondestroyaction"s)
			{
				sharedData->SetTopOfStackType(DatumType::TABLE);
				const string& nameFromStack = sharedData->GetTopOfStackName();
				if (nameFromStack != "")
				{
					Scope* currentScope = sharedData->GetCurrentScope();
					if (currentScope != nullptr)
					{
						if (!currentScope->Is(Sector::TypeIdClass()) &&
							!currentScope->Is(World::TypeIdClass()) &&
							!currentScope->Is(Entity::TypeIdClass()) &&
							!currentScope->Is(Action::TypeIdClass()))
						{
							throw exception("Cannot declare an Action outside\
								of a parent World, Sector, Entity, or Action!");
						}
						sharedData->SetCurrentScope(*(currentScope->As<Sector>()->CreateEntity("Entity"s, nameFromStack)));

					}

				}
			}
			else
			{
				if (newPush)
				{
					sharedData->PopStack();
				}
				return false;
			}
		}
		else if (key == "value")
		{
			/*If the key is "value", then we must set the Json::Value pointer on
			the top of the stack to point to the input Json::Value, unless that
			pointer already points to a Json::Value, in which case a new stack
			item must be pushed, and then its pointer should point to the input
			Json::Value.*/
			if (sharedData->GetTopOfStackValue() != nullptr)
			{
				sharedData->PushStack();
			}
			sharedData->SetTopOfStackValue(&value);
		}
		else if (key == "foreach"s)
		{
			Datum* const contentsDatum = sharedData->GetCurrentScope()->Search(value["contents"].asString());
			sharedData->PushStack();
			sharedData->SetTopOfStackName(key);
			sharedData->SetTopOfStackType(DatumType::TABLE);
			sharedData->SetTopOfStackValue(&value);
			ActionListForEach* forEach = new ActionListForEach(*contentsDatum);
			sharedData->GetCurrentScope()->Adopt(""s, *forEach);
			sharedData->SetCurrentScope(*forEach);
		}
		else if (key == "")
		{
			/*If key is an empty string, then that means value is expected to be
			the root of the JSON tree. If value is not an object, then return
			false. Nothing else needs to be done here.*/
			if (!value.isObject())
			{
				return false;
			}
		}
		else
		{
			/*If key is not "type", "value", or the empty string, then it is
			assumed to be the name of a new Datum.*/
			sharedData->PushStack();
			sharedData->SetTopOfStackName(key);
		}

		/*If there is a Json::Value on the stack, if there is a type on the
		stack, and if the type and value are incompatible, we must pop that item
		off the stack and return false.*/
		if ((sharedData->GetTopOfStackValue() != nullptr) &&
			!TypeAndValueAreCompatible(sharedData->GetTopOfStackType(), *sharedData->GetTopOfStackValue()))
		{
			sharedData->PopStack();
			return false;
		}

		/*Finally, at long last, we can check to see if there is a complete
		object on the top of the stack, with a non-empty name, a valid type, and
		a non-null pointer.*/
		const Json::Value* const valueFromStack = sharedData->GetTopOfStackValue();
		if (valueFromStack != nullptr)
		{
			const string& nameFromStack = sharedData->GetTopOfStackName();
			const DatumType typeFromStack = sharedData->GetTopOfStackType();
			Json::ArrayIndex sizeInValue = valueFromStack->size();
			if (sizeInValue == 0)
			{
				++sizeInValue; // if scalar, size() returns 0
			}
			if ((nameFromStack != "") && (typeFromStack != DatumType::UNKNOWN))
			{
				if (typeFromStack != DatumType::TABLE)
				{
					Datum& d = sharedData->GetCurrentScope()->Append(nameFromStack);
					d.SetType(typeFromStack);
					d.SetSize(sizeInValue);
					const Json::Value& valueRef = *valueFromStack;
					for (Json::ArrayIndex i = 0; i < sizeInValue; ++i)
					{
						switch (typeFromStack)
						{
						case DatumType::INTEGER:
							if (valueRef.isArray())
							{
								d.Set(valueRef[i].asInt(), i);
							}
							else
							{
								d.Set(valueRef.asInt());
							}
							break;
						case DatumType::FLOAT:
							if (valueRef.isArray())
							{
								d.Set(static_cast<float>(valueRef[i].asDouble()), i);
							}
							else
							{
								d.Set(static_cast<float>(valueRef.asDouble()));
							}
							break;
						case DatumType::VECTOR:
						case DatumType::MATRIX:
						case DatumType::STRING:
							if (valueRef.isArray())
							{
								d.SetFromString(valueRef[i].asString(), i);
							}
							else
							{
								d.SetFromString(valueRef.asString());
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}

		return true;
	}

	bool JsonScopeParseHelper::TypeAndValueAreCompatible(DatumType type,
		const Json::Value& value)
	{
		Json::ArrayIndex valueSize = value.size();
		
		float f; //dummy for sscanf_s

		switch (type)
		{
		case DatumType::INTEGER:
			if (value.isIntegral())
			{
				break;
			}
			if (value.isArray())
			{
				for (Json::ArrayIndex i = 0; i < valueSize; ++i)
				{
					if (!value[i].isIntegral())
					{
						return false;
					}
				}
			}
			break;
		case DatumType::FLOAT:
			if (value.isDouble())
			{
				break;
			}
			if (value.isArray())
			{
				for (Json::ArrayIndex i = 0; i < valueSize; ++i)
				{
					if (!value[i].isDouble())
					{
						return false;
					}
				}
			}
			break;
		case DatumType::VECTOR:
			if (value.isString())
			{
				if (sscanf_s(value.asCString(), "vec4(%f, %f, %f, %f)", &f, &f, &f, &f) != EOF)
				{
					break;
				}
			}
			if (value.isArray())
			{
				for (Json::ArrayIndex i = 0; i < valueSize; ++i)
				{
					if (!value[i].isString())
					{
						return false;
					}
					if (sscanf_s(value[i].asCString(), "vec4(%f, %f, %f, %f)", &f, &f, &f, &f)
						== EOF)
					{
						return false;
					}
				}
			}
			break;
		case DatumType::MATRIX:
			if (value.isString())
			{
				if (sscanf_s(value.asCString(),
					"mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
					&f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f)
					!= EOF)
				{
					break;
				}
			}
			if (value.isArray())
			{
				for (Json::ArrayIndex i = 0; i < valueSize; ++i)
				{
					if (!value[i].isString())
					{
						return false;
					}
					if (sscanf_s(value[i].asCString(),
						"mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
						&f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f, &f)
						== EOF)
					{
						return false;
					}
				}
			}
			break;
		case DatumType::STRING:
			if (value.isString())
			{
				break;
			}
			if (value.isArray())
			{
				for (Json::ArrayIndex i = 0; i < valueSize; ++i)
				{
					if (!value[i].isString())
					{
						return false;
					}
				}
			}
			break;
		case DatumType::TABLE:
			if (value.isObject())
			{
				break;
			}
			if (value.isArray())
			{
				for (Json::ArrayIndex i = 0; i < valueSize; ++i)
				{
					if (!value[i].isObject())
					{
						return false;
					}
				}
			}
		default:
			return false;
		}
		
		return true;
	}
}