#pragma once
#include "TrashCollection.h"
#include "UtilityVec.h"
#include "UtilityRefMap.h"

namespace TrashSerialization
{

	enum SerializationInfo : std::uint32_t
	{
		kModID = 'TRSH',
		kVersion = 1,

		// for collection
		kAuxArray = 'AVEC',
		kRefMap = 'RMAP'
	};
}
