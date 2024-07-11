#pragma once
#include "TrashFunction.h"

namespace Trash_Function::TimeMultSetter
{
	class TimeMult
	{
		using func_t = decltype(&RE::VATS::SetMagicTimeSlowdown);

	public:
		void SetMagicTimeSlowdown(const float& _WorldTimeMult, const float& _PlayerOnlyTimeMult);
		static TimeMult* GetSingleton();
		
		TimeMult(TimeMult& another) = delete;
		TimeMult(const TimeMult& another) = delete;
		void operator=(const TimeMult&) = delete;

		float world_time_mult = 1.0f;
		float player_time_mult = 1.0f;
	private:
		static inline REL::Relocation<func_t> _SetMagicTimeSlowdown;
		TimeMult();
		~TimeMult();
	};
	TimeMult::TimeMult() { _SetMagicTimeSlowdown = RELOCATION_ID(43103, 44300); }
	TimeMult::~TimeMult() {}

	inline void TimeMult::SetMagicTimeSlowdown(const float& _WorldTimeMult, const float& _PlayerOnlyTimeMult)
	{ 
		_SetMagicTimeSlowdown(RE::VATS::GetSingleton(), _WorldTimeMult, _PlayerOnlyTimeMult);
		world_time_mult = _WorldTimeMult == 0.0f ? 1.0f : _WorldTimeMult;
		player_time_mult = _PlayerOnlyTimeMult;
	}

	inline TimeMult* TimeMult::TimeMult::GetSingleton() { 
		static TimeMult* tm_Singleton = new TimeMult();
		return tm_Singleton;
	}

	inline void SetSlowTimeMult(float _WorldTimeMult, float _PlayerOnlyTimeMult, bool _Setter)
	{
		if (_Setter) TimeMult::GetSingleton()->SetMagicTimeSlowdown(_WorldTimeMult, _PlayerOnlyTimeMult);
		else TimeMult::GetSingleton()->SetMagicTimeSlowdown(TimeMult::GetSingleton()->world_time_mult * _WorldTimeMult, TimeMult::GetSingleton()->player_time_mult * _PlayerOnlyTimeMult);
	}

	inline float GetSlowTimeMult(bool _GetWorldTimeMult)
	{
		if (_GetWorldTimeMult) return TimeMult::GetSingleton()->world_time_mult;
		return TimeMult::GetSingleton()->player_time_mult;
	}

	inline void Papyrus_SetSlowTimeMult(RE::StaticFunctionTag*, float _WorldTimeMult = 0.0, float _PlayerOnlyTimeMult = 1.0, bool _Setter = true)
	{
		SetSlowTimeMult(_WorldTimeMult, _PlayerOnlyTimeMult, _Setter);
	}

	inline float Papyrus_GetSlowTimeMult(RE::StaticFunctionTag*, bool _GetWorldTimeMult = true)
	{
		return GetSlowTimeMult(_GetWorldTimeMult);
	}

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("SetSlowTimeMult"sv, pex_name_collection, Papyrus_SetSlowTimeMult);
		_vm->RegisterFunction("GetSlowTimeMult"sv, pex_name_collection, Papyrus_GetSlowTimeMult);
		return true;
	}
}
