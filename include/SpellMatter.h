#pragma once
#include "TrashFunction.h"
using namespace Trash_Function::Misc;

namespace Trash_Function
{
	
	inline bool Check_InstantCast_Args(VM_VMStackID_ForCheck, const RE::TESObjectREFR* _caster, const RE::TESForm* _spell,const RE::TESForm* _KWorList)
	{
		if (!_caster) {
			_vm->TraceStack("caster is none", _stackID);
			return false;
		}
		if (!_spell) {
			_vm->TraceStack("_spell is none", _stackID);
			return false;
		}
		if (!_KWorList) {
			_vm->TraceStack("keyword or keywordlist is none", _stackID);
			return false;
		}
		return true;
	}

	inline bool Check_InstantCast_Args(VM_VMStackID_ForCheck, const RE::TESObjectREFR* _caster,const RE::TESForm* _spell)
	{
		if (!_caster) {
			_vm->TraceStack("caster is none", _stackID);
			return false;
		}
		if (!_spell) {
			_vm->TraceStack("_spell is none", _stackID);
			return false;
		}
		return true;
	}

	inline bool Check_Actor_Args(VM_VMStackID_ForCheck, const RE::Actor* _actor)
	{
		if (!_actor) {
			_vm->TraceStack("acotr is none", _stackID);
			return false;
		}
		return true;
	}

	inline bool Check_Perk_Args(VM_VMStackID_ForCheck, const RE::BGSPerk* _perk)
	{
		if (!_perk) {
			_vm->TraceStack("perk is none", _stackID);
			return false;
		}
		return true;
	}

}


namespace Trash_Function::SpellApplying
{
	namespace detail
	{
		inline bool IsMagicItem(const RE::TESForm* _magicitem_form)
		{
			switch (_magicitem_form->GetFormType())
			{
			case RE::FormType::Spell:  return true;
			case RE::FormType::Enchantment:  return true;
			case RE::FormType::AlchemyItem:  return true;
			case RE::FormType::Scroll:  return true;
			case RE::FormType::Ingredient:  return true;
			default:return false;
			}
		}

		inline bool DeliveryTypeIsSelf(const RE::TESForm* _magicitem_form)
		{
			switch (_magicitem_form->GetFormType())
			{
			case RE::FormType::Spell:  return true;
			case RE::FormType::Enchantment:  return true;
			case RE::FormType::AlchemyItem:  return true;
			case RE::FormType::Scroll:  return true;
			case RE::FormType::Ingredient:  return true;
			default:return false;
			}
		}

		inline void __InstantCastSpell(RE::TESObjectREFR* _Caster, RE::TESObjectREFR* _Target, RE::MagicItem* _Spell, const bool& _BlameCaster) {
			auto blame = _BlameCaster? _Caster->As<RE::Actor>() : nullptr;

			if (auto instant_magic_caster = _Caster->As<RE::Actor>()->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant))
			{
				instant_magic_caster->CastSpellImmediate(_Spell, false, _Target->As<RE::Actor>(), 1.0f, false, 0.0f, (blame ? blame : nullptr));
			}
		}

		inline bool _InstantCastSpell(RE::TESObjectREFR* _Caster, RE::TESObjectREFR* _Target, RE::MagicItem* _Spell,const bool& _CastToCaster,const bool& _BlameCaster) {
			if (!_CastToCaster && _Caster == _Target) return false;
			__InstantCastSpell(_Caster, _Target, _Spell, _BlameCaster);
			return true;
		}
		// _InstantCastSpellWithFilter - keyword array filter	
		inline bool _InstantCastSpellWithFilter(RE::TESObjectREFR* _Caster, RE::TESObjectREFR* _Target, RE::MagicItem* _Spell, const std::vector<RE::BGSKeyword*>& a_keywords, const bool& _CastToCaster, const bool& _MatchAllKeywords, const bool& _BlameCaster) {
			if (!_CastToCaster && _Caster == _Target) return false;
			if (_Target->HasKeywordInArray(a_keywords, _MatchAllKeywords)) __InstantCastSpell(_Caster, _Target, _Spell, _BlameCaster);
			else return false;
			return true;
		}

