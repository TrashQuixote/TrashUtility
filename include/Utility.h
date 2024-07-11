#pragma once
#include "TrashCollectionManager.h"

using VM = RE::BSScript::Internal::VirtualMachine;
using VMStackID = RE::VMStackID;
#define VM_VMStackID_FunTag VM *_vm, VMStackID _stackID, RE::StaticFunctionTag *
#define VM_VMStackID_ForCheck VM *_vm, VMStackID _stackID
#define VM_VMStackID_Parameter _vm, _stackID
#define TRASHMANAGER TrashManager::GetSingleton()
constexpr auto pex_name_collection = "Trash_Collection"sv;

namespace TrashUtility {
}

namespace TrashUtility::ErrorPrinter
{
	using namespace _TrashManager;
	
	inline bool Check_Holder_CollectionName_Args(VM_VMStackID_ForCheck, const Holder holder, const CollectionName& name)
	{
		if (!holder) {
			_vm->TraceStack("Holder is none", _stackID);
			return false;
		}
		if (name.size() == 0) {
			_vm->TraceStack("Collection name is none", _stackID);
			return false;
		}
		return true;
	}

	inline bool Check_Holder_CollectionName_Args(VM_VMStackID_ForCheck, const Holder holder, const CollectionName& name, const RE::TESForm* arg_Ref)
	{
		if (!holder) {
			_vm->TraceStack("Holder is none", _stackID);
			return false;
		}
		if (name.size() == 0) {
			_vm->TraceStack("Collection name is none", _stackID);
			return false;
		}
		if (!arg_Ref) {
			_vm->TraceStack("Object Reference is invaild", _stackID);
			return false;
		}
		return true;
	}

	inline bool Check_Holder_CollectionName_Args(VM_VMStackID_ForCheck, const Holder holder, const CollectionName& name, const float& flt)
	{
		if (!holder) {
			_vm->TraceStack("Holder is none", _stackID);
			return false;
		}
		if (name.size() == 0) {
			_vm->TraceStack("Collection name is none", _stackID);
			return false;
		}
		if (isnan(flt)) {
			_vm->TraceStack("float argument is NAN", _stackID);
			return false;
		}
		return true;
	}

	inline bool Check_Holder_CollectionName_Args(VM_VMStackID_ForCheck, const Holder holder, const CollectionName& name, const RE::TESForm* key, const float& flt)
	{
		if (!holder) {
			_vm->TraceStack("Holder is none", _stackID);
			return false;
		}
		if (name.size() == 0) {
			_vm->TraceStack("Collection name is none", _stackID);
			return false;
		}
		if (!key) {
			_vm->TraceStack("key is none", _stackID);
			return false;
		}
		if (isnan(flt)) {
			_vm->TraceStack("float argument is NAN", _stackID);
			return false;
		}
		return true;
	}

	inline bool Check_Holder_CollectionName_Args(VM_VMStackID_ForCheck, const Holder holder, const CollectionName& name, const RE::TESForm* key, const RE::TESForm* ref)
	{
		if (!holder) {
			_vm->TraceStack("Holder is none", _stackID);
			return false;
		}
		if (name.size() == 0) {
			_vm->TraceStack("Collection name is none", _stackID);
			return false;
		}
		if (!key) {
			_vm->TraceStack("key is none", _stackID);
			return false;
		}
		if (!ref) {
			_vm->TraceStack("ref is none", _stackID);
			return false;
		}
		return true;
	}
}

namespace TrashUtility::DumpCollection
{
	using namespace _TrashManager;
	using namespace TrashCollection;
	using namespace TrashCollection::RefMap;
	using namespace TrashCollection::Vector;
	using namespace Trash_Function::Misc;

	
	inline void CollectionNotExistMsg(const Holder holder, const CollectionName& name) {
		RE::ConsoleLog::GetSingleton()->Print("Collection Not Exist- CollectionName: \"%s\",Holder: [EditorID: %s], [FormID: %x], [FormName: %s]", &name, 
			GetEditorID(holder).data(), holder->GetFormID(), holder->GetName());
	}

	inline void DumpCollectionBaseMsg(const Holder holder, const CollectionName& name, bool isMap) {
		const std::string c_type = isMap ? "RefMap" : "AuxArray";
		RE::ConsoleLog::GetSingleton()->Print("Dump %s - CollectionName: \"%s\",Holder: [EditorID: %s], [FormID: %x], [FormName: %s]", &c_type, &name, 
			GetEditorID(holder).data(), holder->GetFormID(), holder->GetName());
	}
	
