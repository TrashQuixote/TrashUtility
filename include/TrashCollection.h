#pragma once

#include <memory>
#include <deque>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#define COLLECTION_DEQUE TrashCollection::Deque
#define COLLECTION_REFMAP TrashCollection::RefMap
#define COLLECTION_VECTOR TrashCollection::Vector

namespace TrashCollection {
	using ref = RE::TESForm*;
	using ref_id = RE::FormID;
	using none = std::monostate;
	//using elem =  TrashCollection::elem;
	using elem =  std::variant<none,float,ref>;
	


	enum elem_flag : std::size_t
	{
		None = 0,
		Float = 1,
		Ref = 2,
		Ref_ID = 2,	// for serialization
	};
	
	enum elem_ret_state : std::size_t
	{
		Failed = 0,
		Success = 1,
		NotInit = 2,
		WrongType= 3,
		UnknownType = 4
	};

	inline const char* CheckElemRet(const std::size_t& ret_state) {
		switch (ret_state) {
		case NotInit: return "try to get uninitialized value ";
		case WrongType: return "try to get wrong type value ";
		case Failed: 
		default:
			return "unknown error ";
		}
	}

	enum CollectionInfo : std::size_t
	{
		IsVec = 0,
		IsRefMap = 1,
		UnknownCollection = 2
	};

	struct RefMapKey
	{
		RE::TESForm* a_ref;
		RE::FormID _formid;
		bool operator==(const RefMapKey& a_key) const { return this->_formid == a_key._formid; }
		bool operator>=(const RefMapKey& a_key) const { return this->_formid >= a_key._formid; }
		bool operator<=(const RefMapKey& a_key) const { return this->_formid <= a_key._formid; }
		bool operator>(const RefMapKey& a_key) const { return this->_formid > a_key._formid; }
		bool operator<(const RefMapKey& a_key) const { return this->_formid < a_key._formid; }

		RefMapKey(RE::TESForm* a_ref)
		{
			this->a_ref = a_ref;
			this->_formid = a_ref->GetFormID();
		}
		~RefMapKey() {}
	};

	class _TrashCollection
	{
	protected:
		RE::TESForm* _Holder;
		std::string* _Name;
		std::size_t _Type;
	public:

		_TrashCollection()
		{
			_Holder = nullptr;
			_Name = nullptr;
			_Type = UnknownCollection;
		}
		virtual ~_TrashCollection() 
		{
			delete _Name;
			_Name = nullptr;
		}
		virtual std::size_t GetCollectionType() { return UnknownCollection; }
		virtual RE::TESForm* GetHolder() { return _Holder; }
		virtual std::string* GetName() { return _Name; }
		virtual std::size_t GetSize() { return 0; }
		virtual std::vector<elem>* GetVecPtr() { return nullptr; }
		virtual std::map<RefMapKey, elem>* GetMapPtr() { return nullptr; }
		virtual bool IsBakedToSave() { return _Name->at(0) != '*'; }
		
		bool operator==(const _TrashCollection& hs) { return (this->_Name->compare(*(hs._Name)) == 0) && this->_Type == hs._Type; }
		bool operator!=(const _TrashCollection& hs) { return (this->_Name->compare(*(hs._Name)) != 0); }
		bool operator>(const _TrashCollection& hs) { return (this->_Name->compare(*(hs._Name)) > 0); }
		bool operator>=(const _TrashCollection& hs) { return (this->_Name->compare(*(hs._Name)) >= 0); }
		bool operator<(const _TrashCollection& hs) { return (this->_Name->compare(*(hs._Name)) < 0); }
		bool operator<=(const _TrashCollection& hs) { return (this->_Name->compare(*(hs._Name)) <= 0); }

		
		inline void PrintRetError(const std::size_t& elem_ret_state)
		{
			RE::ConsoleLog::GetSingleton()->Print("error occurred: %s, where collection holder id is %x, name is %s",
				CheckElemRet(elem_ret_state), _Holder->GetFormID(), _Name);
		}

		inline bool PrintOutOfRangeError(const std::size_t& idx)
		{
			RE::ConsoleLog::GetSingleton()->Print(
				"error occurred: index %d is out of range, where collection holder id is %x, name is %s", idx,
				_Holder->GetFormID(), _Name);
			return false;
		}

	};

}
