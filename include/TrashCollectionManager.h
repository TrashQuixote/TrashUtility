#pragma once
#include "TrashCollectionCreator.h"
#define Collection this->Collections_Map

using namespace TrashCollection;


namespace _TrashManager
{
	using Holder = RE::TESForm*;
	
	using TrashCollectionPtr = TrashCollection::_TrashCollection*;
	using TrashVecPtr = COLLECTION_VECTOR::TrashVec*;
	using TrashRefMapPtr = COLLECTION_REFMAP::TrashRefMap*;

	using CollectionName = std::string;
	using svCollectionName = std::string_view;
	using CollectionNamePtr = std::string*;
	using CollectionType = std::uint32_t;
	using Creator = TrashCollectionCreator::Creator;
	using Pair = std::pair<Holder, TrashCollectionPtr>;

	struct HolderCmp {
		bool operator()(const Holder a, const Holder b) const {
			return a->GetFormID() < b->GetFormID();
		}
	};
	using Collection_Map = std::multimap< Holder, TrashCollectionPtr, HolderCmp >;
	
	class TrashManager
	{
	private:
		
		
		TrashManager();
		~TrashManager(){};
		Collection_Map* Collections_Map;
	public:	
		
		inline TrashCollectionPtr CreateCollection(const Holder holder, const CollectionName& name, const CollectionType& collect_type) const;
		inline TrashCollectionPtr AddNewCollectionToMap(const Holder holder, const CollectionName& name, const CollectionType& collect_type);
		inline Collection_Map::iterator FindCollectionInMap(const Holder holder, const CollectionName& name, const CollectionType& collect_type) const;
		inline bool RemoveCollectionFromMap(const Holder holder, const CollectionName& name, const CollectionType& collect_type);
		inline size_t RemoveHolderFromMap(const Holder holder);
		inline void RemoveInvaildHolderFromMap(bool _RemoveNo3DLoad); 
		inline void RemoveSaveBakedCollectionFromMap(); 
		
	
		mutable std::mutex Serializationlocker;

		TrashManager(TrashManager& another) = delete;
		TrashManager(const TrashManager& another) = delete;
		void operator=(const TrashManager&) = delete;
		
		static TrashManager* GetSingleton();

		inline Collection_Map* GetCollectionMap() { return Collections_Map; }
		inline TrashCollectionPtr GetCollectionPtr(const Holder holder, const CollectionName& name, const CollectionType& type) const;

		inline TrashVecPtr GetAsVecPtr(const Holder holder, const CollectionName& name) const;
		inline TrashVecPtr GetAsVecPtr(const TrashCollectionPtr _collection) const;
		inline TrashRefMapPtr GetAsRefMapPtr(const Holder holder, const CollectionName& name) const;
		inline TrashRefMapPtr GetAsRefMapPtr(const TrashCollectionPtr _collection) const;

		inline int isVecEmpty(const Holder holder, const CollectionName& name) const;
		inline bool VecErase(const Holder holder, const CollectionName& name, const int& index) const;
		inline std::size_t VecGetValType(const Holder holder, const CollectionName& name, const int& index) const;
		inline int VecGetSize(const Holder holder, const CollectionName& name) const;
		inline bool VecSetFloat(const Holder holder, const CollectionName& name, const float& arg_Float, const int& index = -1);
		inline float VecGetFloat(const Holder holder, const CollectionName& name, const int& index) const;
		inline RE::TESForm* VecGetRef(const Holder holder, const CollectionName& name, int index) const;
		inline bool VecSetRef(const Holder holder, const CollectionName& name, RE::TESForm* arg_Ref, const int& index = -1);
		inline std::vector<RE::TESForm*> VecGetArray_Ref(const Holder holder, const CollectionName& name) const;
		inline int VecSetFromArray_Ref(const Holder holder, const CollectionName& name, const std::vector<RE::TESForm*>& src);
		inline std::vector<float> VecGetArray_Float(const Holder holder, const CollectionName& name) const;
		inline int VecSetFromArray_Float(const Holder holder, const CollectionName& name, const std::vector<float>& src);