	inline void PrintElem(const elem& _elem, const int& _idx) {
		if (_elem.index() == Float) {
			RE::ConsoleLog::GetSingleton()->Print("[index: %d] : [Float: %.7f]", _idx,(std::get<float>(_elem)) );
		} 
		else if (_elem.index() == Ref) {
			if (auto _form = std::get<ref>(_elem)) {
				auto _ref = _form->AsReference();
				RE::ConsoleLog::GetSingleton()->Print("[index: %d] : [EditorID: %s], [FormID: %x], [Name: %s]", 
					_idx,(_ref ? _ref->GetFormEditorID() : GetEditorID(_form).data()), _form->GetFormID(), (_ref ? _ref->GetDisplayFullName() : _form->GetName()) );
			}
		}	
	}
	
	inline void PrintMapPair(ref _key,const elem& _elem) {
		auto _key_ref = _key->AsReference();
		auto _key_editorid = _key_ref ? _key_ref->GetFormEditorID() : GetEditorID(_key).data();
		auto _key_name = _key_ref ? _key_ref->GetDisplayFullName() : _key->GetName();
		if (_elem.index() == Float) {
			RE::ConsoleLog::GetSingleton()->Print("Key = { EditorID: %s, FormID: %x, Name: %s } - Value = [ float: %.7f ]",
				_key_editorid, _key->GetFormID(), _key_name,(std::get<float>(_elem)));
		} 
		else if (_elem.index() == Ref) {
			if (auto _form = std::get<ref>(_elem)) {
				auto _ref = _form->AsReference();
				RE::ConsoleLog::GetSingleton()->Print("Key = { EditorID: %s, FormID: %x, Name: %s } - Value = { EditorID: %s, FormID: %x ,Name: %s }", 
					_key_editorid,_key->GetFormID(), _key_name, (_ref ? _ref->GetFormEditorID() : GetEditorID(_form).data() ), _form->GetFormID(), (_ref ? _ref->GetDisplayFullName() : _form->GetName() ));
			}
		}	
	}

	inline void DumpVec(const Holder holder, const CollectionName& name, const TrashVecPtr _Vec) 
	{
		if (!_Vec) {
			CollectionNotExistMsg(holder, name);
			return;
		}

		DumpCollectionBaseMsg(holder, name, false);

		int idx = 0;

		for_each(_Vec->Vec->begin(), _Vec->Vec->end(), [&idx](elem _elem) {
			PrintElem(_elem,idx++);
		});

		RE::ConsoleLog::GetSingleton()->Print(" ");
	}
	
	
	inline void DumpMap(const Holder holder, const CollectionName& name, const TrashRefMapPtr _Map)
	{
		if (!_Map) {
			CollectionNotExistMsg(holder, name);
			return;
		}

		DumpCollectionBaseMsg(holder, name, true);
		
		for (auto iter = _Map->RefMap->begin(); iter != _Map->RefMap->end(); iter++) {
			PrintMapPair(iter->first.a_ref,iter->second);
		}

		RE::ConsoleLog::GetSingleton()->Print(" ");
	}
	

	
	inline void DumpCollection(const Holder holder, const CollectionName& name,const CollectionType& collection_type)
	{
		if (auto Collection_Ptr = TRASHMANAGER->GetCollectionPtr(holder, name, collection_type)) {
			switch (Collection_Ptr->GetCollectionType()) {
			case IsVec:
				DumpVec(holder,name,TRASHMANAGER->GetAsVecPtr(Collection_Ptr));
				return;
			case IsRefMap:
				DumpMap(holder,name,TRASHMANAGER->GetAsRefMapPtr(Collection_Ptr));
				return;
			default:
				RE::ConsoleLog::GetSingleton()->Print("Collection Type Error: Report To Author");
				break;
			}
		}
		CollectionNotExistMsg(holder, name);
	}
	
