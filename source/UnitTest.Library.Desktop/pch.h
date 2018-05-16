// pch for unit tests
// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Headers for CppUnitTest
#include "CppUnitTest.h"

// Headers for memory leak test checking
#define _CRTDBG_MAP_ALLOC

// TODO: reference additional headers your program requires here
#include <stdlib.h>
#include <crtdbg.h>
#include <cstdint>
#include <fstream>

#define GLM_FORCE_CXX98
#define GLM_ENABLE_EXPERIMENTAL

#include "Foo.h"
#include "glm\glm.hpp"
#include "Scope.h"
#include "Attributed.h"
#include "AttributedFoo.h"
#include "IJsonParseHelper.h"
#include "JsonSharedData.h"
#include "Datum.h"
#include "json/json.h"
#include "glm\gtx\string_cast.hpp"
#include "Factory.h"
#include "AbstractBase.h"
#include "ConcreteDerivedA.h"
#include "ConcreteDerivedB.h"
#include "JsonScopeSharedData.h"
#include "JsonScopeParseHelper.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "ConcreteEntity.h"
#include "ActionList.h"
#include "ActionCreateAction.h"
#include "ActionFoo.h"
#include "WorldState.h"
#include "GameTime.h"
#include "ActionDestroyAction.h"
#include "Event.h"
#include "DatumSubscriber.h"
#include "EventQueue.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "FloatSubscriber.h"
#include "EventSubscriberEnqueue.h"
#include "EventSubscriberUnsubscribe.h"