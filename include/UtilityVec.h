#pragma once
#include "Utility.h"

using namespace _TrashManager;
#define TRASHMANAGER TrashManager::GetSingleton()
constexpr int DefaultIndex = 0;
using namespace TrashUtility::ErrorPrinter;
namespace TrashUtility::UtilityVec
{
	using VM = RE::BSScript::Internal::VirtualMachine;

	inline bool isVecEmpty(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		int ret = (int)TRASHMANAGER->isVecEmpty(holder, name);
		if (ret < 0) { logs::critical("Failed to execute the function: AuxArrayEmpty ");return false;}
		return ret;
	}
	
	inline bool VecErase(VM_VMStackID_FunTag, const Holder holder, const CollectionName name,int index = -1)
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		bool ret = TRASHMANAGER->VecErase(holder, name, index);
		if (!ret) logs::critical("Failed to execute the function: AuxArrayErase ");
		return ret;
	}
	
	inline int VecValType(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, int index = 0)
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter, holder, name)) return false;
		auto ret = TRASHMANAGER->VecGetValType(holder, name, index);
		if (ret == None) logs::critical("Failed to execute the function: AuxArrayGetType ");
		return (int)ret;
	}

	inline int VecGetSize(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		int ret = (int)TRASHMANAGER->VecGetSize(holder, name);
		if (ret < 0) logs::critical("Failed to execute the function: AuxArrayGetSize ");
		return ret;
	}

	// index < 0 - pushback or set float in specific index
	// will create new collection when the coleection not exist 
	inline bool VecSetFloat(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, float arg_Float, int index = DefaultIndex)
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name, arg_Float)) return false;
		bool ret = TRASHMANAGER->VecSetFloat(holder, name, arg_Float, index);
		if (!ret) logs::critical("Failed to execute the function: AuxArraySetFloat ");
		return ret;
	}


	// index < 0 - get back elem or get float in specific index
	inline float VecGetFloat(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, int index = DefaultIndex)
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter, holder, name)) return 0.0f;
		auto ret = TRASHMANAGER->VecGetFloat(holder, name, index);
		if (isnan(ret)){ logs::critical("Failed to execute the function: AuxArrayGetFloat "); return 0.0f; }
		return ret;
	}
	
	// index < 0 - get back elem or get ref in specific index
	inline RE::TESForm* VecGetRef(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, int index = DefaultIndex)
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return nullptr;
		auto ret = TRASHMANAGER->VecGetRef(holder, name, index);
		if (!ret) logs::critical("Failed to execute the function: AuxArrayGetRef ");
		return ret;
	}

	// index < 0 - pushback or set float in specific index
	// will create new collection when the coleection not exist 
	inline bool VecSetRef(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, RE::TESForm* arg_Ref, int index = DefaultIndex)
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name, arg_Ref)) return false;
		bool ret = TRASHMANAGER->VecSetRef(holder, name, arg_Ref, index);
		if (!ret) logs::critical("Failed to execute the function: AuxArraySetRef ");
		return ret;
	}


	inline std::vector<RE::TESForm*> VecGetRefArray(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return {};
		return TRASHMANAGER->VecGetArray_Ref(holder, name);
	}
	
	inline int VecSetFromRefArray(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, const std::vector<RE::TESForm*> src) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		int ret = (int)TRASHMANAGER->VecSetFromArray_Ref(holder, name, src);

		if (ret < 0) logs::critical("Failed to execute the function: AuxArraySetFromRefArray");
		return ret;
	}


	inline std::vector<float> VecGetFloatArray(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return {};
		return TRASHMANAGER->VecGetArray_Float(holder, name);
	}

	inline int VecSetFromFloatArray(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, std::vector<float> src) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		int ret = (int)TRASHMANAGER->VecSetFromArray_Float(holder, name, src);

		if (ret < 0) logs::critical("Failed to execute the function: AuxArraySetFromFloatArray ");
		return ret;
	}
	
	inline int VecSetFromFormList(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, const RE::BGSListForm* formlist)
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter, holder, name)) return false;
		if (!formlist) {
			_vm->TraceStack("formlist is none", _stackID);
			return false;
		}
		int ret = (int)TRASHMANAGER->VecSetFromFormList(holder, name, formlist);

		if (ret < 0) logs::critical("Failed to execute the function: AuxArraySetFromFormList ");
		return ret;
	}


	inline bool binding(VM* _vm) 
	{
		_vm->RegisterFunction("AuxArrayEmpty"sv, pex_name_collection, isVecEmpty);
		_vm->RegisterFunction("AuxArrayErase"sv, pex_name_collection, VecErase);
		_vm->RegisterFunction("AuxArrayGetType"sv, pex_name_collection, VecValType);
		_vm->RegisterFunction("AuxArrayGetSize"sv, pex_name_collection, VecGetSize);
		_vm->RegisterFunction("AuxArraySetFloat"sv, pex_name_collection, VecSetFloat);
		_vm->RegisterFunction("AuxArrayGetFloat"sv, pex_name_collection, VecGetFloat);
		_vm->RegisterFunction("AuxArrayGetRef"sv, pex_name_collection, VecGetRef);
		_vm->RegisterFunction("AuxArraySetRef"sv, pex_name_collection, VecSetRef);
		_vm->RegisterFunction("AuxArrayGetFloatArray"sv, pex_name_collection, VecGetFloatArray);
		_vm->RegisterFunction("AuxArrayGetRefArray"sv, pex_name_collection, VecGetRefArray);
		_vm->RegisterFunction("AuxArraySetFromRefArray"sv, pex_name_collection, VecSetFromRefArray);
		_vm->RegisterFunction("AuxArraySetFromFloatArray"sv, pex_name_collection, VecSetFromFloatArray);
		_vm->RegisterFunction("AuxArraySetFromFormList"sv, pex_name_collection, VecSetFromFormList);
		return true;
	}
}
