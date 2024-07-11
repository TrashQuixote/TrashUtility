#pragma once
#include "SerializationUtility.h"
#include "TrashCollectionManager.h"
#define Serialization_Debug 0
#define _UNIQUE_LOCK std::unique_lock lock(TrashManager::GetSingleton()->Serializationlocker);
#define CLCTManager TrashManager::GetSingleton()

using namespace _TrashManager;
using namespace TrashSerialization::Utility;
using namespace TrashCollection;
using namespace TrashCollection::Vector;
using namespace TrashCollection::RefMap;
using namespace TrashSerialization;


namespace TrashSerialization::Coll
{
	using namespace TrashSerialization;
	void GetAuxArrays(std::multimap<Holder, TrashCollectionPtr>& _AuxArrs)
	{
		auto _ClctMap = TrashManager::GetSingleton()->GetCollectionMap();
		for (auto iter = _ClctMap->begin(); iter != _ClctMap->end(); iter++) { 
			if (iter->first) {
				if (iter->second->GetCollectionType() == IsVec && iter->second->IsBakedToSave() ) {
					_AuxArrs.insert(std::make_pair(iter->first, iter->second));
				}
			} else logs::critical("Failed to save collection due to holder is none");
		}
	}


	void GetRefMaps(std::multimap<Holder, TrashCollectionPtr>& _RefMaps)
	{
		auto _ClctMap = TrashManager::GetSingleton()->GetCollectionMap();
		for (auto iter = _ClctMap->begin(); iter != _ClctMap->end(); iter++) {
			if (iter->first){
				if (iter->second->GetCollectionType() == IsRefMap && iter->second->IsBakedToSave() ) {
					_RefMaps.insert(std::make_pair(iter->first, iter->second));
				}
			}
			else logs::critical("Failed to save collection due to holder is none");
		}
	}

	void GetAuxArraysAndRefMaps(std::multimap<Holder, TrashCollectionPtr>& _AuxArrs, std::multimap<Holder, TrashCollectionPtr>& _RefMaps)
	{
		auto _ClctMap = TrashManager::GetSingleton()->GetCollectionMap();
		for (auto iter = _ClctMap->begin(); iter != _ClctMap->end(); iter++) { 
			switch (iter->second->GetCollectionType()) {
			case IsVec:
				_AuxArrs.insert(std::make_pair(iter->first, iter->second));
				break;
			case IsRefMap:
				_RefMaps.insert(std::make_pair(iter->first, iter->second));
				break;
			default:
				logs::critical("Failed to save a uninit collection");
				break;
			}
		}
	}

	union SavedVals
	{
		float flt;
		ref_id rID;
	};

	struct SavedElem
	{
		SavedVals _val;
		std::uint32_t _ValType;

		SavedElem() : _ValType(None) { _val.rID = 0; }
		SavedElem(float _flt) : _ValType(Float) { _val.flt = _flt; }
		SavedElem(ref_id _rID) : _ValType(Ref_ID) { _val.rID = _rID; }

		void operator=(const float& flt)
		{
			_val.flt = flt;
			_ValType = Float;
		}