		inline int VecSetFromFormList(const Holder holder, const CollectionName& name, const RE::BGSListForm* formlist);

		
		inline int isMapEmpty(const Holder holder, const CollectionName& name) const;
		inline bool MapClear(const Holder holder, const CollectionName& name) const;
		inline int MapGetSize(const Holder holder, const CollectionName& name) const;
		inline bool MapSetFloat(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key, const float& arg_NewFloat);
		inline bool MapSetRef(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key, RE::TESForm* arg_NewRef);
		inline RE::TESForm* MapGetRefWithKey(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key) const;
		inline float MapGetFloatWithKey(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key) const;
		inline std::vector<RE::TESForm*> RefMapGetKeys(const Holder holder, const CollectionName& name) const;
		inline std::vector<float> MapGetVals_Float(const Holder holder, const CollectionName& name) const;
		inline std::vector<RE::TESForm*> MapGetVals_Ref(const Holder holder, const CollectionName& name) const;
		inline bool MapErase(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key) const;
		inline bool MapEraseAll(const Holder holder, const CollectionName& name) const;

		inline bool MapEraseInvalidKey(const Holder holder, const CollectionName& name) const;

		inline std::size_t MapGetValueType(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key) const;
		
};

	
}



namespace _TrashManager
{
	using namespace CollectionErrorPrinter;

	TrashManager::TrashManager() { Collections_Map = new Collection_Map(); }

	TrashManager* TrashManager::GetSingleton()
	{
		static TrashManager* tm_Singleton = new TrashManager();
		return tm_Singleton;
	}

	/*Create Collection*/
	inline TrashCollectionPtr TrashManager::CreateCollection(const Holder holder, const CollectionName& name, const CollectionType& collect_type) const
	{
		switch (collect_type) {
			case IsVec:
				return Creator::CreatVector(holder, name);
			case IsRefMap:
				return Creator::CreatRefMap(holder, name);
			default:
				break;
		}
		return nullptr;
	}

	/* Create Collection and add to Map */
	inline TrashCollectionPtr TrashManager::AddNewCollectionToMap(const Holder holder, const CollectionName& name, const CollectionType& collect_type)
	{
		if (auto Collection_Ptr = CreateCollection(holder, name, collect_type)) {
			Collection->insert(std::make_pair(holder, Collection_Ptr));
			return Collection_Ptr;
		}
		return nullptr;  // fail to create
	}

	/* Find collection in map and return a iterator */
	inline Collection_Map::iterator TrashManager::FindCollectionInMap(const Holder holder, const CollectionName& name, const CollectionType& collect_type) const
	{
		if (auto range_iter = Collection->equal_range(holder); range_iter.first != Collection->end()) {
			for (auto lower = range_iter.first; lower != range_iter.second; lower++) {
				if (!lower->second) continue;
				if (lower->second->GetCollectionType() == collect_type && lower->second->GetName()->compare(name) == 0) return lower;  // both holder and name,type match
			}
			// - holder match but name or type un_match
		}
		return Collection->end();
	}

	/* Remove From Collection, Will Delete Related Collection_Ptr */
	inline bool TrashManager::RemoveCollectionFromMap(const Holder holder, const CollectionName& name, const CollectionType& collect_type)
	{
		if (auto iter = FindCollectionInMap(holder, name, collect_type); iter != Collection->end()) {
			delete iter->second;
			Collection->erase(iter);
			return true;
		}
		return false;
	}

	/* Remove All Collection With Same Holder, Return Count of Removing */
	inline size_t TrashManager::RemoveHolderFromMap(const Holder holder)
	{
		if (auto range_iter = Collection->equal_range(holder); range_iter.first != Collection->end()) {
			for (auto lower = range_iter.first; lower != range_iter.second; lower++) {
				delete lower->second;
			}
		}
		return Collection->erase(holder);
	}

	/* Remove Collection which holded by invaild holder */
	inline void TrashManager::RemoveInvaildHolderFromMap(bool _RemoveNo3DLoad)
	{
		for (auto iter = Collection->begin(); iter != Collection->end(); iter++) {
			if (iter->first == nullptr) {
				if (iter->second != nullptr) delete iter->second;
				Collection->erase(iter);
			} else if (auto holder = iter->first->As<RE::TESObjectREFR>(); holder) {
				if (holder->Is3DLoaded() == 0 && _RemoveNo3DLoad) {
					if (iter->second != nullptr) delete iter->second;
					Collection->erase(iter);
				}
			}
		}
	}

	inline void TrashManager::RemoveSaveBakedCollectionFromMap()
	{
		for (auto iter = Collection->begin(); iter != Collection->end(); iter++) {
			if (iter->first) {
				if (auto _collection = iter->second; _collection != nullptr) {
					if (_collection->IsBakedToSave()) Collection->erase(iter);
				}
			}
		}
	}

