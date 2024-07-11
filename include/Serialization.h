#pragma once
#include "SerializationForCollection.h"

namespace TrashSerialization {
	void InitSerialization()
	{
		auto* _interface = SKSE::GetSerializationInterface();
		_interface->SetUniqueID(SerializationInfo::kModID);
		TrashSerialization::InitSerializationForCollection(_interface);
	}
}