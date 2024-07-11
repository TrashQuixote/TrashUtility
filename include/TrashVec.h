#pragma once
#include "TrashCollection.h"

namespace TrashCollection::Vector
{
	using namespace TrashCollection;
	


	class TrashVec : public _TrashCollection
	{

public:
	std::unique_ptr<std::vector<elem>> Vec;
	inline RE::TESForm* GetHolder() override;
	inline std::string* GetName() override;
	inline std::size_t GetCollectionType() override;
	inline std::size_t GetSize() override;
	inline std::vector<elem>* GetVecPtr() override;
	TrashVec(RE::TESForm* arg_Holder, const std::string& arg_Name);
	virtual ~TrashVec() { Vec = nullptr; }

// Getter
	inline float GetFltAt(const std::size_t& index);
	inline ref GetRefAt(const std::size_t& index);
	inline std::size_t GetValType(const std::size_t& index);
	inline std::vector<float> GetAsFltArray() const;
	inline std::vector<ref> GetAsRefArray() const;
// Operation
	inline void Reverse() const;
	inline void Clear() const;
	inline void Erase(const int& index);

	inline bool IsEmpty() const;
// Setter
	inline bool SetFltAt(float arg_Setter, const int& index);
	inline bool SetRefAt(ref arg_Setter,const int& index);
	inline std::size_t SetFromFltArray(const std::vector<float>& src_Vec);
	inline size_t SetFromFormList(const RE::BGSListForm* formlist);
	inline std::size_t SetFromIntArray(const std::vector<int>& src_Vec);
	inline std::size_t SetFromRefArray(const std::vector<ref>& src_Vec);
	
	};
}

namespace TrashCollection::Vector
{
	// construct
	TrashVec::TrashVec(RE::TESForm* arg_Holder,const std::string& arg_Name)
	{
		this->_Holder = arg_Holder;
		this->_Name = new std::string(arg_Name);
		this->_Type = IsVec;
		this->Vec = std::unique_ptr<std::vector<elem>>(new std::vector<elem>());
	}
	//GetHolder
	inline RE::TESForm* TrashVec::GetHolder() { return this->_Holder; }
	//GetVecName
	inline std::string* TrashVec::GetName() { return this->_Name; }
	
	//GetCollectionType
	inline std::size_t TrashVec::GetCollectionType() { return IsVec; }
	//GetSize
	inline std::size_t TrashVec::GetSize() { return Vec->size(); }
	//GetVecPtr
	inline std::vector<elem>* TrashVec::GetVecPtr() { return Vec.get(); }
	
	//GetFltAt
	inline float TrashVec::GetFltAt(const std::size_t& index) { 
		if (auto val = Vec->at(index);val.index()==Float) return std::get<float>(val); 
		else if(val.index()==None) PrintRetError(NotInit);
		else PrintRetError(WrongType);
		
		return NAN;
	}
	//GetRefAt
	inline ref TrashVec::GetRefAt(const std::size_t& index){
		if (auto val = Vec->at(index);val.index()==Ref) return std::get<ref>(val); 
		else if(val.index()==None) PrintRetError(NotInit);
		else PrintRetError(WrongType);
		return nullptr;
	}
	//GetValType
	inline std::size_t TrashVec::GetValType(const std::size_t& index)
	{
		
		if (auto val = Vec->at(index); val.index() != NotInit)
		{
			switch (val.index())
			{
			case Ref:
				return Ref;
			case Float:
				return Float;
			default:
				return None;
			}
		}
		else PrintRetError(NotInit);
		return None;
	}
	//GetAsFltArray
	inline std::vector<float> TrashVec::GetAsFltArray() const
	{
		std::vector<float> ret;
		for (auto iter = Vec->begin(); iter != Vec->end(); iter++) { 
			if (iter->index()==Float) ret.emplace_back(std::get<float>(*iter));
		}
		return ret;
	}
	
	//GetAsRefArray
	inline std::vector<ref> TrashVec::GetAsRefArray() const
	{
		std::vector<ref> ret;
		for (auto iter = Vec->begin(); iter != Vec->end(); iter++) { 
			if (iter->index()==Ref) ret.emplace_back(std::get<ref>(*iter));
		}
		return ret;
	}
	//Reverse
	inline void TrashVec::Reverse() const { std::reverse(this->Vec->begin(), this->Vec->end()); }
	//Clear
	inline void TrashVec::Clear() const { this->Vec->clear(); }
	//Erase
	inline void TrashVec::Erase(const int& index)
	{
		if (index < 0) {
			this->Vec->clear();
		}
		else this->Vec->erase(Vec->begin() + index);
		
	}
	
	//IsEmpty
	inline bool TrashVec::IsEmpty() const { return this->Vec->empty(); }
	
	//SetFltAt
	inline bool TrashVec::SetFltAt(float arg_flt,const int& index) { 
		if (index < 0) { 
			this->Vec->emplace_back(elem{ arg_flt }); 
			return true;
		}

		if(index >= (int)GetSize()) return PrintOutOfRangeError(index);
		else Vec->at(index).emplace<float>(arg_flt);

		return true;
	}
	//SetRefAt
	inline bool TrashVec::SetRefAt(ref arg_ref,const int& index) { 
		if (index < 0) {
			this->Vec->emplace_back(elem{ arg_ref });
			return true;
		}

		if(index >= (int)GetSize()) return PrintOutOfRangeError(index);
		else Vec->at(index).emplace<ref>(arg_ref); 
	
		return true;
	}
	// SetFromFltArray


	inline size_t TrashVec::SetFromFltArray(const std::vector<float>& src_Vec) 
	{
		this->Clear();
		for (auto iter = src_Vec.begin(); iter != src_Vec.end(); iter++) { Vec->emplace_back(elem{ *iter }); }
		return this->GetSize();
	}

	inline size_t TrashVec::SetFromFormList(const RE::BGSListForm* formlist)
	{
		this->Clear();
		//auto lambda_ins_to_vec = [&](RE::TESForm* _elem) {
		//	if (_elem) Vec->emplace_back(_elem);
		//	return RE::BSContainer::ForEachResult::kContinue;
		//};
		formlist->ForEachForm(std::function<RE::BSContainer::ForEachResult(RE::TESForm*)>([&](RE::TESForm* _elem) {
			if (_elem) Vec->emplace_back(_elem);
			return RE::BSContainer::ForEachResult::kContinue;
			})
		);
		return this->GetSize();
	}

	// SetFromIntArray
	inline size_t TrashVec::SetFromIntArray(const std::vector<int>& src_Vec)
	{
		this->Clear();
		for (auto iter = src_Vec.begin(); iter != src_Vec.end(); iter++) { Vec->emplace_back(elem{ static_cast<float>( *iter) }); }
		return this->GetSize();
	}
	// SetFromFltArray
	inline size_t TrashVec::SetFromRefArray(const std::vector<ref>& src_Vec) 
	{
		this->Clear();
		for (auto iter = src_Vec.begin(); iter != src_Vec.end(); iter++) { Vec->emplace_back(elem{ *iter }); }
		return this->GetSize();
	}

}
