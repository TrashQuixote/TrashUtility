#pragma once
#include "TrashFunction.h"
#include "MiscDistance.h"
using namespace Trash_Function::Distance;
using namespace Trash_Function::Misc;

namespace Trash_Function::Pos
{
	inline std::vector<float> GetPosAsArray(RE::TESObjectREFR* _ref) { return { _ref->data.location.x, _ref->data.location.y, _ref->data.location.z }; }

	inline void SetPosAlt(RE::TESObjectREFR* _ref, float _x, float _y, float _z, bool _ApplyOffset) { 
		if (_ApplyOffset) _ref->SetPosition(_ref->data.location.x + _x, _ref->data.location.y + _y, _ref->data.location.z + _z);
		else _ref->SetPosition(_x, _y,_z);
	}

	inline void SetPosFromArray(RE::TESObjectREFR* _ref, const std::vector<float> _pos, bool _ApplyTransform) {
		if (_ApplyTransform) _ref->SetPosition(_ref->data.location.x + _pos[0], _ref->data.location.y + _pos[1], _ref->data.location.z + _pos[2]);
		else _ref->SetPosition(_pos[0], _pos[1],_pos[2]);
	}
	
	inline std::vector<float> Papyrus_GetPosAsArray(VM_VMStackID_FunTag, RE::TESObjectREFR* _ref) {
		if (!_ref) {
			_vm->TraceStack("object reference is none",_stackID);
			return {};
		}
		return GetPosAsArray(_ref);
	}

	inline void Papyrus_SetPosAlt(VM_VMStackID_FunTag, RE::TESObjectREFR* _ref,float _x, float _y, float _z, bool _ApplyTransform = false) {
		if (!_ref) {
			_vm->TraceStack("object reference is none",_stackID);
			return;
		}
		SetPosAlt(_ref, _x, _y, _z, _ApplyTransform);
	}

	inline void Papyrus_SetPosFromArray(VM_VMStackID_FunTag, RE::TESObjectREFR* _ref,const std::vector<float> _pos, bool _ApplyTransform = false) {
		if (!_ref) {
			_vm->TraceStack("object reference is none",_stackID);
			return;
		}
		SetPosFromArray(_ref, _pos, _ApplyTransform);
	}

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("GetPosAsArray"sv, pex_name_function, Papyrus_GetPosAsArray);
		_vm->RegisterFunction("SetPosAlt"sv, pex_name_function, Papyrus_SetPosAlt);
		_vm->RegisterFunction("SetPosFromArray"sv, pex_name_function, Papyrus_SetPosFromArray);
		return true;
	}
}


namespace Trash_Function::HeadingAngle
{
	inline float GetHeadingPointAngle(RE::TESObjectREFR* _ref, float _x, float _y, bool a_abs)
	{
		float theta = RE::NiFastATan2(_x - _ref->GetPositionX(), _y - _ref->GetPositionY());
		float heading = RE::rad_to_deg(theta - _ref->GetAngleZ());

		if (heading < -180.0f) {
			heading += 360.0f;
		}

		if (heading > 180.0f) {
			heading -= 360.0f;
		}

		return a_abs ? RE::NiAbs(heading) : heading;
	}

