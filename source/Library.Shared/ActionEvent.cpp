#include "pch.h"

using namespace std;
using namespace std::chrono;

using MessageT = LibraryShared::EventMessageAttributed;
using EventT = LibraryShared::Event<LibraryShared::EventMessageAttributed>;

namespace LibraryShared
{
	ActionEvent::ActionEvent(const string& subtype, milliseconds delay, 
		const string& name) : Action(name), mDelay(delay), mSubtype(subtype)
	{
		const uint64_t actionEventId = ActionEvent::TypeIdClass();
		RegisterAttribute(Signature("this"s, DatumType::POINTER, 1u, 0u),
			actionEventId);
		RegisterAttribute(Signature("mDelay"s, DatumType::INTEGER, 1u,
			offsetof(ActionEvent, mDelay)), actionEventId);
		RegisterAttribute(Signature("mSubtype"s, DatumType::STRING,
			1u, offsetof(ActionEvent, mSubtype)), actionEventId);

		RegisterBaseType(actionEventId, Action::TypeIdClass());
	}

	milliseconds ActionEvent::GetDelay() const
	{
		return mDelay;
	}

	void ActionEvent::SetDelay(milliseconds delay)
	{
		mDelay = delay;
	}

	void ActionEvent::SetSubtype(const string& subtype)
	{
		mSubtype = subtype;
	}

	const string& ActionEvent::GetSubtype() const
	{
		return mSubtype;
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		if (GetTable() != nullptr && worldState.GetGameTime() != nullptr)
		{
			World* const world = worldState.GetWorld();
			if (world != nullptr)
			{
				MessageT ema(mSubtype, world);
				for (const auto& pair : *GetTable())
				{
					if (IsAuxiliaryAttribute(pair.first))
					{
						ema.AppendAuxiliaryAttribute(pair.first) = pair.second;
					}
				}
				mEvent = world->GetEventQueue().Enqueue<MessageT>(EventT(ema),
				*worldState.GetGameTime(), mDelay);
			}
		}
	}
}