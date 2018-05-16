#pragma once
#include "AbstractBase.h"
#include "Factory.h"
namespace UnitTest
{
	class ConcreteDerivedB final : public AbstractBase
	{
		RTTI_DECLARATIONS(ConcreteDerivedB, AbstractBase);
	public:
		virtual int32_t GetMember() const override;
	};

	ConcreteFactory(ConcreteDerivedB, AbstractBase)
}