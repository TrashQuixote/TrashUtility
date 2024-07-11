#pragma once
#include "TrashVec.h"
#include "TrashRefMap.h"
#define COLLECTION_REFMAP TrashCollection::RefMap
#define COLLECTION_VECTOR TrashCollection::Vector

namespace TrashCollectionCreator
{
	using Holder = RE::TESForm*;
	using TrashCollectionPtr = TrashCollection::_TrashCollection*;
	using CollectionName = std::string;
	using CollectionType = std::uint32_t;

	class Creator
	{
	public:
		virtual ~Creator() {}
		Creator() {}
		static TrashCollectionPtr CreatRefMap(const Holder holder, const CollectionName& name);
		static TrashCollectionPtr CreatVector(const Holder holder, const CollectionName& name);
	};

}

namespace TrashCollectionCreator
{
	TrashCollectionPtr Creator::CreatRefMap(const Holder holder, const CollectionName& name)
	{
		return new COLLECTION_REFMAP::TrashRefMap(holder, name);
	}

	TrashCollectionPtr Creator::CreatVector(const Holder holder, const CollectionName& name)
	{
		return new COLLECTION_VECTOR::TrashVec(holder, name);
	}

}

namespace CollectionErrorPrinter
{
	using namespace TrashCollectionCreator;
	inline void CollectionNotExistError(const Holder holder, const CollectionName& name, std::string_view function_name)
	{
		logs::critical(
			"Error occurs in function \"{}\" : Collection not exist. Where holder formid = {:x},collection name = \"{}\" ",
			function_name, (holder->GetFormID()), name);
	}

	inline void CollectionNotExistAndFailedToCreateError(const Holder holder, const CollectionName& name, std::string_view function_name)
	{
		logs::critical(
			"Error occurs in function \"{}\" : Collection not exist and failed to create new one. Where holder formid = {:x},collection name = \"{}\" ",
			function_name, (holder->GetFormID()), name);
	}

	inline void CollectionOutOfRangeError(const Holder holder, const CollectionName& name, std::string_view function_name,const int& idx)
	{
		logs::critical(
			"Error occurs in function \"{}\" : The index is out of range. Where holder formid = {:x},collection name = \"{}\" ,index = {}",
			function_name, (holder->GetFormID()), name,idx);
	}

	inline void CollectionFailToGetError(const Holder holder, const CollectionName& name, std::string_view function_name)
	{
		logs::critical(
			"Error occurs in function \"{}\" : Failed to get collection. Where holder formid = {:x},collection name = "
		    "\"{}\" ",
			function_name, (holder->GetFormID()), name);
	}

	inline void CollectionFailToGetAndCreateError(const Holder holder, const CollectionName& name, std::string_view function_name)
	{
		logs::critical(
			"Error occurs in function \"{}\" : Failed to get and create collection. Where holder formid = {:x},collection name = \"{}\" ",
			function_name, (holder->GetFormID()), name);
	}

	inline void CollectionIsEmptyError(const Holder holder, const CollectionName& name, std::string_view function_name)
	{
		logs::critical(
			"Error occurs in function \"{}\" : Collection is empty. Where holder formid = {:x},collection name = \"{}\" ",
			function_name, (holder->GetFormID()), name);
	}

	inline void CollectionKeyNotExistError(const Holder holder, const CollectionName& name, std::string_view function_name, const RE::TESForm* _Key)
	{
		logs::critical(
			"Error occurs in function \"{}\" : Collection not exist. Where holder formid = {:x},collection name = \"{}\" And the key formid = {:x}, editorID = \"{}\" ",
			function_name, (holder->GetFormID()), name, _Key->GetFormID(), _Key->GetFormEditorID());
	}

	inline void CollectionIsEmptyAndKeyNoExistError(const Holder holder, const CollectionName& name, std::string_view function_name, const RE::TESForm* _Key)
	{
		logs::critical(
			"Error occurs in function \"{}\" : Collection is empty or key doesn't exist. Where holder formid = {:x},collection name = \"{}\" And the key formid = {:x}, editorID = \"{}\" ",
			function_name, (holder->GetFormID()), name, _Key->GetFormID(), _Key->GetFormEditorID());
	}
}
