#include "pch.h"

#include "AttributedFoo.h"

using namespace LibraryShared;
using namespace std;
using namespace glm;

namespace UnitTest
{
	RTTI_DEFINITIONS(AttributedFoo);

	void AttributedFoo::RegisterAttributesAndBaseType()
	{
		const uint64_t attributedFooId = AttributedFoo::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			attributedFooId);
		RegisterAttribute(Signature("integerAttribute"s, DatumType::INTEGER, 1u,
			offsetof(AttributedFoo, integerAttribute)), attributedFooId);
		RegisterAttribute(Signature("floatAttribute"s,
			DatumType::FLOAT, 1u, offsetof(AttributedFoo, floatAttribute)),
			attributedFooId);
		RegisterAttribute(Signature("vectorAttribute"s, DatumType::VECTOR, 1u,
			offsetof(AttributedFoo, vectorAttribute)), attributedFooId);
		RegisterAttribute(Signature("matrixAttribute"s, DatumType::MATRIX, 1u,
			offsetof(AttributedFoo, matrixAttribute)), attributedFooId);
		RegisterAttribute(Signature("stringAttribute"s, DatumType::STRING, 1u,
			offsetof(AttributedFoo, stringAttribute)), attributedFooId);
		RegisterAttribute(Signature("pointerAttribute"s, DatumType::POINTER, 1u,
			offsetof(AttributedFoo, pointerAttribute)), attributedFooId);

		RegisterBaseType(attributedFooId, Attributed::TypeIdClass());
	}

	AttributedFoo::AttributedFoo(int32_t  intAttr, float floatAttr, glm::vec4 vecAttr,
		glm::mat4 matAttr, std::string stringAttr, LibraryShared::RTTI* pointerAttr) :
	integerAttribute(intAttr), floatAttribute(floatAttr), vectorAttribute(vecAttr),
	matrixAttribute(matAttr), stringAttribute(stringAttr), pointerAttribute(pointerAttr)
	{
		RegisterAttributesAndBaseType();
	}

	AttributedFoo::AttributedFoo()
	{
		RegisterAttributesAndBaseType();
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& other)
	{
		operator=(other);
	}

	AttributedFoo::AttributedFoo(AttributedFoo&& other)
	{
		operator=(move(other));
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& other)
	{
		if (&other != this)
		{
			Attributed::operator=(other);
			integerAttribute = other.integerAttribute;
			floatAttribute = other.floatAttribute;
			vectorAttribute = other.vectorAttribute;
			matrixAttribute = other.matrixAttribute;
			stringAttribute = other.stringAttribute;
			pointerAttribute = other.pointerAttribute;
		}
		return *this;
	}

	AttributedFoo& AttributedFoo::operator=(AttributedFoo&& other)
	{
		if (&other != this)
		{
			Attributed::operator=(move(other));
			integerAttribute = other.integerAttribute;
			floatAttribute = other.floatAttribute;
			vectorAttribute = other.vectorAttribute;
			matrixAttribute = other.matrixAttribute;
			stringAttribute = other.stringAttribute;
			pointerAttribute = other.pointerAttribute;
		}
		return *this;
	}
}