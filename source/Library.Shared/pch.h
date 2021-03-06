#pragma once

// Standard
#include <cstdint>
#include <cstdlib>

// Engine-specific requirements
#include <fstream>
#include <vector>
#include <algorithm>
#include <future>
#include <functional>

// Header files written by me
#include "Scope.h"
#include "IJsonParseHelper.h"
#include "HashMap.h"
#include "Vector.h"
#include "json/json.h"
#include "JsonSharedData.h"
#include "JsonScopeSharedData.h"
#include "JsonScopeParseHelper.h"
#include "GameTime.h"
#include "GameClock.h"
#include "Entity.h"
#include "Attributed.h"
#include "TypeManager.h"
#include "Sector.h"
#include "World.h"
#include "WorldState.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionListForEach.h"
#include "EventPublisher.h"
#include "Event.h"
#include "EventQueue.h"
#include "EventSubscriber.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"