	inline void DumpAllAuxArr()
	{
		for (auto iter = TRASHMANAGER->GetCollectionMap()->begin(); iter != TRASHMANAGER->GetCollectionMap()->end(); iter++)
		{
			if (!iter->second)
			{
				RE::ConsoleLog::GetSingleton()->Print("Collection Not Exist Error: Collection is none");
				continue;
			}
			if (iter->second->GetCollectionType() != IsVec) continue;

			const Holder _holder = iter->second->GetHolder();
			const std::string* _name = iter->second->GetName();
			const auto Collection_Ptr = TRASHMANAGER->GetCollectionPtr(_holder, *_name, IsVec);
			if (Collection_Ptr) {
				DumpVec(_holder, *_name, TRASHMANAGER->GetAsVecPtr(Collection_Ptr));
			}
		}
	}

	inline void DumpAllRefMap()
	{
		for (auto iter = TRASHMANAGER->GetCollectionMap()->begin(); iter != TRASHMANAGER->GetCollectionMap()->end(); iter++)
		{
			if (!iter->second)
			{
				RE::ConsoleLog::GetSingleton()->Print("Collection Not Exist Error: Collection is none");
				continue;
			}
			if (iter->second->GetCollectionType() != IsRefMap) continue;

			const Holder _holder = iter->second->GetHolder();
			const std::string* _name = iter->second->GetName();
			const auto Collection_Ptr = TRASHMANAGER->GetCollectionPtr(_holder, *_name, IsRefMap);
			if (Collection_Ptr) {
				DumpMap(_holder, *_name, TRASHMANAGER->GetAsRefMapPtr(Collection_Ptr));
			}
		}
	}

	inline void DumpAllCollection()
	{
		for (auto iter = TRASHMANAGER->GetCollectionMap()->begin(); iter != TRASHMANAGER->GetCollectionMap()->end(); iter++)
		{
			if (!iter->second)
			{
				RE::ConsoleLog::GetSingleton()->Print("Collection Not Exist Error: Collection is none");
				continue;
			}
			if (iter->second->GetCollectionType() == UnknownCollection) continue;

			const auto coll_ptr = iter->second;
			const Holder _holder = coll_ptr->GetHolder();
			const std::string* _name = coll_ptr->GetName();
			switch (coll_ptr->GetCollectionType()) {
				case IsVec:
					DumpVec(_holder, *_name, TRASHMANAGER->GetAsVecPtr(coll_ptr));
					break;
				case IsRefMap:
					DumpMap(_holder, *_name, TRASHMANAGER->GetAsRefMapPtr(coll_ptr));
					break;
				default:
					RE::ConsoleLog::GetSingleton()->Print("Collection Type Error: Report To Author");
					break;
			}
		}
	}

	inline bool Papyrus_DestroyAuxArr(RE::StaticFunctionTag* ,const Holder _holder,const CollectionName _collection_name) {
		return TRASHMANAGER->RemoveCollectionFromMap(_holder, _collection_name,IsVec);
	}

	inline bool Papyrus_DestroyRefMap(RE::StaticFunctionTag*, const Holder _holder, const CollectionName _collection_name) {
		return TRASHMANAGER->RemoveCollectionFromMap(_holder, _collection_name, IsRefMap);
	}

	inline void Papyrus_DumpAllAuxArr(RE::StaticFunctionTag* ) {
		DumpAllAuxArr();
	}

	inline void Papyrus_DumpAllRefMap(RE::StaticFunctionTag* ) {
		DumpAllRefMap();
	}

	inline void Papyrus_DumpAllCollection(RE::StaticFunctionTag* ) {
		DumpAllCollection();
	}

	inline void Papyrus_DumpCollection(RE::StaticFunctionTag*, const Holder holder, const CollectionName name, const CollectionType collection_type) {
		DumpCollection(holder, name, collection_type);
	}
	


	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("DumpCollection"sv, pex_name_collection, Papyrus_DumpCollection);
		_vm->RegisterFunction("DumpAllAuxArr"sv, pex_name_collection, Papyrus_DumpAllAuxArr);
		_vm->RegisterFunction("DumpAllRefMap"sv, pex_name_collection, Papyrus_DumpAllRefMap);
		_vm->RegisterFunction("DumpAllCollection"sv, pex_name_collection, Papyrus_DumpAllCollection);
		
		_vm->RegisterFunction("DestroyAuxArr"sv, pex_name_collection, Papyrus_DestroyAuxArr);
		_vm->RegisterFunction("DestroyRefMap"sv, pex_name_collection, Papyrus_DestroyRefMap);


		return true;
	}

}

