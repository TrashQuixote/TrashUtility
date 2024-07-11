#pragma once
#include "SpellMatter.h"
#include "UtilityVec.h"
#include "UtilityRefMap.h"
#include "TrashSlowTime.h"
#include "ConsoleAddon.h"
#include "PlayerControls.h"

namespace TrashPapyrus {
	
	void FunctionRegister() {
		SKSE::GetPapyrusInterface()->Register(TrashUtility::DumpCollection::binding);
		SKSE::GetPapyrusInterface()->Register(TrashUtility::UtilityVec::binding);
		SKSE::GetPapyrusInterface()->Register(TrashUtility::UtilityRefMap::binding);
		SKSE::GetPapyrusInterface()->Register(Trash_Function::ApplyHit::binding);
		SKSE::GetPapyrusInterface()->Register(Trash_Function::Control::binding);
		SKSE::GetPapyrusInterface()->Register(Trash_Function::SpellApplying::binding);
		SKSE::GetPapyrusInterface()->Register(Trash_Function::TimeMultSetter::binding);
		SKSE::GetPapyrusInterface()->Register(Trash_Function::Misc::binding);
		SKSE::GetPapyrusInterface()->Register(Trash_Function::HeadingAngle::binding);
		SKSE::GetPapyrusInterface()->Register(Trash_Function::Pos::binding);
		SKSE::GetPapyrusInterface()->Register(Trash_Function::Sound::binding);
	}
}