		void operator=(const ref_id& rid)
		{
			_val.rID = rid;
			_ValType = Ref_ID;
		}
	};
// SaveCollectionBaseMsg
	inline bool SaveCollectionBaseMsg(SKSE::SerializationInterface* _interface,const Holder holder,const CollectionNamePtr name,const std::size_t collection_size) {
		auto holder_id = holder->GetFormID();
		if (WriteString(_interface, *name) &&
			_interface->WriteRecordData(&holder_id, sizeof(RE::FormID)) &&
			_interface->WriteRecordData(&collection_size, sizeof(std::size_t)))
			return true;

		logs::critical("Failed to save a the base infomation of a collection Where holder formid = {:x},collection name = \"{}\" ",holder_id,*name);
		return false;
	}
// ReadCollectionBaseMsg
	inline bool ReadCollectionBaseMsg(SKSE::SerializationInterface* _interface,ref_id& holder_id,CollectionName& name,std::size_t& collection_size ) {

		if (ReadString(_interface, name) &&
			ReadFormID(_interface, holder_id) &&
			_interface->ReadRecordData(&collection_size, sizeof(std::size_t)))
			return true;

		logs::critical("Failed to load a the base infomation of a collection Where holder formid = {:x},collection name = \"{}\" ", holder_id, name);
		return false;
	}
//SaveCollectionElem
	bool SaveCollectionElem(SKSE::SerializationInterface* _interface, const elem& _elem) { 
		SavedElem saved_elem;
		switch (_elem.index()) {
		case Float:
			{ 
#ifdef Serialization_Debug
			logs::info("Saving float : {}", std::get<float>(_elem));
#endif // Serialization_Debug
				saved_elem = std::get<float>(_elem); 
			}
			break;
		case Ref:
			{
				auto _ref = std::get<ref>(_elem);
				RE::FormID _fid = (_ref ? _ref->GetFormID() : 0);
				saved_elem = _fid;
#ifdef Serialization_Debug
				logs::info("Saving ref : {:x}", _fid);
#endif // Serialization_Debug
			}
			break;
		default:
			logs::error("Failed to save collection elem due to unknow elem type");
			break;
		}
		if (!_interface->WriteRecordData(&saved_elem, sizeof(SavedElem))) {
			logs::error("Failed to save collection elem");
			return false;
		} 
		else return true;
	}

// Save Aux Array at once
	void SaveVec(SKSE::SerializationInterface* _interface)
	{
		if (!_interface->OpenRecord(kAuxArray, kVersion)) {
			logs::critical("Failed to open record for Aux Array!");
		} else {
			std::multimap<Holder, TrashCollectionPtr> _AuxArrs;
			GetAuxArrays(_AuxArrs);
			std::size_t _AuxArrCount = _AuxArrs.size();
#ifdef Serialization_Debug
			logs::info("SaveBaked AuxArr Nums {}",_AuxArrCount);
#endif //Serialization_Debug			
			if (!_interface->WriteRecordData(&_AuxArrCount, sizeof(std::size_t))) {		// save the nums of aux array
				logs::error("Failed to save count of Aux Array");
				return;
			}

			if ( _AuxArrCount > 0) {
				for (const auto& Vec : _AuxArrs) {
#ifdef Serialization_Debug
					logs::info("Saving AuxArr,Holder_id = {:x}, Coll_Name = \"{}\"", Vec.second->GetHolder()->GetFormID(), *(Vec.second->GetName()));
#endif //Serialization_Debug
					if (!SaveCollectionBaseMsg(_interface, (Vec.second->GetHolder()), (Vec.second->GetName()), (Vec.second->GetSize()))) {// save collection base infomation
						logs::error( "Failed to save base information of a ref map! Where holder formid = {:x},collection name =\"{}\" ",
							Vec.second->GetHolder()->GetFormID(), *(Vec.second->GetName()));
					}
					else
					{ 
						if (auto VecPtr = CLCTManager->GetAsVecPtr(Vec.second)) {
							for (auto _VecIter = VecPtr->Vec->begin(); _VecIter != VecPtr->Vec->end(); _VecIter++) {
			
								if (!SaveCollectionElem(_interface, *_VecIter)) {												// save elem
									logs::error( "Failed to save a elem from Aux Array! Where holder formid = {:x},collection name = \"{}\" ",
										Vec.second->GetHolder()->GetFormID(), *(Vec.second->GetName()));
								}
							}
						}
					}
				}
			}
		}
	}

// Save RefMaps at once
	void SaveRefMap(SKSE::SerializationInterface* _interface)
	{
		if (!_interface->OpenRecord(kRefMap, kVersion)) {
			logs::critical("Failed to open record for Aux Array!");
		} else {
			std::multimap<Holder, TrashCollectionPtr> _RefMaps;
			GetRefMaps(_RefMaps);
			std::size_t _RefMapCount = _RefMaps.size();
#ifdef Serialization_Debug
			logs::info("SaveBaked RefMap Nums {}", _RefMapCount);
#endif //Serialization_Debug
			if (!_interface->WriteRecordData(&_RefMapCount, sizeof(std::size_t))) {		// save the nums of refmaps
				logs::error("Failed to save count of Ref Map");
				return;
			}

			if (_RefMapCount > 0) {
				for (const auto& Map : _RefMaps) {
#ifdef Serialization_Debug
					logs::info("Saving RefMap,Holder_id = {:x}, Coll_Name = \"{}\"", Map.second->GetHolder()->GetFormID(), *(Map.second->GetName()));
#endif //Serialization_Debug
					if( !SaveCollectionBaseMsg(_interface, (Map.second->GetHolder()), (Map.second->GetName()), (Map.second->GetSize())) ) { // save the base information of a refmap
						logs::error( "Failed to save base information of a ref map! Where holder formid = {:x},collection name =\"{}\" ",
							Map.second->GetHolder()->GetFormID(), *(Map.second->GetName()));
					}
					else
					{  
						if (auto MapPtr = CLCTManager->GetAsRefMapPtr(Map.second)) {
							MapPtr->EraseInvalidKeys();									// erase invalid keys 

							for (auto _MapIter = MapPtr->RefMap->begin(); _MapIter != MapPtr->RefMap->end(); _MapIter++) {
								if (!SaveFormID(_interface, _MapIter->first.a_ref)) {  // save key
									logs::error( "Failed to save a key from ref map! Where holder formid = {:x},collection name = \"{}\" ",
										Map.second->GetHolder()->GetFormID(), *(Map.second->GetName()));
								}
								if (!SaveCollectionElem(_interface, _MapIter->second)) {	// save elem
									logs::error( "Failed to save a elem from ref map! Where holder formid = {:x},collection name = \"{}\" ",
										Map.second->GetHolder()->GetFormID(), *(Map.second->GetName()));
								}
							}
						}
					}
				}
			}
		}
	}

// Load a aux array
	void LoadVec(SKSE::SerializationInterface* _interface) {
		ref_id _holder_new_id; 
		CollectionName _coll_name; 
		std::size_t _size;
		if (!ReadCollectionBaseMsg(_interface, _holder_new_id, _coll_name, _size)) {
			logs::critical("Failed to load aux array base information");
			return;
		}
#ifdef Serialization_Debug
		logs::info("Loading AuxArr,Holder_id = {:x}, Coll_Name = \"{}\"", _holder_new_id, _coll_name.data());
#endif //Serialization_Debug

		const Holder _holder = RE::TESForm::LookupByID(_holder_new_id);

		for (std::size_t i = 0; i < _size; i++) {
			SavedElem _saved;
			if (!_interface->ReadRecordData(&_saved, sizeof(SavedElem))) {
				logs::error("Failed to load aux array elem");
				break;
			}

			if (_saved._ValType == Float) 
			{
#ifdef Serialization_Debug
				logs::info("Load a float elem = {}", _saved._val.flt);
#endif //Serialization_Debug

				TrashManager::GetSingleton()->VecSetFloat(_holder, _coll_name.data(), _saved._val.flt, -1);
			}
			else if (_saved._ValType == Ref)
			{
				if (!_interface->ResolveFormID(_saved._val.rID, _saved._val.rID)) {// resolve new formid
					logs::error("Failed to load aux array elem");
					TrashManager::GetSingleton()->VecSetRef(_holder, _coll_name.data(), nullptr, -1);
				} 
#ifdef Serialization_Debug
				logs::info("Load a ref elem, id = {:x}", _saved._val.rID);
#endif //Serialization_Debug
				ref _saved_ref = _saved._val.rID == 0 ? nullptr : RE::TESForm::LookupByID(_saved._val.rID);
				TrashManager::GetSingleton()->VecSetRef(_holder, _coll_name.data(), _saved_ref, -1);
			}
		}
	}
// Load a ref map
	void LoadMap(SKSE::SerializationInterface* _interface) {
		ref_id _holder_new_id; 
		CollectionName _coll_name; 
		std::size_t _size;
		if (!ReadCollectionBaseMsg(_interface, _holder_new_id, _coll_name, _size)) {
			logs::critical("Failed to load aux array base information");
			return;
		}
#ifdef Serialization_Debug
		logs::info("Loading RefMap,Holder_id = {:x}, Coll_Name = \"{}\"", _holder_new_id, _coll_name);
#endif //Serialization_Debug
		const Holder _holder = RE::TESForm::LookupByID(_holder_new_id);

		for (std::size_t i = 0; i < _size; i++) {
			ref_id new_key_id;
			ref _key;
			SavedElem _saved;
			if (!ReadFormID(_interface, new_key_id)) {
				logs::error("Failed to load ref map key id");
				break;
			}
			if (!_interface->ReadRecordData(&_saved, sizeof(SavedElem))) {
				logs::error("Failed to load a elem of ref map ");
				break;
			}

			_key =( new_key_id==0 ? nullptr : RE::TESForm::LookupByID(new_key_id) );

#ifdef Serialization_Debug
			logs::info("Loading RefMap Key_id = {:x}", new_key_id);
#endif //Serialization_Debug

			if (!_key) logs::error("Failed to load ref map key"); 
			else 
			{
				if (_saved._ValType == Float) 
				{
					TrashManager::GetSingleton()->MapSetFloat(_holder, _coll_name, _key, _saved._val.flt); 
#ifdef Serialization_Debug
					logs::info("Load a float Elem = {}", _saved._val.flt);
#endif //Serialization_Debug
				}
				else if (_saved._ValType == Ref) 
				{
					if (!_interface->ResolveFormID(_saved._val.rID, _saved._val.rID)) {  // resolve new formid
						logs::error("Failed to resolve id for a ref elem in ref map");
						TrashManager::GetSingleton()->MapSetRef(_holder, _coll_name, _key, nullptr);
					}
#ifdef Serialization_Debug
					logs::info("Load a ref Elem_id = {:x}", _saved._val.rID);
#endif //Serialization_Debug
					ref _saved_ref = (_saved._val.rID == 0 ? nullptr : RE::TESForm::LookupByID(_saved._val.rID));
					TrashManager::GetSingleton()->MapSetRef(_holder, _coll_name, _key,_saved_ref);
				}
			}
		}
	}

