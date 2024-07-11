#pragma once
#include "TrashFunction.h"

namespace Trash_Function::Sound
{
	inline bool PlaySoundAtPoint(RE::TESForm* _sound,const float& _x,const float& _y,const float& _z) {
		if ( !_sound || _sound->GetFormType() != RE::FormType::SoundRecord) return false;
		if (auto _BGSoundDescriptorForm = _sound->As<RE::BGSSoundDescriptorForm>()) {
			if(auto _BGSoundDescriptor = _BGSoundDescriptorForm->soundDescriptor){
				RE::BSSoundHandle sound;
				RE::BSAudioManager::GetSingleton()->BuildSoundDataFromDescriptor(sound, _BGSoundDescriptor);
				if (sound.IsValid()) {
					sound.SetPosition( {_x,_y,_z} );
					return sound.Play();
				}
			}
		}
		return false;
	}

	inline bool Papyrus_PlaySoundAtPoint(VM_VMStackID_ForCheck, RE::TESForm* _sound, float _x, float _y, float _z)
	{
		if (!_sound) {
			_vm->TraceStack("sound form is none",_stackID);
			return false;
		}
		return PlaySoundAtPoint(_sound, _x, _y, _z);
	}

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("PlaySoundAtPoint"sv, pex_name_function, Papyrus_PlaySoundAtPoint);
		return true;
	}
}

