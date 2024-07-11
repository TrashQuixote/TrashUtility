#pragma once
#include "TrashFunction.h"
using namespace Trash_Function::Misc;

namespace Trash_Function::Distance
{
	inline bool GetInSameCellOrWorldSpace(RE::TESObjectREFR* A_ref,RE::TESObjectREFR* B_ref,bool _flag = true)
	{
		using func_t = decltype(&GetInSameCellOrWorldSpace);
		REL::Relocation<func_t> func{ RELOCATION_ID(19092, 19494) };
		return func(A_ref,B_ref,_flag);
	}


	inline float GetDistanceBetweenPoints2D(float fst_x, float fst_y, float sec_x, float sec_y)
	{
		float _dis = (fst_x - sec_x) * (fst_x - sec_x) + (fst_y - sec_y) * (fst_y - sec_y);
		SetMaxWhenOverFlow(_dis);
		return std::sqrtf(_dis);
	}

	inline float GetDistanceFromPoint2D(RE::TESObjectREFR* _ref, float _x, float _y)
	{
		return GetDistanceBetweenPoints2D(_ref->data.location.x, _ref->data.location.y, _x, _y);
	}

	inline float GetDistance2D(RE::TESObjectREFR* fst_ref, RE::TESObjectREFR* sec_ref)
	{
		if (!GetInSameCellOrWorldSpace(fst_ref, sec_ref)) return FLT_MAX;
		return GetDistanceBetweenPoints2D(fst_ref->data.location.x, fst_ref->data.location.y, sec_ref->data.location.x,sec_ref->data.location.y);
	}

	inline float GetDistanceBetweenPoints3D(float fst_x, float fst_y, float fst_z, float sec_x, float sec_y, float sec_z)
	{
		float _dis = (fst_x - sec_x) * (fst_x - sec_x) + (fst_y - sec_y) * (fst_y - sec_y) + (fst_z - sec_z) * (fst_z - sec_z);
		SetMaxWhenOverFlow(_dis);
		return std::sqrtf(_dis);
	}

	inline float GetDistanceFromPoint3D(RE::TESObjectREFR* _ref, float _x, float _y, float _z)
	{
		return GetDistanceBetweenPoints3D(_ref->data.location.x, _ref->data.location.y, _ref->data.location.z, _x, _y, _z);
	}

	inline float GetDistance3D(RE::TESObjectREFR* fst_ref, RE::TESObjectREFR* sec_ref)
	{
		if (!GetInSameCellOrWorldSpace(fst_ref, sec_ref)) return FLT_MAX;
		return GetDistanceBetweenPoints3D(fst_ref->data.location.x, fst_ref->data.location.y, fst_ref->data.location.z,
			sec_ref->data.location.x, sec_ref->data.location.y, sec_ref->data.location.z);
	}
	//GetDistanceBetweenPoints2D
	inline float Papyrus_GetDistanceBetweenPoints2D(RE::StaticFunctionTag*, float fst_x, float fst_y, float sec_x, float sec_y)
	{
		return GetDistanceBetweenPoints2D(fst_x, fst_y, sec_x, sec_y);
	}

	//GetDistanceBetweenPoints2D
	inline float Papyrus_GetDistanceFromPoint2D(VM_VMStackID_FunTag, RE::TESObjectREFR* _ref, float _x, float _y)
	{
		if (!_ref) {
			_vm->TraceStack("Object Reference is none", _stackID);
			return FLT_MAX;
		}
		return GetDistanceFromPoint2D(_ref, _x, _y);
	}

	//GetDistance2D
	inline float Papyrus_GetDistance2D(VM_VMStackID_FunTag, RE::TESObjectREFR* fst_ref, RE::TESObjectREFR* sec_ref)
	{
		if (!fst_ref) {
			_vm->TraceStack("Object Reference is none", _stackID);
			return FLT_MAX;
		}
		if (!sec_ref) {
			_vm->TraceStack("Object Reference is none", _stackID);
			return FLT_MAX;
		}
		return GetDistance2D(fst_ref, sec_ref);
	}

	//GetDistance3D
	inline float Papyrus_GetDistance3D(VM_VMStackID_FunTag, RE::TESObjectREFR* fst_ref, RE::TESObjectREFR* sec_ref)
	{
		if (!fst_ref) {
			_vm->TraceStack("Object Reference is none", _stackID);
			return FLT_MAX;
		}
		if (!sec_ref) {
			_vm->TraceStack("Object Reference is none", _stackID);
			return FLT_MAX;
		}
		return GetDistance3D(fst_ref, sec_ref);
	}

	//GetDistanceFromPoint3D
	inline float Papyrus_GetDistanceFromPoint3D(VM_VMStackID_FunTag, RE::TESObjectREFR* _ref, float _x, float _y, float _z)
	{
		if (!_ref) {
			_vm->TraceStack("Object Reference is none", _stackID);
			return FLT_MAX;
		}

		return GetDistanceFromPoint3D(_ref, _x, _y, _z);
	}

	//GetDistanceBetweenPoints3D
	inline float Papyrus_GetDistanceBetweenPoints3D(RE::StaticFunctionTag *, float fst_x, float fst_y, float fst_z, float sec_x, float sec_y, float sec_z)
	{
		return GetDistanceBetweenPoints3D(fst_x, fst_y, fst_z, sec_x, sec_y, sec_z);
	}

	inline bool Papyrus_GetInSameCellOrWorldSpace(RE::StaticFunctionTag*, RE::TESObjectREFR* A_ref, RE::TESObjectREFR* B_ref)
	{
		return GetInSameCellOrWorldSpace(A_ref, B_ref,true);
	}

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("GetDistanceFromPoint2D"sv, pex_name_function, Papyrus_GetDistanceFromPoint2D);
		_vm->RegisterFunction("GetDistanceBetweenPoints2D"sv, pex_name_function, Papyrus_GetDistanceBetweenPoints2D);
		_vm->RegisterFunction("GetDistance2D"sv, pex_name_function, Papyrus_GetDistance2D);

		_vm->RegisterFunction("GetDistanceBetweenPoints3D"sv, pex_name_function, Papyrus_GetDistanceBetweenPoints3D);
		_vm->RegisterFunction("GetDistanceFromPoint3D"sv, pex_name_function, Papyrus_GetDistanceFromPoint3D);
		_vm->RegisterFunction("GetDistance3D"sv, pex_name_function, Papyrus_GetDistance3D);

		_vm->RegisterFunction("GetInSameCellOrWorldSpace"sv, pex_name_function, Papyrus_GetInSameCellOrWorldSpace);
		return true;
	}
}