		// _InstantCastSpellWithFilter - keyword or keyword formlist filter
		inline bool _InstantCastSpellWithFilter(RE::TESObjectREFR* _Caster, RE::TESObjectREFR* _Target, RE::MagicItem* _Spell, RE::TESForm* _KWorList, const bool& _CastToCaster, const bool& _MatchAllKeywords, const bool& _BlameCaster)
		{
			if (!_CastToCaster && _Caster == _Target) return false;

			if (const auto keyword = _KWorList->As<RE::BGSKeyword>())
			{
				if (_Target->HasKeyword(keyword)) __InstantCastSpell(_Caster, _Target, _Spell, _BlameCaster);
			}
			else if (const auto list = _KWorList->As<RE::BGSListForm>())
			{
				if (_Target->HasKeywordInList(list, _MatchAllKeywords)) __InstantCastSpell(_Caster, _Target, _Spell, _BlameCaster);
			}
			else return false;
			return true;
		}
	}
	
	using namespace Trash_Function::SpellApplying::detail;

	std::vector<RE::TESForm*> InstantCastToActors(RE::TESObjectREFR* _Caster, RE::MagicItem* _Spell, const float& _Radius, const bool& _BlameCaster, const bool& _CastToCaster, RE::TESObjectREFR* _Center) {

		std::vector<RE::TESForm*> ret;
		
		if (const auto _TES = RE::TES::GetSingleton(); _TES) { 
			_TES->ForEachReferenceInRange(_Center, _Radius, [&](RE::TESObjectREFR* a_ref) {
				if (a_ref->Is3DLoaded()) {
					const auto obj = a_ref->GetBaseObject();
					if (a_ref->Is(RE::FormType::ActorCharacter) || obj && obj->Is(RE::FormType::ActorCharacter)) {
						if (_InstantCastSpell(_Caster, a_ref, _Spell, _CastToCaster, _BlameCaster)) ret.emplace_back(a_ref);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
		return ret;
	}

	std::vector<RE::TESForm*> InstantCastToActorsWithFilter(RE::TESObjectREFR* _Caster, RE::MagicItem* _Spell,const float& _Radius, const bool& _BlameCaster, const bool& _CastToCaster, RE::TESObjectREFR* _Center, RE::TESForm* _KWorList, const bool& _MatchAllKeywords) {
		if (!_KWorList->Is(RE::FormType::Keyword) && !_KWorList->Is(RE::FormType::FormList)) {
			RE::ConsoleLog::GetSingleton()->Print("Error occurred in function InstantCastToActorsWithFilter: Filter require keyword or a formlist of keywords");
			return {};
		}
			
		std::vector<RE::TESForm*> ret;
		if (const auto _TES = RE::TES::GetSingleton(); _TES) { 
			_TES->ForEachReferenceInRange(_Center, _Radius, [&](RE::TESObjectREFR* a_ref) {
				if (a_ref->Is3DLoaded()) {
					const auto obj = a_ref->GetBaseObject();
					if (a_ref->Is(RE::FormType::ActorCharacter) || obj && obj->Is(RE::FormType::ActorCharacter)) {
						if (_InstantCastSpellWithFilter(_Caster, a_ref, _Spell, _KWorList, _CastToCaster, _MatchAllKeywords, _BlameCaster)) ret.emplace_back(a_ref);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
		return ret;
	}

	std::vector<RE::TESForm*> InstantCastToActorsWithFilter(RE::TESObjectREFR* _Caster, RE::MagicItem* _Spell, const float& _Radius, const bool& _BlameCaster, const bool& _CastToCaster, RE::TESObjectREFR* _Center, const std::vector<RE::BGSKeyword*>& a_keywords, const bool& _MatchAllKeywords) {
		std::vector<RE::TESForm*> ret;
		if (const auto _TES = RE::TES::GetSingleton(); _TES) { 
			_TES->ForEachReferenceInRange(_Center, _Radius, [&](RE::TESObjectREFR* a_ref) {
				if (a_ref->Is3DLoaded()) {
					const auto obj = a_ref->GetBaseObject();
					if (a_ref->Is(RE::FormType::ActorCharacter) || obj && obj->Is(RE::FormType::ActorCharacter)) {
						if (_InstantCastSpellWithFilter(_Caster, a_ref, _Spell, a_keywords, _CastToCaster, _MatchAllKeywords, _BlameCaster)) ret.emplace_back(a_ref);
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
		return ret;
	}

//InstantCastSpellToActors
	std::vector<RE::TESForm*> Papyrus_InstantCastToActors(VM_VMStackID_FunTag ,RE::TESObjectREFR* _Caster, RE::TESForm* _Spell, float _Radius = 0.0f, bool _BlameCaster = false, bool _CastToCaster = false, RE::TESObjectREFR* _Center = nullptr)
	{	
		if (! Check_InstantCast_Args(VM_VMStackID_Parameter, _Caster, _Spell)) return {};
		if (! IsMagicItem(_Spell)) return {};
		if (_Center == nullptr) _Center = _Caster;

		return InstantCastToActors(_Caster, _Spell->As<RE::MagicItem>(), _Radius, _BlameCaster, _CastToCaster, _Center);
	}


//InstantCastSpellToActorsWithFilter - keyword or keyword formlist filter
	std::vector<RE::TESForm*> Papyrus_InstantCastToActorsWithFilter(VM_VMStackID_FunTag ,RE::TESObjectREFR* _Caster, RE::TESForm* _Spell, float _Radius = 0.0f, bool _BlameCaster = false, bool _CastToCaster = false, RE::TESObjectREFR* _Center = nullptr, RE::TESForm* _KWorList = nullptr, bool _MatchAllKeywords = false)
	{	
		if (! Check_InstantCast_Args(VM_VMStackID_Parameter, _Caster, _Spell,_KWorList)) return {};
		if (! IsMagicItem(_Spell)) return {};
		if (!_Center) _Center = _Caster;
		if (!_KWorList) return InstantCastToActors(_Caster, _Spell->As<RE::MagicItem>(), _Radius, _BlameCaster, _CastToCaster, _Center);

		return InstantCastToActorsWithFilter(_Caster, _Spell->As<RE::MagicItem>(),_Radius, _BlameCaster, _CastToCaster, _Center, _KWorList, _MatchAllKeywords);
	}

//InstantCastSpellToActorsWithFilter - keyword array filter
	std::vector<RE::TESForm*> Papyrus_InstantCastToActorsWithFilterAlt(VM_VMStackID_FunTag, RE::TESObjectREFR* _Caster, RE::TESForm* _Spell, float _Radius = 0.0f, bool _BlameCaster = false, bool _CastToCaster = false, RE::TESObjectREFR* _Center = nullptr, std::vector<RE::BGSKeyword*> a_keywords = {}, bool _MatchAllKeywords = false)
	{	
		if (! Check_InstantCast_Args(VM_VMStackID_Parameter, _Caster, _Spell) ) return {};
		if (! IsMagicItem(_Spell)) return {};
		if (_Center == nullptr) _Center = _Caster;
		if (a_keywords.empty()) return InstantCastToActors(_Caster, _Spell->As<RE::MagicItem>(), _Radius, _BlameCaster, _CastToCaster, _Center);

		return InstantCastToActorsWithFilter(_Caster, _Spell->As<RE::MagicItem>(), _Radius, _BlameCaster, _CastToCaster, _Center, a_keywords, _MatchAllKeywords);
	}

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("InstantCastToActors"sv, pex_name_function, Papyrus_InstantCastToActors);
		_vm->RegisterFunction("InstantCastToActorsWithFilter"sv, pex_name_function, Papyrus_InstantCastToActorsWithFilter);
		_vm->RegisterFunction("InstantCastToActorsWithFilterAlt"sv, pex_name_function, Papyrus_InstantCastToActorsWithFilterAlt);
		return true;
	}
}