	inline float GetHeadingAngleBetweenPoints(float pos_x, float pos_y, float ang_z, float tar_pos_x, float tar_pos_y, bool a_abs)
	{
		ang_z = std::fmodf(ang_z,360);

		float theta = RE::NiFastATan2(tar_pos_x - pos_x, tar_pos_y - pos_y);
		float heading = RE::rad_to_deg(theta);
		heading -= ang_z;
		if (heading < -180.0f) {
			heading += 360.0f;
		}

		if (heading > 180.0f) {
			heading -= 360.0f;
		}

		return a_abs ? RE::NiAbs(heading) : heading;
	}

// 2 ref
	inline float GetHeadingAngleX(RE::TESObjectREFR* fst_ref, RE::TESObjectREFR* sec_ref, bool a_abs)
	{
		float BoundOffset_fst = 0 + fst_ref->GetHeight();
		float BoundOffset_sec = 0 + fst_ref->GetHeight() * 0.5f;
		
		int direction = sgn(fst_ref->GetPositionZ() - sec_ref->GetPositionZ());  // negative is upper, positive is lower
		float height_def = std::fabs(fst_ref->GetPositionZ() + BoundOffset_fst - (sec_ref->GetPositionZ() + BoundOffset_sec) );

		float theta = RE::NiFastATan2(height_def, GetDistance2D(fst_ref, sec_ref));
		float Deg = std::fabs(RE::rad_to_deg(theta));
		float Ang_X = RE::rad_to_deg(fst_ref->data.angle.x);

		float heading = Deg * direction - Ang_X;
		return a_abs ? RE::NiAbs(heading) : heading;
	}

// 1 ref and pos x y z
	inline float GetHeadingPointAngleX(RE::TESObjectREFR* _ref, float _x, float _y, float _z, bool a_abs)
	{
		float BoundOffset = _ref->GetHeight();

		int direction = sgn(_ref->GetPositionZ() - _z);	// negative is upper, positive is lower
		float height_def = std::fabs(_ref->GetPositionZ() + BoundOffset - _z);

		float theta = RE::NiFastATan2( height_def,GetDistanceFromPoint2D(_ref, _x, _y));
		float Deg = std::fabs(RE::rad_to_deg(theta));
		float Ang_X = RE::rad_to_deg(_ref->data.angle.x);
		
		float heading = Deg * direction - Ang_X;

		return a_abs ? RE::NiAbs(heading) : heading;
	}

// pos x y z and pos x y z
	inline float GetHeadingAngleBetweenPointsX(float pos_x, float pos_y, float pos_z, float ang_x, float tar_pos_x, float tar_pos_y,float tar_pos_z, bool a_abs)
	{
		ang_x = std::fmodf(ang_x,90);

		int direction = sgn(pos_x - tar_pos_x);  // negative is upper, positive is lower
		float height_def = std::fabs(pos_z - tar_pos_z);

		float theta = RE::NiFastATan2(height_def,GetDistanceBetweenPoints2D(pos_x, pos_y, tar_pos_x, tar_pos_y));
		float Deg = std::fabs(RE::rad_to_deg(theta));

		float heading = Deg * direction - ang_x;

		return a_abs ? RE::NiAbs(heading) : heading;
	}
//GetHeadingPointAngle
	inline float Papyrus_GetHeadingPointAngle(VM_VMStackID_FunTag,RE::TESObjectREFR* _ref, float _x, float _y, bool a_abs = false) {
		if (!_ref) {
			_vm->TraceStack("object reference is none", _stackID);
			return -1;
		}

		return GetHeadingPointAngle(_ref, _x, _y, a_abs);
	}
//GetHeadingAngleBetweenPoints	
	inline float Papyrus_GetHeadingAngleBetweenPoints(RE::StaticFunctionTag *,float pos_x, float pos_y, float ang_z, float tar_pos_x, float tar_pos_y, bool a_abs = false) {
		return GetHeadingAngleBetweenPoints(pos_x, pos_y, ang_z, tar_pos_x, tar_pos_y, a_abs);
	}
//GetHeadingAngleX
	inline float Papyrus_GetHeadingAngleX(VM_VMStackID_FunTag,RE::TESObjectREFR* fst_ref, RE::TESObjectREFR* sec_ref,  bool a_abs = false) {
		if (!fst_ref) {
			_vm->TraceStack("1st object reference is none", _stackID);
			return -1;
		}
		if (!sec_ref) {
			_vm->TraceStack("2st object reference is none", _stackID);
			return -1;
		}
		return GetHeadingAngleX(fst_ref, sec_ref, a_abs);
	}
//GetHeadingPointAngleX
	inline float Papyrus_GetHeadingPointAngleX(VM_VMStackID_FunTag, RE::TESObjectREFR* fst_ref, float _x, float _y, float _z, bool a_abs = false)
	{
		if (!fst_ref) {
			_vm->TraceStack("1st object reference is none", _stackID);
			return -1;
		}
		return GetHeadingPointAngleX(fst_ref, _x, _y, _z, a_abs);
	}
//GetHeadingAngleBetweenPointsX
	inline float Papyrus_GetHeadingAngleBetweenPointsX(RE::StaticFunctionTag *,float pos_x, float pos_y, float pos_z, float ang_x, float tar_pos_x, float tar_pos_y,float tar_pos_z, bool a_abs = false) {
		return GetHeadingAngleBetweenPointsX(pos_x, pos_y, pos_z, ang_x, tar_pos_x, tar_pos_y, tar_pos_z, a_abs);
	}

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("GetHeadingPointAngle"sv, pex_name_function, Papyrus_GetHeadingPointAngle);
		_vm->RegisterFunction("GetHeadingAngleBetweenPoints"sv, pex_name_function, Papyrus_GetHeadingAngleBetweenPoints);

		_vm->RegisterFunction("GetHeadingAngleX"sv, pex_name_function, Papyrus_GetHeadingAngleX);
		_vm->RegisterFunction("GetHeadingPointAngleX"sv, pex_name_function, Papyrus_GetHeadingPointAngleX);
		_vm->RegisterFunction("GetHeadingAngleBetweenPointsX"sv, pex_name_function, Papyrus_GetHeadingAngleBetweenPointsX);
		return true;
	}

}
