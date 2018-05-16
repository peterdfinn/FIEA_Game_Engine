#include "pch.h"

using namespace std;

namespace LibraryShared
{
	EventMessageAttributed::EventMessageAttributed(const string& subtype,
		World* world) : Attributed(), mSubtype(subtype), mWorld(world)
	{
		const uint64_t eventMessageAttributedId =
			EventMessageAttributed::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			eventMessageAttributedId);
		RegisterAttribute(Signature("mSubtype"s, DatumType::STRING, 1u,
				offsetof(EventMessageAttributed, mSubtype)),
			eventMessageAttributedId);
		RegisterAttribute(Signature("mWorld"s, DatumType::POINTER, 1u,
			offsetof(EventMessageAttributed, mWorld)),
			eventMessageAttributedId);

		RegisterBaseType(eventMessageAttributedId, Attributed::TypeIdClass());
	}

	EventMessageAttributed::EventMessageAttributed(EventMessageAttributed&& other)
		: Attributed(move(other)), mWorld(other.mWorld),
		mSubtype(move(other.mSubtype))
	{
		other.SetWorld(nullptr);
	}

	EventMessageAttributed& EventMessageAttributed::operator=(EventMessageAttributed&& other)
	{
		if (&other != this)
		{
			Attributed::operator=(move(other));
			SetWorld(other.mWorld);
			other.SetWorld(nullptr);
			mSubtype = move(other.mSubtype);
		}
		return *this;
	}


	World* EventMessageAttributed::GetWorld() const
	{
		return mWorld;
	}

	void EventMessageAttributed::SetWorld(World* world)
	{
		mWorld = world;
	}

	const string& EventMessageAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	void EventMessageAttributed::SetSubtype(const string& subtype)
	{
		mSubtype = subtype;
	}
}