	/* Get Collection Ptr*/
	inline TrashCollectionPtr TrashManager::GetCollectionPtr(const Holder holder, const CollectionName& name, const CollectionType& type) const
	{
		if (auto iter = FindCollectionInMap(holder, name, type); iter != Collection->end()) return iter->second;
		return nullptr;  // collection not exist
	}

	/* cast to vector ptr*/
	inline TrashVecPtr TrashManager::GetAsVecPtr(const Holder holder, const CollectionName& name) const
	{
		if (auto collection_ptr = GetCollectionPtr(holder, name,IsVec) ) {
			if (auto ret_ptr = static_cast<TrashVecPtr>(collection_ptr)) return ret_ptr;
		}
		return nullptr;
	}

	inline TrashVecPtr TrashManager::GetAsVecPtr(const TrashCollectionPtr _collection) const 
	{ 
		if (_collection->GetCollectionType() == IsVec) {
			if (auto ret_ptr = static_cast<TrashVecPtr>(_collection)) return ret_ptr;
		}
		return nullptr; 
	}
	/* cast to vector ptr*/

	/* cast to map ptr*/
	inline TrashRefMapPtr TrashManager::GetAsRefMapPtr(const Holder holder, const CollectionName& name) const
	{
		if (auto collection_ptr = GetCollectionPtr(holder, name, IsRefMap)) {
			if (auto ret_ptr = static_cast<TrashRefMapPtr>(collection_ptr)) return ret_ptr;
		}
		return nullptr;
	}
	inline TrashRefMapPtr TrashManager::GetAsRefMapPtr(const TrashCollectionPtr _collection) const 
	{ 
		if (_collection->GetCollectionType()==IsRefMap) {
			if (auto ret_ptr = static_cast<TrashRefMapPtr>(_collection)) return ret_ptr;
		}
		return nullptr; 
	}
	/* cast to map ptr*/

