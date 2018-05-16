#include "pch.h"

#include "Foo.h"

namespace UnitTest
{
	RTTI_DEFINITIONS(Foo)

	Foo::Foo(int value) : valuePtr(new int(value)) {}

	Foo::Foo(const Foo& other) : valuePtr(new int(other.GetValue()))
	{
		operator=(other);
	}

	Foo::Foo(Foo&& other) : valuePtr(other.valuePtr)
	{
		other.valuePtr = nullptr;
	}

	Foo::~Foo()
	{
		delete valuePtr;
	}

	bool Foo::operator()(const Foo& foo1, const Foo& foo2) const
	{
		return foo1.operator==(foo2);
	}

	int Foo::GetValue() const
	{
		return *valuePtr;
	}
	
	void Foo::SetValue(int newValue)
	{
		*valuePtr = newValue;
	}


	Foo& Foo::operator=(const Foo& other)
	{
		if (&other != this)
		{
			delete valuePtr;
			valuePtr = new int(*(other.valuePtr));
		}

		return *this;
	}

	Foo& Foo::operator=(Foo&& other)
	{
		delete valuePtr;
		valuePtr = other.valuePtr;
		return *this;
	}

	bool Foo::operator==(const Foo& other) const
	{
		return (*valuePtr == *(other.valuePtr));
	}

	bool Foo::operator!=(const Foo& other) const
	{
		return !operator==(other);
	}
}