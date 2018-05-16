#pragma once
#include "AbstractBase.h"
#include "Factory.h"
namespace UnitTest
{
	class ConcreteDerivedA final : public AbstractBase
	{
		RTTI_DECLARATIONS(ConcreteDerivedA, AbstractBase);
	public:
		virtual int32_t GetMember() const override;
	};

	ConcreteFactory(ConcreteDerivedA, AbstractBase)
}