	static void SaveCollectionCallBack(SKSE::SerializationInterface* _interface) {
		_UNIQUE_LOCK
		logs::info("Save Collection");
		TrashManager::GetSingleton()->RemoveInvaildHolderFromMap(false);
		SaveVec(_interface);
		SaveRefMap(_interface);
		
		
	}

	static void LoadCollectionCallBack(SKSE::SerializationInterface* _interface)
	{
		_UNIQUE_LOCK
		TrashManager::GetSingleton()->RemoveSaveBakedCollectionFromMap();
		logs::info("Load Collection");
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;

		while (_interface->GetNextRecordInfo(type, version, length)) {
			if (version != kVersion) {
				logs::critical("Load Data Error: Record data version mismatch! Data version: {}, Needed version: {}", (static_cast<int>(version)), (static_cast<int>(kVersion)));
				continue;
			}

			switch (type) {
			case kAuxArray: 
				std::size_t _AuxArrCnt;
				if (!_interface->ReadRecordData(&_AuxArrCnt, sizeof(std::size_t))) {
					logs::critical("Failed to read the count of aux array from save");
					break;
				}
#ifdef Serialization_Debug
				logs::info("Loading AuxArr,total count of AuxArr is = {}", _AuxArrCnt);
#endif //Serialization_Debug
				for (auto idx = 0; idx < _AuxArrCnt; idx++) LoadVec(_interface);
				break;
			case kRefMap:
				std::size_t _RefMapCnt;
				if (!_interface->ReadRecordData(&_RefMapCnt, sizeof(std::size_t))) {
					logs::critical("Failed to read the size of ref map from save");
					break;
				}
#ifdef Serialization_Debug
				logs::info("Loading RefMap,total count of RefMap is = {}", _RefMapCnt);
#endif //Serialization_Debug
				for (auto idx = 0; idx < _RefMapCnt; idx++) LoadMap(_interface);
				break;
			default:
				break;
			}

		}
	}
}

namespace TrashSerialization
{

	
	void InitSerializationForCollection(const SKSE::SerializationInterface* _interface)
	{
		logs::info("Initializing cosave serialization...");

		_interface->SetSaveCallback(TrashSerialization::Coll::SaveCollectionCallBack);
		_interface->SetLoadCallback(TrashSerialization::Coll::LoadCollectionCallBack);
		logs::info("Cosave serialization initialized.");
	}

}
