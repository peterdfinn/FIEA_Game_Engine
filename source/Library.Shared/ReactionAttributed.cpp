#include "pch.h"

using namespace std;

using Message = LibraryShared::EventMessageAttributed;
using RawEvent = LibraryShared::Event<Message>;

namespace LibraryShared
{
	RTTI_DEFINITIONS(ReactionAttributed);

	ReactionAttributed::ReactionAttributed(const string& subtype) : Reaction(),
		mSubtype(subtype)
	{
		const uint64_t reactionAttributedId = ReactionAttributed::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			reactionAttributedId);
		RegisterAttribute(Signature("mSubtype"s, DatumType::STRING, 1u,
				offsetof(ReactionAttributed, mSubtype)), reactionAttributedId);
		RegisterAttribute(Signature("mEventMessage"s, DatumType::POINTER, 1u,
			offsetof(ReactionAttributed, mEventMessage)), reactionAttributedId);

		RegisterBaseType(reactionAttributedId, Reaction::TypeIdClass());

		RawEvent::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& other) :
		Reaction(move(other)), mEventMessage(other.mEventMessage),
		mSubtype(move(other.mSubtype))
	{
		other.mEventMessage = nullptr;
	}

	ReactionAttributed& ReactionAttributed::operator=(ReactionAttributed&& other)
	{
		if (this != &other)
		{
			Reaction::operator=(move(other));
			mEventMessage = other.mEventMessage;
			other.mEventMessage = nullptr;
			mSubtype = move(other.mSubtype);
		}
		return *this;
	}

	ReactionAttributed::~ReactionAttributed()
	{
		RawEvent::Unsubscribe(*this);
	}

	void ReactionAttributed::SetSubtype(const string& subtype)
	{
		mSubtype = subtype;
	}

	const string& ReactionAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	const Message* ReactionAttributed::GetEventMessage() const
	{
		return mEventMessage;
	}

	void ReactionAttributed::Notify(const EventPublisher& event)
	{
		if (event.Is(RawEvent::TypeIdClass()))
		{
			const RawEvent& rawEvent = static_cast<const RawEvent&>(event);
			const Message& rawMessage = rawEvent.Message();
			if (rawMessage.GetSubtype() == mSubtype)
			{
				mEventMessage = &rawMessage;
			}
		}
	}
}