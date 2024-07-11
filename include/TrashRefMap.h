#pragma once

#include "TrashCollection.h"
//#define _Map_Write_Lock std::unique_lock lock(_write_mutex);

namespace TrashCollection::RefMap
{
	using namespace TrashCollection;
	using ref = RE::TESForm*;

	using FormID = std::uint32_t;
	

	using namespace TrashCollection;

	class TrashRefMap : public _TrashCollection
	{
		//std::mutex _write_mutex;
		
public:
		std::unique_ptr<std::map<RefMapKey, elem>> RefMap;
		inline RE::TESForm* GetHolder() override;
		inline std::string* GetName() override;
		inline std::size_t GetCollectionType() override;
		inline std::size_t GetSize() override;
		inline std::map<RefMapKey, elem>* GetMapPtr() override;
		TrashRefMap(RE::TESForm* arg_Holder, const std::string& arg_Name);
		virtual ~TrashRefMap() { RefMap = nullptr; }
//Getter
		inline float GetFltValueWithKey(RE::TESForm* arg_key);
		inline ref GetRefValueWithKey(RE::TESForm* arg_key);
		inline std::vector<RE::TESForm*> GetKeysAsArray() const;
		inline std::vector<float> GetFltsAsArray() const;
		inline std::vector<ref> GetRefsAsArray() const;
		inline void Clear() const;
//Operation
		inline size_t HasKey(RE::TESForm* arg_key) const;
		inline size_t InsertFltValue(RE::TESForm* arg_key, const float& arg_flt) const;
		inline size_t InsertRefValue(RE::TESForm* arg_key, ref arg_ref) const;
		inline bool IsEmpty() const;
		inline void EraseAll() const;
		inline void Erase(RE::TESForm* arg_key) const;
		inline void EraseInvalidKeys() const;
//Setter
		inline void SetFltValue(RE::TESForm* arg_key, const float& arg_flt) const;
		inline void SetRefValue(RE::TESForm* arg_key, const ref arg_ref) const;
	};
}

namespace TrashCollection::RefMap
{
	inline RE::TESForm* TrashRefMap::GetHolder() { return this->_Holder; }
	inline std::string* TrashRefMap::GetName() { return this->_Name; }
	inline std::size_t TrashRefMap::GetCollectionType() { return IsRefMap; }
	// construct
	TrashRefMap::TrashRefMap(RE::TESForm* arg_Holder, const std::string& arg_Name)
	{
		this->_Holder = arg_Holder;
		this->_Name = new std::string(arg_Name);
		this->_Type = IsRefMap;
		this->RefMap = std::unique_ptr<std::map<RefMapKey, elem>>(new std::map<RefMapKey, elem>());
	}
	//GetFltValueWithKey
	inline float TrashRefMap::GetFltValueWithKey(RE::TESForm* arg_key)
	{
		if (auto val = RefMap->at(RefMapKey{ arg_key });val.index() == Float) {
			return std::get<float>(val);
		} 
		else if(val.index()==None) PrintRetError(NotInit);
		else PrintRetError(WrongType);
		return NAN;
	}
	//GetRefValueWithKey
	inline ref TrashRefMap::GetRefValueWithKey(RE::TESForm* arg_key)
	{
		if (auto val = RefMap->at(RefMapKey{ arg_key });val.index() == Ref) {
			return std::get<ref>(val);
		} 
		else if(val.index()==None) PrintRetError(NotInit);
		else PrintRetError(WrongType);
		return nullptr;
	}
	//HasKey
	inline size_t TrashRefMap::HasKey(RE::TESForm* arg_key) const { return this->RefMap->count(RefMapKey{ arg_key }); }
	//InsertFltVals
	inline size_t TrashRefMap::InsertFltValue(RE::TESForm* arg_key, const float& arg_flt) const
	{
		this->RefMap->insert(std::pair<RefMapKey, elem>(RefMapKey{ arg_key }, elem{ arg_flt }));
		return this->RefMap->size();
	}
	//InsertRefVals
	inline size_t TrashRefMap::InsertRefValue(RE::TESForm* arg_key, ref arg_ref) const
	{
		this->RefMap->insert(std::pair<RefMapKey, elem>(RefMapKey{ arg_key }, elem{ arg_ref }));
		return this->RefMap->size();
	}
	//IsEmpty
	inline bool TrashRefMap::IsEmpty() const { return this->RefMap->empty(); }
	//EarseAll
	inline void TrashRefMap::EraseAll() const { this->RefMap->clear(); }
	//Earse
	inline void TrashRefMap::Erase(RE::TESForm* arg_key) const { this->RefMap->erase(RefMapKey{ arg_key }); }
	//EarseInvalidKeys
	inline void TrashRefMap::EraseInvalidKeys() const {
		for (auto iter = this->RefMap->begin(); iter != this->RefMap->end(); iter++) { 
			if (!iter->first.a_ref) this->RefMap->erase(iter);
		}
	}
	//GetSize
	inline std::size_t TrashRefMap::GetSize() { return RefMap->size(); }
	//GetMapPtr
	inline std::map<RefMapKey, elem>* TrashRefMap::GetMapPtr() { return RefMap.get(); }
	//GetKeyAsArray
	inline std::vector<RE::TESForm*> TrashRefMap::GetKeysAsArray() const
	{
		std::vector<RE::TESForm*> ret;
		for (auto iter = this->RefMap->begin(); iter != this->RefMap->end(); iter++) { ret.emplace_back(iter->first.a_ref); }
		return ret;
	}
	//GetFltsAsArray
	inline std::vector<float> TrashRefMap::GetFltsAsArray() const
	{
		std::vector<float> ret;
		for (auto iter = RefMap->begin(); iter != RefMap->end(); iter++) { 
			if (iter->second.index() == Float) {
				ret.emplace_back(std::get<float>(iter->second));
			}
		}
		return ret;
	}
	//GetRefsAsArray
	inline std::vector<ref> TrashRefMap::GetRefsAsArray() const
	{
		std::vector<ref> ret;
		for (auto iter = RefMap->begin(); iter != RefMap->end(); iter++) {
			if (iter->second.index() == Ref) {
				ret.emplace_back(std::get<ref>(iter->second));
			}
		}
		return ret;
	}
	//Clear
	inline void TrashRefMap::Clear() const { this->RefMap->clear(); }
	//SetFltValue
	inline void TrashRefMap::SetFltValue(RE::TESForm* arg_key, const float& arg_flt) const {

		if (HasKey(arg_key)) this->RefMap->at(RefMapKey{ arg_key }) = elem{ arg_flt };
		else this->RefMap->insert(std::pair<RefMapKey, elem>(RefMapKey{ arg_key }, elem{ arg_flt }));
	}
	//SetRefValue
	inline void TrashRefMap::SetRefValue(RE::TESForm* arg_key, const ref arg_ref) const {
		if (HasKey(arg_key)) this->RefMap->at(RefMapKey{ arg_key }) = elem{arg_ref};
		else this->RefMap->insert(std::pair<RefMapKey, elem>(RefMapKey{ arg_key }, elem{ arg_ref }));
	}
}
