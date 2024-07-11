#pragma once
#include <libloaderapi.h>
#define VM_VMStackID_FunTag VM *_vm, VMStackID _stackID, RE::StaticFunctionTag *

#define VM_VMStackID_ForCheck VM *_vm, VMStackID _stackID
#define VM_VMStackID_Parameter _vm, _stackID
#define MISC Trash_Function::Misc

constexpr auto pex_name_function = "Trash_Function"sv;
using VM = RE::BSScript::Internal::VirtualMachine;
using VMStackID = RE::VMStackID;

namespace Trash_Function {}

namespace clib_util::editorID
{
	using _GetFormEditorID = const char* (*)(std::uint32_t);

	inline std::string get_editorID(const RE::TESForm* a_form)
	{
		switch (a_form->GetFormType()) {
		case RE::FormType::Keyword:
		case RE::FormType::LocationRefType:
		case RE::FormType::Action:
		case RE::FormType::MenuIcon:
		case RE::FormType::Global:
		case RE::FormType::HeadPart:
		case RE::FormType::Race:
		case RE::FormType::Sound:
		case RE::FormType::Script:
		case RE::FormType::Navigation:
		case RE::FormType::Cell:
		case RE::FormType::WorldSpace:
		case RE::FormType::Land:
		case RE::FormType::NavMesh:
		case RE::FormType::Dialogue:
		case RE::FormType::Quest:
		case RE::FormType::Idle:
		case RE::FormType::AnimatedObject:
		case RE::FormType::ImageAdapter:
		case RE::FormType::VoiceType:
		case RE::FormType::Ragdoll:
		case RE::FormType::DefaultObject:
		case RE::FormType::MusicType:
		case RE::FormType::StoryManagerBranchNode:
		case RE::FormType::StoryManagerQuestNode:
		case RE::FormType::StoryManagerEventNode:
		case RE::FormType::SoundRecord:
			return a_form->GetFormEditorID();
		default:
		{
			static auto tweaks = GetModuleHandle("po3_Tweaks");
			static auto func = reinterpret_cast<_GetFormEditorID>(GetProcAddress(tweaks, "GetFormEditorID"));
			if (func) {
				return func(a_form->formID);
			}
			return {};
		}
		}
	}
}

namespace Trash_Function::Misc
{
	template <typename T>
	static int sgn(T val)
	{
		return static_cast<int>((T(0) < val) - (val < T(0)));
	}

	inline std::string GetHexString(const std::uint32_t& formid ) {
		char temp[16];
		sprintf_s(temp,"%X",formid);
		return std::string(temp);
	}

	inline void PrintNiPoint3(const RE::NiPoint3& point) {
		RE::ConsoleLog::GetSingleton()->Print("Point x:%.2f, y:%.2f, z:%.2f.",point.x,point.y,point.z);
	}

	inline void ToLowercaseStr(std::string& _str) {
		for (char& ch : _str) 
			ch = static_cast<char>(std::tolower(ch));
	}

	inline std::string GetLowercaseStr(const std::string& _str)
	{
		std::string ret = _str;
		for (char& ch : ret) ch = static_cast<char>(std::tolower(ch));
		return ret;
	}

	inline bool IsXDigitsStr(const std::string& _str) {
		for (const char& ch : _str) {
			if (!std::isxdigit(ch)) return false;
		}
		return true;
	}

	inline std::string GetEditorID(const RE::TESForm* _form)
	{
		return clib_util::editorID::get_editorID(_form);
	}

	inline void PrintFormVec(const std::vector<RE::TESForm*>& _vec)
	{
		if (_vec.empty()) {
			RE::ConsoleLog::GetSingleton()->Print("The form array is empty.");
			return;
		}

		int idx = 0;
		for (RE::TESForm* elem : _vec) {
			if (!elem) continue;
			auto elem_ref = elem->AsReference();
			RE::ConsoleLog::GetSingleton()->Print("Index - %d : EditorID: %s, FormID: %x, FormName: %s",idx, (elem_ref ? elem_ref->GetFormEditorID() : GetEditorID(elem).data()),
				elem->GetFormID(), (elem_ref ? elem_ref->GetDisplayFullName() : elem->GetName()));
			idx += 1;
		}
	}

	inline void PrintFormVec(const std::vector<RE::TESObjectREFR*>& _vec)
	{
		if (_vec.empty()) {
			RE::ConsoleLog::GetSingleton()->Print("The ref array is empty.");
			return;
		}

		int idx = 0;
		for (RE::TESObjectREFR* elem : _vec) {
			if (!elem) continue;
			RE::ConsoleLog::GetSingleton()->Print("Index - %d : EditorID: %s, FormID: %x, FormName: %s", idx,
				elem->GetFormEditorID(), elem->GetFormID(), elem->GetDisplayFullName());
			idx += 1;
		}
	}

	inline void PrintString(const std::string& _str)
	{
		RE::ConsoleLog::GetSingleton()->Print(_str.data());
	}

	inline void PrintFltVec(const std::vector<float>& _vec)
	{
		if (_vec.empty()) {
			RE::ConsoleLog::GetSingleton()->Print("The float array is empty.");
			return;
		}

		int idx = 0;
		for (const float& elem : _vec) {
			RE::ConsoleLog::GetSingleton()->Print("Index - %d : %.7f", idx,elem);
			idx += 1;
		}
	}

