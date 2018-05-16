#pragma once

#include "RTTI.h"

namespace UnitTest
{
	class Foo : public LibraryShared::RTTI
	{
		RTTI_DECLARATIONS(Foo, LibraryShared::RTTI)
	public:
		explicit Foo(int value = 0);
		Foo(const Foo& other);
		Foo(Foo&& other);
		virtual ~Foo();
		void SetValue(int newValue);
		int GetValue() const;
		Foo& operator=(const Foo& other);
		Foo& operator=(Foo&& other);
		bool operator==(const Foo& other) const;
		bool operator()(const Foo& foo1, const Foo& foo2) const;
		bool operator!=(const Foo& other) const;
	private:
		int* valuePtr;
	};
}