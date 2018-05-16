#pragma once
#include "Entity.h"
#include "Factory.h"
using namespace LibraryShared;
namespace UnitTest
{
	class ConcreteEntity final : public Entity
	{
	RTTI_DECLARATIONS(ConcreteEntity, Entity);
	public:
		ConcreteEntity(int32_t integer = -1);
	private:
		int32_t mConcreteEntityInteger;
	};


	ConcreteFactory(ConcreteEntity, Entity);
}