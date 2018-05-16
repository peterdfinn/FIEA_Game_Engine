#pragma once
#include "pch.h"
namespace UnitTest
{
	class AttributedFoo : public LibraryShared::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, LibraryShared::Attributed)

	public:
		AttributedFoo(int32_t intAttr, float floatAttr, glm::vec4 vecAttr,
			glm::mat4 matAttr, std::string stringAttr, LibraryShared::RTTI* pointerAttr);

		AttributedFoo();

		virtual ~AttributedFoo() override = default;

		AttributedFoo(const AttributedFoo& other);

		AttributedFoo& operator=(const AttributedFoo& other);

		AttributedFoo(AttributedFoo&& other);

		AttributedFoo& operator=(AttributedFoo&& other);

	private:
		int32_t integerAttribute{ 0 };
		float floatAttribute{0.0f};
		glm::vec4 vectorAttribute{0.0f, 0.0f, 0.0f, 0.0f};
		glm::mat4 matrixAttribute{vectorAttribute, vectorAttribute, vectorAttribute, vectorAttribute};
		std::string stringAttribute{""};
		LibraryShared::RTTI* pointerAttribute{ nullptr };

		void RegisterAttributesAndBaseType();
	};
}