	inline int TrashManager::isVecEmpty(const Holder holder, const CollectionName& name) const
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) return _Vec->IsEmpty();
		CollectionNotExistError(holder, name, "AuxArrayEmpty");
		return -1;
	}

	inline bool TrashManager::VecErase(const Holder holder, const CollectionName& name, const int& index) const
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			if ((int)_Vec->GetSize() <= index) {
				CollectionOutOfRangeError(holder, name, "AuxArrayErase", index);
				return false;
			}
			_Vec->Erase(index);
			return true;
		}
		CollectionNotExistError(holder, name, "AuxArrayErase");
		return false;
	}

	inline std::size_t TrashManager::VecGetValType(const Holder holder, const CollectionName& name, const int& index) const
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			if ((int)_Vec->GetSize() <= index) {
				CollectionOutOfRangeError(holder, name, "AuxArrayGetType", index);
				return None;
			}
			return _Vec->GetValType(index);
		}
		CollectionNotExistError(holder, name, "AuxArrayGetType");
		return None;
	}

	/* Get specific collection */
	/* ↓ Set or insert val to collections 
Will insert if not find the collections */
	inline int TrashManager::VecGetSize(const Holder holder, const CollectionName& name) const
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) return (int)_Vec->GetSize();
		CollectionNotExistError(holder, name, "VecGetSize");
		return -1;
	}

	inline bool TrashManager::VecSetFloat(const Holder holder, const CollectionName& name, const float& arg_Float, const int& index)
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			if (index >= (int)_Vec->GetSize()) {
				CollectionOutOfRangeError(holder, name, "AuxArraySetFloat", index); 
				return false;	
			}
			_Vec->SetFltAt(arg_Float, index);
			return true;
		} 
		else if (auto collection_ptr = AddNewCollectionToMap(holder, name, IsVec)) {
			if (auto newVec_ptr = GetAsVecPtr(collection_ptr)) {
				newVec_ptr->SetFltAt(arg_Float,-1);
				return true;
			}
		}
		CollectionFailToGetAndCreateError(holder, name, "AuxArraySetFloat"); 
		return false;
	}

	inline float TrashManager::VecGetFloat(const Holder holder, const CollectionName& name, const int& index) const
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			if ((int)_Vec->GetSize() <= index) {
				CollectionOutOfRangeError(holder, name, "AuxArrayGetFloat", index); 
				return NAN;
			}
			return _Vec->GetFltAt(index);
		}
		CollectionFailToGetError(holder, name, "AuxArrayGetFloat");
		return NAN;
	}


	inline RE::TESForm* TrashManager::VecGetRef(const Holder holder, const CollectionName& name, int index) const
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			if ((int)_Vec->GetSize() <= index) {
				CollectionOutOfRangeError(holder, name, "AuxArrayGetRef", index);
				return nullptr;
			}
			return _Vec->GetRefAt(index);
		}
		CollectionFailToGetError(holder, name, "AuxArrayGetRef");
		return nullptr;
	}


	inline bool TrashManager::VecSetRef(const Holder holder, const CollectionName& name, RE::TESForm* arg_Ref, const int& index)
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			if ((int)_Vec->GetSize() <= index) {
				CollectionOutOfRangeError(holder, name, "AuxArraySetRef", index);
				return false;
			}
			_Vec->SetRefAt(arg_Ref, index);
			return true;
		} 
		else if (auto collection_ptr = AddNewCollectionToMap(holder, name, IsVec)) {
			if (auto newVec_ptr = GetAsVecPtr(collection_ptr)) {
				newVec_ptr->SetRefAt(arg_Ref, -1);
				return true;
			}
		}
		CollectionFailToGetAndCreateError(holder, name, "AuxArraySetRef");
		return false;
	}

	inline std::vector<RE::TESForm*> TrashManager::VecGetArray_Ref(const Holder holder, const CollectionName& name) const
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			if (!_Vec->IsEmpty()) return _Vec->GetAsRefArray();
			return {};
		}
		CollectionNotExistError(holder, name, "AuxArrayGetRefArray");
		return {};
	}

	inline int TrashManager::VecSetFromArray_Ref(const Holder holder, const CollectionName& name, const std::vector<RE::TESForm*>& src)
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			_Vec->SetFromRefArray(src);
			return (int)_Vec->GetSize();
		} 
		else if (auto collection_ptr = AddNewCollectionToMap(holder, name, IsVec)) {
			if (auto newVec_ptr = GetAsVecPtr(collection_ptr)) {
				newVec_ptr->SetFromRefArray(src);
				return (int)newVec_ptr->GetSize();
			}
		}
		CollectionNotExistAndFailedToCreateError(holder, name, "AuxArraySetFromRefArray");
		return -1;
	}

	inline std::vector<float> TrashManager::VecGetArray_Float(const Holder holder, const CollectionName& name) const
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			if (!_Vec->IsEmpty()) return _Vec->GetAsFltArray();
			return {};
		}
		CollectionNotExistError(holder, name, "AuxArrayGetFloatArray");
		return {};
	}

	inline int TrashManager::VecSetFromArray_Float(const Holder holder, const CollectionName& name, const std::vector<float>& src)
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			_Vec->SetFromFltArray(src);
			return (int)_Vec->GetSize();
		} 
		else if (auto collection_ptr = AddNewCollectionToMap(holder, name, IsVec)) {
			if (auto newVec_ptr = GetAsVecPtr(collection_ptr)) {
				newVec_ptr->SetFromFltArray(src);
				return (int)newVec_ptr->GetSize();
			}
		}
		CollectionNotExistError(holder, name, "AuxArraySetFromFloatArray");
		return -1;
	}

	inline int TrashManager::VecSetFromFormList(const Holder holder, const CollectionName& name, const RE::BGSListForm* formlist)
	{
		if (auto _Vec = GetAsVecPtr(holder, name)) {
			_Vec->SetFromFormList(formlist);
			return (int)_Vec->GetSize();
		}
		else if (auto collection_ptr = AddNewCollectionToMap(holder, name, IsVec)) {
			if (auto newVec_ptr = GetAsVecPtr(collection_ptr)) {
				newVec_ptr->SetFromFormList(formlist);
				return (int)newVec_ptr->GetSize();
			}
		}
		CollectionNotExistError(holder, name, "AuxArraySetFromFormList");
		return -1;
	}


	inline int TrashManager::isMapEmpty(const Holder holder, const CollectionName& name) const
	{
		if (auto _Map = GetAsRefMapPtr(holder, name)) return _Map->IsEmpty();
		CollectionNotExistError(holder, name, "isMapEmpty");
		return -1;
	}

	inline bool TrashManager::MapClear(const Holder holder, const CollectionName& name) const
	{
		if (auto _Map = GetAsRefMapPtr(holder, name)) {
			_Map->Clear();
			return true;
		}
		CollectionNotExistError(holder, name, "MapClear");
		return false;
	}

	inline int TrashManager::MapGetSize(const Holder holder, const CollectionName& name) const
	{
		if (auto _Map = GetAsRefMapPtr(holder, name)) return (int)_Map->GetSize();
		CollectionNotExistError(holder, name, "MapGetSize");
		return -1;
	}

	inline bool TrashManager::MapSetFloat(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key,const float& arg_NewFloat)
	{
		if (auto _Map = GetAsRefMapPtr(holder, name)) {
			_Map->SetFltValue(arg_Key, arg_NewFloat);
			return true;
		}
		else if (auto collection_ptr = AddNewCollectionToMap(holder, name, IsRefMap)) {
			if (auto newMap_ptr = GetAsRefMapPtr(collection_ptr)) {
				newMap_ptr->SetFltValue(arg_Key, arg_NewFloat);
				return true;
			}
		}
		CollectionFailToGetAndCreateError(holder, name, "MapSetFloat");
		return false;
	}

	inline bool TrashManager::MapSetRef(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key, RE::TESForm* arg_NewRef)
	{
		if (auto _Map = GetAsRefMapPtr(holder, name)) {
			_Map->SetRefValue(arg_Key, arg_NewRef);
			return true;
		}
		else if (auto collection_ptr = AddNewCollectionToMap(holder, name, IsRefMap)) {
			if (auto newMap_ptr = GetAsRefMapPtr(collection_ptr)) {
				newMap_ptr->SetRefValue(arg_Key, arg_NewRef);
				return true;
			}
		}
		CollectionFailToGetAndCreateError(holder, name, "MapSetRef");
		return false;
	}

	inline RE::TESForm* TrashManager::MapGetRefWithKey(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			if (!Map_ptr->IsEmpty() && Map_ptr->HasKey(arg_Key)) return Map_ptr->GetRefValueWithKey(arg_Key);
			CollectionIsEmptyAndKeyNoExistError(holder, name, "MapGetRef", arg_Key);
			return nullptr;
		}
		CollectionNotExistError(holder, name, "MapGetRef");
		return nullptr;
	}

	inline float TrashManager::MapGetFloatWithKey(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			if (!Map_ptr->IsEmpty() && Map_ptr->HasKey(arg_Key)) return Map_ptr->GetFltValueWithKey(arg_Key);
			CollectionIsEmptyAndKeyNoExistError(holder, name, "MapGetFloat", arg_Key);
			return NAN;
		}
		CollectionNotExistError(holder, name, "MapGetFloat");
		return NAN;
	}

	inline std::vector<RE::TESForm*> TrashManager::RefMapGetKeys(const Holder holder, const CollectionName& name) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			if (!Map_ptr->IsEmpty()) return Map_ptr->GetKeysAsArray();
			CollectionIsEmptyError(holder, name, "RefMapGetKeys");
			return {};
		}
		CollectionNotExistError(holder, name, "RefMapGetKeys");
		return {};
	}

	inline std::vector<float> TrashManager::MapGetVals_Float(const Holder holder, const CollectionName& name) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			if (!Map_ptr->IsEmpty()) return Map_ptr->GetFltsAsArray();
			CollectionIsEmptyError(holder, name, "MapGetFloatVals");
			return {};
		}
		CollectionNotExistError(holder, name, "MapGetFloatVals");
		return {};
	}

	inline std::vector<RE::TESForm*> TrashManager::MapGetVals_Ref(const Holder holder, const CollectionName& name) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			if (!Map_ptr->IsEmpty()) return Map_ptr->GetRefsAsArray();
			CollectionIsEmptyError(holder, name, "MapGetRefVals");
			return {};
		}
		CollectionNotExistError(holder, name, "MapGetRefVals");
		return {};
	}

	inline bool TrashManager::MapErase(const Holder holder, const CollectionName& name, RE::TESForm* arg_Key) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			Map_ptr->Erase(arg_Key);
			return true;
		}
		CollectionFailToGetError(holder, name, "MapErase");
		return false;
	}

	inline bool TrashManager::MapEraseAll(const Holder holder, const CollectionName& name) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			Map_ptr->EraseAll();
			return true;
		}
		CollectionFailToGetError(holder, name, "MapEraseAll");
		return false;
	}

	inline bool TrashManager::MapEraseInvalidKey(const Holder holder, const CollectionName& name) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			Map_ptr->EraseInvalidKeys();
			return true;
		}
		CollectionFailToGetError(holder, name, "MapEraseInvalidKeys");
		return false;
	}

	inline std::size_t TrashManager::MapGetValueType(const Holder holder, const CollectionName& name,RE::TESForm* arg_Key) const
	{
		if (auto Map_ptr = GetAsRefMapPtr(holder, name)) {
			return Map_ptr->GetValType(arg_Key);
		}
		CollectionFailToGetError(holder, name, "MapGetValueType");
		return None;
	}

}
