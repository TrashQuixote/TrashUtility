#pragma once
#include "TrashFunction.h"


namespace Trash_Function::ApplyHit
{
	inline RE::EnchantmentItem* GetEnchantmentFromForm(RE::TESForm* weap_form, RE::ExtraDataList* ed_list)
	{
		using func_t = decltype(&GetEnchantmentFromForm);
		REL::Relocation<func_t> func{ RELOCATION_ID(14411, 14563) };
		return func(weap_form, ed_list);
	}

	inline void OnHit(RE::Character* _attacker, RE::Character* _victim, RE::Projectile* _proj, bool _left)
	{
		using func_t = decltype(&OnHit);
		REL::Relocation<func_t> func{ RELOCATION_ID(37673, 38627) };
		return func(_attacker, _victim, _proj, _left);
	}
	
	inline void ApplyHitData(RE::Character* victim, RE::HitData* _data)
	{
		using func_t = decltype(&ApplyHitData);
		REL::Relocation<func_t> func{ RELOCATION_ID(37633, 38586) };
		return func(victim, _data);
	}
	
	inline void HitDataInit(RE::HitData* _data)
	{
		using func_t = decltype(&HitDataInit);
		REL::Relocation<func_t> func{ RELOCATION_ID(42826, 43995) };
		return func(_data);
	}

	inline void RemoveMagicEffectsDueToAction(RE::Actor* _this, int Archetype = -1)
	{
		using func_t = decltype(&RemoveMagicEffectsDueToAction);
		REL::Relocation<func_t> func{ RELOCATION_ID(37864, 38819) };
		return func(_this, Archetype);
	}

	inline void ApplyMeleeHit(RE::Character* _attacker, RE::Character* _victim, bool _left) {
		if (_attacker && _victim) {
			OnHit(_attacker, _victim, nullptr, _left);
			RemoveMagicEffectsDueToAction(_attacker);
		}
	}


	inline void ApplyHit(RE::Character* _attacker, RE::Character* _victim, RE::TESObjectWEAP* _weapon, bool _applyench) {
		if (_attacker && _victim && _weapon) {
			RE::HitData _hitdata;
			RE::InventoryEntryData _ied{ _weapon, 1 };
			HitDataInit(&_hitdata);
			_hitdata.Populate(_attacker, _victim, &_ied);
			ApplyHitData(_victim, &_hitdata);
			RemoveMagicEffectsDueToAction(_attacker);
			if (_applyench) {
				auto ench = GetEnchantmentFromForm(_weapon,nullptr);
				if (ench) {
					_attacker->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
						->CastSpellImmediate(ench, false, _victim->As<RE::TESObjectREFR>(), 1.0f, false, 0.0f, _attacker);
				}
			}
			_ied.~InventoryEntryData();
		}
	}


	inline void ApplyProjHit(RE::Character* _attacker, RE::Character* _victim, RE::Projectile* _proj, bool _left) {
		if (_attacker && _victim) {
			OnHit(_attacker, _victim, _proj, _left);
			RemoveMagicEffectsDueToAction(_attacker);
		}
	}
	

	inline void ApplyProjHit(RE::Character* _attacker, RE::Character* _victim, RE::TESAmmo* _BaseAmmo, RE::TESObjectWEAP* _BaseWeap)
	{
		RE::NiPoint3 DePoint = _attacker->GetPosition() - _victim->GetPosition();
		RE::Projectile::LaunchData _launch_data{ _attacker, _attacker->GetPosition(), { DePoint.x, DePoint.z }, _BaseAmmo,
			_BaseWeap };
		_launch_data.desiredTarget = _victim;
		_launch_data.autoAim = true;
		
		RE::ProjectileHandle res;
		RE::Projectile::Launch(&res, _launch_data);
		auto proj_ref = res.get().get();
		
		if (!proj_ref) {
			RE::ConsoleLog::GetSingleton()->Print("proj ref is none ");
			return;
		}
		ApplyProjHit(_attacker, _victim, proj_ref, false);
		res.~BSPointerHandle();
	}

	inline void Papyrus_ApplyHit(VM_VMStackID_FunTag, RE::Character* _attacker, RE::Character* _victim, RE::TESObjectWEAP* _weapon, bool _applyench = true) {
		if (!_attacker ) {
			_vm->TraceStack("attacker is none", _stackID);
			return;
		}
		if (!_victim) {
			_vm->TraceStack("victim is none", _stackID);
			return;
		}
		if (!_weapon) {
			_vm->TraceStack("weapon is none", _stackID);
			return;
		}
		ApplyHit(_attacker, _victim, _weapon, _applyench);
	}

	inline void Papyrus_ApplyMeleeHit(VM_VMStackID_FunTag, RE::Character* _attacker, RE::Character* _victim, bool _left = false) {
		RE::ConsoleLog::GetSingleton()->Print("Papyrus_ApplyMeleeHit Start");
		if (!_attacker) {
			_vm->TraceStack("_attacker is none", _stackID);
			return;
		}
		if (!_victim) {
			_vm->TraceStack("_victim is none", _stackID);
			return;
		}
		RE::ConsoleLog::GetSingleton()->Print("Papyrus_ApplyMeleeHit CheckPass");
		ApplyMeleeHit(_attacker, _victim, _left);
	}

	//not now
	inline void Papyrus_ApplyProjHit(VM_VMStackID_FunTag, RE::Character* _attacker, RE::Character* _victim, RE::Projectile* _proj) {
		if (!_attacker) {
			_vm->TraceStack("_attacker is none", _stackID);
			return;
		}
		if (!_victim) {
			_vm->TraceStack("_victim is none", _stackID);
			return;
		}
		if (!_proj) {
			_vm->TraceStack("_victim is none", _stackID);
			return;
		}
		ApplyProjHit(_attacker, _victim, _proj, false);
	}

	inline bool binding(VM* _vm)
	{
		//_vm->RegisterFunction("ApplyProjHit"sv, pex_name_function, Papyrus_ApplyProjHit);
		_vm->RegisterFunction("ApplyMeleeHit"sv, pex_name_function, Papyrus_ApplyMeleeHit);
		_vm->RegisterFunction("ApplyHit"sv, pex_name_function, Papyrus_ApplyHit);
		return true;
	}

}

