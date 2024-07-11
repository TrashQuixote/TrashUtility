#pragma once
#include "Utility.h"
using namespace _TrashManager;
#define TRASHMANAGER TrashManager::GetSingleton()

namespace TrashUtility::UtilityRefMap
{
	using namespace TrashUtility::ErrorPrinter;
	
	inline bool isMapEmpty(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		int ret = (int)TRASHMANAGER->isMapEmpty(holder, name);
		if (ret < 0) {logs::critical("Failed to execute the function: isMapEmpty ");return false;}
		return ret;
	}

	inline bool MapClear(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		bool ret = TRASHMANAGER->MapClear(holder, name);
		if (!ret) logs::critical("Failed to execute the function: MapClear ");
		return ret;
	}

	inline int MapGetSize(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		int ret = (int)TRASHMANAGER->MapGetSize(holder, name);
		if (ret < 0) logs::critical("Failed to execute the function: MapGetSize ");
		return ret;
	}

	// key not exist - insert or set float for specific key
	// will create new collection when the coleection not exist 
	inline bool MapSetFloat(VM_VMStackID_FunTag, const Holder holder, const CollectionName name,RE::TESForm* arg_Key, float arg_Float) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name, arg_Key, arg_Float)) return false;
		bool ret = TRASHMANAGER->MapSetFloat(holder, name, arg_Key, arg_Float);
		if (!ret) logs::critical("Failed to execute the function: MapSetFloat ");
		return ret;
	}

	// get float from map with key
	inline float MapGetFloat(VM_VMStackID_FunTag, const Holder holder, const CollectionName name,RE::TESForm* arg_Key) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name, arg_Key)) return false;
		auto flt = TRASHMANAGER->MapGetFloatWithKey(holder, name, arg_Key);
		if (isnan(flt)) logs::critical("Failed to execute the function: MapSetRef ");
		return flt;
	}

	// get key from float map 
	inline std::vector<RE::TESForm*> MapGetKeys(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return {};
		auto vec = TRASHMANAGER->RefMapGetKeys(holder, name);
		if (vec.size()<=0) logs::critical("Failed to execute the function: RefMapGetKeys ");
		return vec;
	}

	// key not exist - insert or set float for specific key
	// will create new collection when the coleection not exist 
	inline bool MapSetRef(VM_VMStackID_FunTag, const Holder holder, const CollectionName name,RE::TESForm* arg_Key, RE::TESForm* arg_Ref) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name, arg_Key, arg_Ref)) return false;
		bool ret = TRASHMANAGER->MapSetRef(holder, name, arg_Key, arg_Ref);
		if (!ret) logs::critical("Failed to execute the function: MapSetRef ");
		return ret;
	}

	// get ref from map with key
	inline RE::TESForm* MapGetRef(VM_VMStackID_FunTag, const Holder holder, const CollectionName name,RE::TESForm* arg_Key) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name, arg_Key)) return nullptr;
		auto ref = TRASHMANAGER->MapGetRefWithKey(holder, name, arg_Key);
		if (!ref) logs::critical("Failed to execute the function: MapSetRef ");
		return ref;
	}

	inline std::vector<float> MapGetFloatVals(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return {};
		auto ref = TRASHMANAGER->MapGetVals_Float(holder, name);
		if (ref.size()<=0) logs::critical("Failed to execute the function: MapGetFloatVals ");
		return ref;
	}

	inline std::vector<RE::TESForm*> MapGetRefVals(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return {};
		auto ref = TRASHMANAGER->MapGetVals_Ref(holder, name);
		if (ref.size()<=0) logs::critical("Failed to execute the function: MapGetRefVals ");
		return ref;
	}

	inline bool MapErase(VM_VMStackID_FunTag, const Holder holder, const CollectionName name, RE::TESForm* arg_Key) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		auto ret = TRASHMANAGER->MapErase(holder, name, arg_Key);
		if (!ret) logs::critical("Failed to execute the function: MapErase ");
		return ret;
	}

	inline bool MapEraseAll(VM_VMStackID_FunTag, const Holder holder, const CollectionName name) 
	{
		if (!Check_Holder_CollectionName_Args(VM_VMStackID_Parameter,holder, name)) return false;
		auto ret = TRASHMANAGER->MapEraseAll(holder, name);
		if (!ret) logs::critical("Failed to execute the function: MapEraseAll ");
		return ret;
	}

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("RefMapClear"sv, pex_name_collection, MapClear);
		_vm->RegisterFunction("RefMapGetSize"sv, pex_name_collection, MapGetSize);
		_vm->RegisterFunction("RefMapSetFloat"sv, pex_name_collection, MapSetFloat);
		_vm->RegisterFunction("RefMapGetFloat"sv, pex_name_collection, MapGetFloat);
		_vm->RegisterFunction("RefMapGetKeys"sv, pex_name_collection, MapGetKeys);
		_vm->RegisterFunction("RefMapSetRef"sv, pex_name_collection, MapSetRef);
		_vm->RegisterFunction("RefMapGetRef"sv, pex_name_collection, MapGetRef);
		_vm->RegisterFunction("RefMapGetFloatValues"sv, pex_name_collection, MapGetFloatVals);
		_vm->RegisterFunction("RefMapGetRefValues"sv, pex_name_collection, MapGetRefVals);
		_vm->RegisterFunction("RefMapErase"sv, pex_name_collection, MapErase);
		_vm->RegisterFunction("RefMapEraseAll"sv, pex_name_collection, MapEraseAll);
		return true;
	}
}