	inline void PrintIntVec(const std::vector<int>& _vec)
	{
		if (_vec.empty()) {
			RE::ConsoleLog::GetSingleton()->Print("The int array is empty.");
			return;
		}

		int idx = 0;
		for (const int& elem : _vec) {
			RE::ConsoleLog::GetSingleton()->Print("Index - %d : %d", idx, elem);
			idx += 1;
		}
	}

	inline void PrintForm(RE::TESForm* _form)
	{
		if (_form) {
			auto _ref = _form->AsReference();
			RE::ConsoleLog::GetSingleton()->Print("PrintForm: EditorID: %s, FormID: %x, FormName: %s", (_ref ? _ref->GetFormEditorID() : GetEditorID(_form).data()),
				_form->GetFormID(), (_ref? _ref->GetDisplayFullName() : _form->GetName()) );
			return;
		}
		RE::ConsoleLog::GetSingleton()->Print("Form is none");
	}

	inline void PrintRef(RE::TESObjectREFR* _ref)
	{
		if (_ref) {
			RE::ConsoleLog::GetSingleton()->Print("PrintRef: EditorID: %s, FormID: %x, FormName: %s", _ref->GetFormEditorID(),
				_ref->GetFormID(), _ref->GetDisplayFullName());
			return;
		}
		RE::ConsoleLog::GetSingleton()->Print("ObjectReference is none");
	}

	inline void PrintFlt(const float& _flt)
	{
		RE::ConsoleLog::GetSingleton()->Print("%.7f",_flt);
	}

	inline void SetMaxWhenOverFlow(float& _num) {
		if (_num == HUGE_VALF) _num = FLT_MAX;
	}

// Only for console prase
	void StringSplit(const std::string& str, const char& split, std::vector<std::string>& res)
	{
		std::istringstream iss(str);
		std::string buffer;
		while (getline(iss, buffer, split)) { res.emplace_back(buffer); }
	}


	inline float GetDefaultFOV(int _1stPersonFOV)
	{
		if (!_1stPersonFOV) {
			REL::Relocation<float*> df_worldFOV{ RELOCATION_ID(512129, 388785) };
			return *df_worldFOV.get();
		}

		REL::Relocation<float*> df_FirstPersonFOV{ RELOCATION_ID(512131, 388788) };
		return *df_FirstPersonFOV.get();
	}


	inline void Papyrus_PrintFlt(RE::StaticFunctionTag*, float flt)
	{
		PrintFlt(flt);
	}

	inline void Papyrus_PrintForm(RE::StaticFunctionTag* ,RE::TESForm* _form)
	{
		PrintForm(_form);
	}

	inline int Papyrus_IntMax(RE::StaticFunctionTag* ) { return INT_MAX; }
	inline float Papyrus_FloatMax(RE::StaticFunctionTag* ) { return FLT_MAX; }
	inline int Papyrus_IntMin(RE::StaticFunctionTag* ) { return INT_MIN; }
	inline float Papyrus_FloatMin(RE::StaticFunctionTag*) { return FLT_MIN; }

	inline void Papyrus_DumpFloatArray(RE::StaticFunctionTag*, std::vector<float> _fltarr) 
	{ 
		PrintFltVec(_fltarr);
	}

	inline void Papyrus_DumpIntArray(RE::StaticFunctionTag*, std::vector<int> _fltarr)
	{
		PrintIntVec(_fltarr);
	}

	inline void Papyrus_DumpFormArray(RE::StaticFunctionTag* , std::vector<RE::TESForm*> _FormArr)
	{
		PrintFormVec(_FormArr);
	}

	inline void Papyrus_DumpRefArray(RE::StaticFunctionTag*, std::vector<RE::TESObjectREFR*> _FormArr)
	{
		PrintFormVec(_FormArr);
	}

	inline void Papyrus_ConsoleInfo(RE::StaticFunctionTag*, std::string _str)
	{
		PrintString(_str);
	}

	inline std::string Papyrus_GetFormIDString(RE::StaticFunctionTag*, RE::TESForm* form)
	{
		if (!form) return "";
		return GetHexString( form->formID );
	}

	inline std::string Papyrus_ToHexString(RE::StaticFunctionTag*, int _num)
	{
		return GetHexString( _num );
	}

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("ConsoleInfo"sv, pex_name_function, Papyrus_ConsoleInfo);
		_vm->RegisterFunction("GetFormIDString"sv, pex_name_function, Papyrus_GetFormIDString);
		_vm->RegisterFunction("ToHexString"sv, pex_name_function, Papyrus_ToHexString);
		_vm->RegisterFunction("PrintForm"sv, pex_name_function, Papyrus_PrintForm);
		_vm->RegisterFunction("PrintFlt"sv, pex_name_function, Papyrus_PrintFlt);
		_vm->RegisterFunction("GetIntMax"sv, pex_name_function, Papyrus_IntMax);
		_vm->RegisterFunction("GetIntMin"sv, pex_name_function, Papyrus_IntMin);
		_vm->RegisterFunction("GetFloatMax"sv, pex_name_function, Papyrus_FloatMax);
		_vm->RegisterFunction("GetFloatMin"sv, pex_name_function, Papyrus_FloatMin);
		_vm->RegisterFunction("DumpFloatArray"sv, pex_name_function, Papyrus_DumpFloatArray);
		_vm->RegisterFunction("DumpIntArray"sv, pex_name_function, Papyrus_DumpIntArray);
		_vm->RegisterFunction("DumpFormArray"sv, pex_name_function, Papyrus_DumpFormArray);
		_vm->RegisterFunction("DumpRefArray"sv, pex_name_function, Papyrus_DumpRefArray);
		return true;
	}
}

