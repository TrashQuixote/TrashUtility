#pragma once
#include "ConsoleAddon.h"
#include "PlayerControls.h"

namespace TrashHook {

	void Hook() {
		Trash_Function::Control::ForPlayerControl::GetSingleton()->Hook();
		TrashConsoleAddon::ConsoleAddon::Hook();
	}
}