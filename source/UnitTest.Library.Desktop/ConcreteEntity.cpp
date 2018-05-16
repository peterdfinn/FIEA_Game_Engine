#include "pch.h"
using namespace LibraryShared;
using namespace std;
namespace UnitTest
{
	RTTI_DEFINITIONS(ConcreteEntity);
	ConcreteEntity::ConcreteEntity(int32_t integer) : mConcreteEntityInteger(integer)
	{
		const uint64_t concreteEntityId = ConcreteEntity::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			concreteEntityId);
		RegisterAttribute(Signature("mConcreteEntityInteger"s,
			DatumType::INTEGER, 1u,
			offsetof(ConcreteEntity, mConcreteEntityInteger)),
			concreteEntityId);

		RegisterBaseType(concreteEntityId, Entity::TypeIdClass());
	}
}