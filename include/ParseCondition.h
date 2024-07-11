#pragma once
#include "ReflectFunc.h"

namespace ParseCond
{
	using namespace FuncInfoGetter;
	using FuncID = RE::FUNCTION_DATA::FunctionID;
	using FuncName = std::string;

	inline bool FillCondtionItemData(RE::CONDITION_ITEM_DATA& data, FuncName& func_name, const OpCodeStr& op_code, RE::TESForm* _fst_obj, RE::TESForm* _sec_obj,float compare_flt);
	inline RE::TESConditionItem* CreateCondtionItem(FuncName func_name, OpCodeStr op_code, RE::TESForm* _fst_obj,RE::TESForm* _sec_obj,float compare_flt);
	inline bool IsConditionTrue(RE::TESConditionItem* conditem, RE::TESObjectREFR* action_ref, RE::TESObjectREFR* target_ref);

// will destroy conditem
	inline bool IsConditionTrue(RE::TESConditionItem* conditem,RE::TESObjectREFR* action_ref,RE::TESObjectREFR* target_ref) {
		RE::ConditionCheckParams params{ action_ref, target_ref }; 
		auto unique_ptr = std::make_unique<RE::TESConditionItem>(conditem);
		return conditem->IsTrue(params);
	}

	inline RE::TESConditionItem* CreateCondtionItem(FuncName func_name, OpCodeStr op_code, RE::TESForm* _fst_obj,RE::TESForm* _sec_obj,float compare_flt)
	{
		RE::CONDITION_ITEM_DATA cond_data;
		FillCondtionItemData(cond_data, func_name, op_code, _fst_obj, _sec_obj, compare_flt);
		auto conditionItem = new RE::TESConditionItem();
		conditionItem->data = cond_data;
		return conditionItem;
	}

	RE::TESConditionItem* CreateCondtionItemZeroParams(FuncName& func_name,const OpCodeStr& op_code, RE::TESForm* _fst_obj, RE::TESForm* _sec_obj,float compare_flt)
	{
		return CreateCondtionItem(func_name, op_code, nullptr, nullptr, compare_flt);
	}

	RE::TESConditionItem* CreateCondtionItemCoupleParams(FuncName& func_name,const OpCodeStr& op_code, RE::TESForm* _fst_obj, RE::TESForm* _sec_obj,float compare_flt)
	{
		return CreateCondtionItem(func_name, op_code, _fst_obj, _sec_obj, compare_flt);
	}

	RE::TESConditionItem* CreateCondtionItemSingleParam(FuncName& func_name,const OpCodeStr& op_code, RE::TESForm* _obj,float compare_flt)
	{
		return CreateCondtionItem(func_name, op_code, _obj, _obj, compare_flt);
	}

	inline bool FillCondtionItemData(RE::CONDITION_ITEM_DATA& data, FuncName& func_name, const OpCodeStr& op_code, RE::TESForm* _fst_obj, RE::TESForm* _sec_obj,float compare_flt)
	{
		data.comparisonValue.f = compare_flt;
		data.functionData.params[0] = _fst_obj;
		data.functionData.params[1] = _sec_obj;

		if (auto ret_tuple = GetFunctionID(func_name); std::get<SuccessGet>(ret_tuple)) {
			data.functionData.function = std::get<Info>(ret_tuple);
		} 
		else false;
		
		if (auto ret_tuple = GetFunctionOpCode(op_code); std::get<SuccessGet>(ret_tuple)) {
			data.flags.opCode = std::get<Info>(ret_tuple);
		} 
		else false;

		return true;
	}
}
