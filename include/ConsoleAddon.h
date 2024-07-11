#pragma once
#include "SpellMatter.h"
#include "PlayerControls.h"
#include "Utility.h"
#include "TrashSlowTime.h"
#include "SoundMatter.h"
#include "MiscDistance.h"
#include "MiscPosAndHeadingAngle.h"
#include "ApplyHit.h"


// ========================== get function name ==========================
#define GetFunctionName(id) case id : return #id
#define GET_FUNC_NAME	GetFunctionName(GetPosAsArray);\
						GetFunctionName(SetPosAlt);\
						GetFunctionName(GetHeadingPointAngle);\
						GetFunctionName(GetHeadingAngleBetweenPoints);\
						GetFunctionName(GetHeadingAngleX);\
						GetFunctionName(GetHeadingPointAngleX);\
						GetFunctionName(GetHeadingAngleBetweenPointsX);\
						GetFunctionName(GetDistanceFromPoint2D);\
						GetFunctionName(GetDistanceBetweenPoints2D);\
						GetFunctionName(GetDistance2D);\
						GetFunctionName(GetDistanceBetweenPoints3D);\
						GetFunctionName(GetDistanceFromPoint3D);\
						GetFunctionName(GetDistance3D);\
						GetFunctionName(PlaySoundAtPoint);\
						GetFunctionName(InstantCastToActors);\
						GetFunctionName(InstantCastToActorsWithFilter);\
						GetFunctionName(DumpCollection);\
						GetFunctionName(ApplyMeleeHit);\
						GetFunctionName(ApplyHit);\
						GetFunctionName(ApplyProjHit);\
						GetFunctionName(SetSlowTimeMult);\
						GetFunctionName(GetSlowTimeMult);\
						GetFunctionName(GetInSameCellOrWorldSpace);\
						GetFunctionName(DumpAllCollection);\
						GetFunctionName(DumpAllAuxArr);\
						GetFunctionName(DumpAllRefMap);\
						GetFunctionName(LookMovementScale);\
						GetFunctionName(IsLookMovementScale);\
						GetFunctionName(GetMoveDirection);\
						GetFunctionName(GetLookMoveDirection);\
						GetFunctionName(GetLookMovementScale);\
						GetFunctionName(IsMoveDisabled);\
						GetFunctionName(SetMoveDisabled);\
						GetFunctionName(ReverseMove);\
						GetFunctionName(ReverseLook);\
						GetFunctionName(SetAllReverse);\
						GetFunctionName(SetMoveOverwrite);\
						GetFunctionName(SetLookMoveOverwrite);\
						GetFunctionName(GetMoveOverwrite);\
						GetFunctionName(GetLookMoveOverwrite);
// ========================== get function name ==========================

// ========================== get function info ==========================
#define RetFuncInfo(func_name) case func_name: return FuncInfo
#define FuncInfoNotFind return FuncInfo{}
#define RET_FUNC_INFO	RetFuncInfo(GetPosAsArray) { pRef };\
						RetFuncInfo(SetPosAlt) { pRef, pFlt, pFlt, pFlt, pBool };\
						RetFuncInfo(GetHeadingPointAngle) { pRef, pFlt, pFlt, pBool };\
						RetFuncInfo(GetHeadingAngleBetweenPoints) { pFlt, pFlt, pFlt, pFlt, pFlt, pFlt };\
						RetFuncInfo(GetHeadingAngleX) { pRef, pRef, pBool };\
						RetFuncInfo(GetHeadingPointAngleX) { pRef, pFlt, pFlt, pFlt, pBool };\
						RetFuncInfo(GetHeadingAngleBetweenPointsX) { pFlt, pFlt, pFlt, pFlt, pFlt, pFlt, pFlt, pBool };\
						RetFuncInfo(GetDistanceFromPoint2D) { pRef, pFlt, pFlt };\
						RetFuncInfo(GetDistanceBetweenPoints2D) { pFlt, pFlt, pFlt, pFlt };\
						RetFuncInfo(GetDistance2D) { pRef, pRef };\
						RetFuncInfo(GetDistanceBetweenPoints3D) { pFlt, pFlt, pFlt, pFlt, pFlt, pFlt };\
						RetFuncInfo(GetDistanceFromPoint3D) { pRef, pFlt, pFlt, pFlt };\
						RetFuncInfo(GetDistance3D) { pRef, pRef };\
						RetFuncInfo(PlaySoundAtPoint) { pForm, pFlt, pFlt, pFlt };\
						RetFuncInfo(InstantCastToActors) { pRef, pForm, pFlt, pBool ,pBool ,pRef };\
						RetFuncInfo(InstantCastToActorsWithFilter) { pRef, pForm, pFlt, pBool, pBool, pRef, pForm, pBool };\
						RetFuncInfo(DumpCollection) { pForm, pStr };\
						RetFuncInfo(ApplyMeleeHit) { pRef, pRef, pBool };\
						RetFuncInfo(ApplyHit) { pRef, pRef, pForm, pBool };\
						RetFuncInfo(ApplyProjHit) { pRef, pRef, pForm, pForm };\
						RetFuncInfo(SetSlowTimeMult) { pFlt, pFlt, pBool };\
						RetFuncInfo(GetSlowTimeMult) { pBool };\
						RetFuncInfo(GetInSameCellOrWorldSpace) { pRef, pRef };\
						RetFuncInfo(DumpAllCollection) { pNone };\
						RetFuncInfo(DumpAllAuxArr) { pNone };\
						RetFuncInfo(DumpAllRefMap) { pNone };\
						RetFuncInfo(LookMovementScale) { pFlt,pInt };\
						RetFuncInfo(IsLookMovementScale) { pInt };\
						RetFuncInfo(GetMoveDirection) { pInt };\
						RetFuncInfo(GetLookMoveDirection) { pInt };\
						RetFuncInfo(GetLookMovementScale) { pInt };\
						RetFuncInfo(IsMoveDisabled) { pInt };\
						RetFuncInfo(SetMoveDisabled) { pBool,pInt };\
						RetFuncInfo(ReverseMove) { pBool,pInt };\
						RetFuncInfo(ReverseLook) { pBool,pInt };\
						RetFuncInfo(SetAllReverse) { pBool };\
						RetFuncInfo(SetMoveOverwrite) { pInt,pInt };\
						RetFuncInfo(SetLookMoveOverwrite) { pFlt,pInt };\
						RetFuncInfo(GetMoveOverwrite) { pInt };\
						RetFuncInfo(GetLookMoveOverwrite) { pInt };

// ========================== get function info ==========================

// ========================== exe function ==========================

#define CallFunction(name_space,func_name) case func_name : return name_space::func_name
#define CallClassFunc(name_space_class,func_name) case name_space_class : return name_space_class->func_name
#define CallFunctionNoRet(name_space,v_func_name) case v_func_name : name_space::v_func_name
#define ReturnVoid return Ret_Void{}
#define MismatchFunc return std::monostate{}

#define Arg_Ref(x)  (args.at(x).index() == arg_ref ? std::get<RE::TESObjectREFR*>(args.at(x)) : nullptr)
#define Arg_Form(x) (args.at(x).index() == arg_form ? std::get<RE::TESForm*>(args.at(x)) : nullptr)
#define Arg_Flt(x)  (args.at(x).index() == arg_float ? std::get<float>(args.at(x)) : 0.0f)
#define Arg_Int(x)  (args.at(x).index() == arg_int ? std::get<int>(args.at(x)) : 0)
#define Arg_Bool(x) (args.at(x).index() == arg_bool ? std::get<bool>(args.at(x)) : false)
#define Arg_Str(x)  (args.at(x).index() == arg_str ? std::get<std::string>(args.at(x)) : "")

#define EXEC_FUNC	CallFunction(Trash_Function::Pos, GetPosAsArray)							(Arg_Ref(0));\
					CallFunctionNoRet(Trash_Function::Pos, SetPosAlt)							(Arg_Ref(0), Arg_Flt(1), Arg_Flt(2), Arg_Flt(3), Arg_Bool(4)); ReturnVoid;\
					CallFunction(Trash_Function::HeadingAngle, GetHeadingPointAngle)			(Arg_Ref(0), Arg_Flt(1), Arg_Flt(2), Arg_Bool(4));\
					CallFunction(Trash_Function::HeadingAngle, GetHeadingAngleBetweenPoints)	(Arg_Flt(0), Arg_Flt(1), Arg_Flt(2), Arg_Flt(3), Arg_Flt(4), Arg_Flt(5));\
					CallFunction(Trash_Function::HeadingAngle, GetHeadingAngleX)				(Arg_Ref(0), Arg_Ref(1), Arg_Bool(2));\
					CallFunction(Trash_Function::HeadingAngle, GetHeadingPointAngleX)			(Arg_Ref(0), Arg_Flt(1), Arg_Flt(2), Arg_Flt(3), Arg_Bool(4));\
					CallFunction(Trash_Function::HeadingAngle, GetHeadingAngleBetweenPointsX)	(Arg_Flt(0), Arg_Flt(1), Arg_Flt(2), Arg_Flt(3), Arg_Flt(4), Arg_Flt(5), Arg_Flt(6), Arg_Bool(7));\
					CallFunction(Trash_Function::Distance, GetDistanceFromPoint2D)				(Arg_Ref(0), Arg_Flt(1), Arg_Flt(2));\
					CallFunction(Trash_Function::Distance, GetDistanceBetweenPoints2D)			(Arg_Flt(0), Arg_Flt(1), Arg_Flt(2), Arg_Flt(3));\
					CallFunction(Trash_Function::Distance, GetDistance2D)						(Arg_Ref(0), Arg_Ref(1));\
					CallFunction(Trash_Function::Distance, GetDistanceBetweenPoints3D)			(Arg_Flt(0), Arg_Flt(1), Arg_Flt(2), Arg_Flt(3), Arg_Flt(4), Arg_Flt(5));\
					CallFunction(Trash_Function::Distance, GetDistanceFromPoint3D)				(Arg_Ref(0), Arg_Flt(1), Arg_Flt(2), Arg_Flt(3));\
					CallFunction(Trash_Function::Distance, GetDistance3D)						(Arg_Ref(0), Arg_Ref(1));\
					CallFunction(Trash_Function::Sound, PlaySoundAtPoint)						(Arg_Form(0)->As<RE::BGSSoundDescriptorForm>(), Arg_Flt(1), Arg_Flt(2), Arg_Flt(3));\
					CallFunction(Trash_Function::SpellApplying, InstantCastToActors)			(Arg_Ref(0), Arg_Form(1)->As<RE::MagicItem>(), Arg_Flt(2), Arg_Bool(3), Arg_Bool(4), Arg_Ref(5));\
					CallFunction(Trash_Function::SpellApplying, InstantCastToActorsWithFilter)	(Arg_Ref(0), Arg_Form(1)->As<RE::MagicItem>(), Arg_Flt(2), Arg_Bool(3), Arg_Bool(4), Arg_Ref(5), Arg_Form(6), Arg_Bool(7));\
					CallFunctionNoRet(TrashUtility::DumpCollection, DumpCollection)				(Arg_Form(0), Arg_Str(1), Arg_Int(2)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::ApplyHit, ApplyMeleeHit)					(Arg_Ref(0)->As<RE::Character>(), Arg_Ref(1)->As<RE::Character>(), Arg_Bool(2)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::ApplyHit, ApplyHit)						(Arg_Ref(0)->As<RE::Character>(), Arg_Ref(1)->As<RE::Character>(), Arg_Form(2)->As<RE::TESObjectWEAP>(), Arg_Bool(3)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::ApplyHit, ApplyProjHit)					(Arg_Ref(0)->As<RE::Character>(), Arg_Ref(1)->As<RE::Character>(), Arg_Form(2)->As<RE::TESAmmo>(), Arg_Form(3)->As<RE::TESObjectWEAP>()); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::TimeMultSetter, SetSlowTimeMult)			(Arg_Flt(0), Arg_Flt(1), Arg_Bool(2)); ReturnVoid;\
					CallFunction(Trash_Function::TimeMultSetter, GetSlowTimeMult)				(Arg_Bool(0));\
					CallFunction(Trash_Function::Distance, GetInSameCellOrWorldSpace)			(Arg_Ref(0), Arg_Ref(1));\
					CallFunctionNoRet(TrashUtility::DumpCollection, DumpAllCollection)			(); ReturnVoid;\
					CallFunctionNoRet(TrashUtility::DumpCollection, DumpAllAuxArr)				(); ReturnVoid;\
					CallFunctionNoRet(TrashUtility::DumpCollection, DumpAllRefMap)				(); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::Control, LookMovementScale)				(Arg_Flt(0),Arg_Int(1)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::Control, IsLookMovementScale)				(Arg_Int(0)); ReturnVoid;\
					CallFunction(Trash_Function::Control, GetMoveDirection)						(Arg_Int(0)); \
					CallFunction(Trash_Function::Control, GetLookMoveDirection)					(Arg_Int(0)); \
					CallFunction(Trash_Function::Control, GetLookMovementScale)					(Arg_Int(0)); \
					CallFunction(Trash_Function::Control, IsMoveDisabled)						(Arg_Int(0)); \
					CallFunctionNoRet(Trash_Function::Control, SetMoveDisabled)					(Arg_Bool(0),Arg_Int(1)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::Control, ReverseMove)						(Arg_Bool(0),Arg_Int(1)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::Control, ReverseLook)						(Arg_Bool(0),Arg_Int(1)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::Control, SetAllReverse)					(Arg_Bool(0)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::Control, SetMoveOverwrite)				(Arg_Int(0),Arg_Int(1)); ReturnVoid;\
					CallFunctionNoRet(Trash_Function::Control, SetLookMoveOverwrite)			(Arg_Flt(0),Arg_Int(1)); ReturnVoid;\
					CallFunction(Trash_Function::Control, GetMoveOverwrite)						(Arg_Int(0));\
					CallFunction(Trash_Function::Control, GetLookMoveOverwrite)					(Arg_Int(0));
// ========================== exe function ==========================



namespace TrashConsoleAddon::FuncInformation
{
	using FuncName = std::string;
	using param_type_id = std::uint8_t;
	using u_Func_ID = std::uint16_t;
	
	enum param_type : param_type_id
	{
		pBool = 0,
		pInt = 1,
		pFlt = 2,
		pRef = 3,
		pForm = 4,
		pStr = 5,
		pNone = 6
	};

	enum Func_ID : uint16_t
	{
		GetPosAsArray = 0,
		SetPosAlt = 1,
		GetHeadingPointAngle = 2,
		GetHeadingAngleBetweenPoints = 3,
		GetHeadingAngleX = 4,
		GetHeadingPointAngleX = 5,
		GetHeadingAngleBetweenPointsX = 6,
		GetDistanceFromPoint2D = 7,
		GetDistanceBetweenPoints2D = 8,
		GetDistance2D = 9,
		GetDistanceBetweenPoints3D = 10,
		GetDistanceFromPoint3D = 11,
		GetDistance3D = 12,
		PlaySoundAtPoint = 13,
		InstantCastToActors = 14,
		InstantCastToActorsWithFilter = 15,
		DumpCollection = 16,
		ApplyMeleeHit = 17,
		ApplyHit = 18,
		ApplyProjHit = 19,
		SetSlowTimeMult = 20,
		GetSlowTimeMult = 21,
		GetInSameCellOrWorldSpace = 22,
		DumpAllCollection = 23,
		DumpAllAuxArr = 24,
		DumpAllRefMap = 25,

		LookMovementScale = 26,
		IsLookMovementScale = 27,
		GetMoveDirection = 28,
		GetLookMoveDirection = 29,
		IsMoveDisabled = 30,
		SetMoveDisabled = 31,
		ReverseMove = 32,
		ReverseLook = 33,
		SetAllReverse = 34,
		GetLookMovementScale = 35,
		SetMoveOverwrite = 36,
		SetLookMoveOverwrite = 37,
		GetMoveOverwrite = 38,
		GetLookMoveOverwrite = 39,
		Total
	};

	FuncName get_func_name(u_Func_ID _id) {
		switch (_id) {

			GET_FUNC_NAME

		default:return "";
		}
	}

	struct Func
	{
		FuncName _func_name;
		u_Func_ID _func_id;
		
		Func(u_Func_ID init_func_id) { 
			_func_id =  init_func_id ;
			_func_name = get_func_name(init_func_id);
			ToLowercaseStr(_func_name);
		}

		Func(const std::string& _funcname) {
			_func_id = Total;
			_func_name = _funcname;
		}
		~Func() {}

		bool operator==(const Func& a_key) const { return this->_func_name.compare(a_key._func_name)==0; }
		bool operator<(const Func& a_key) const { return this->_func_name.compare(a_key._func_name)<0; }
	};

	struct FuncInfo
	{
		enum : size_t
		{
			VaildFlag = 63
		};
		std::bitset<64> params_type;
		std::size_t params_count = 0;
		FuncInfo(std::initializer_list<param_type> init_list)
		{
			if (init_list.size()==1 && *init_list.begin() == pNone)
			{
				params_type.set(VaildFlag, true);
				return;
			}

			size_t fst_pos = 0;
			size_t sec_pos = 1;
			size_t tri_pos = 2;
			for (const auto& elem : init_list) {
				switch (elem) {
#define SetParamsType(fstPos, secPos, triPos, tFlag, sFlag, fFlag) params_type.set(fstPos, fFlag);\
																   params_type.set(secPos, sFlag);\
																   params_type.set(triPos, tFlag);
				case pBool:
					SetParamsType(fst_pos, sec_pos, tri_pos, 0, 0, 0)
					params_count++;
					break;
				case pInt:
					SetParamsType(fst_pos, sec_pos, tri_pos, 0, 0, 1)
					params_count++;
					break;
				case pFlt:
					SetParamsType(fst_pos, sec_pos, tri_pos, 0, 1, 0)
					params_count++;
					break;
				case pRef:
					SetParamsType(fst_pos, sec_pos, tri_pos, 0, 1, 1)
					params_count++;
					break;
				case pForm:
					SetParamsType(fst_pos, sec_pos, tri_pos, 1, 0, 0) 
					params_count++;
					break;
				case pStr:
					SetParamsType(fst_pos, sec_pos, tri_pos, 1, 0, 1)
					params_count++;
					break;
				default:
					SetParamsType(fst_pos, sec_pos, tri_pos, 0, 0, 0)
					break;
				}
				fst_pos += 3;
				sec_pos += 3;
				tri_pos += 3;
			}
			params_type.set(VaildFlag, true);
		}
		inline param_type_id get_param_type(const bool& bit_0, const bool& bit_1, const bool& bit_2) { return (bit_0 * 1 + bit_1 * 2 + bit_2 * 4); }

		bool Vaild() const { return params_type.test(VaildFlag); }

		inline param_type_id GetParamType(const std::size_t& pos) {
			return get_param_type(params_type.test(pos),params_type.test(pos + 1),params_type.test(pos + 2));
		}

		inline void GetParamsTypeVec(std::vector<param_type_id>& vec_params_types) {
			for (std::size_t round = 0, pos = 0; round < params_count; round++, pos += 3) {
				vec_params_types.emplace_back(GetParamType(pos));
			}
		}

		static std::string GetParamTypeStr(const param_type_id& p_t_id ) {
			switch (p_t_id) {
				case pBool: return "bool"; 
				case pInt: return "int";
				case pFlt: return "float";
				case pRef: return "ref";
				case pForm: return "form";
				case pStr: return "string";
				case pNone: return "none";
				default: 
					return"unknown_type";
			}
		}

		//pBool = 0, pInt = 1, pFlt = 2, pRef = 3,pForm = 4,pStr = 5
		static void DumpParamsTypeVec(std::vector<param_type_id>& vec_params_types) {
			RE::ConsoleLog::GetSingleton()->Print("Params Nums: %d", vec_params_types.size());
			for (const auto& p_t_id : vec_params_types) {
				RE::ConsoleLog::GetSingleton()->Print(GetParamTypeStr(p_t_id).data());
			}
		}
		FuncInfo() { params_type.set(VaildFlag, false); }
		~FuncInfo() {}
	};


	class TrashFuncVec
	{
		using u_Func_ID = uint16_t;
		using ID_AND_INFO = std::pair<u_Func_ID,FuncInfo>;
		std::set<Func> func_set;
		std::vector<FuncInfo> func_vec;

public:
		inline u_Func_ID get_func_id(const std::string& _func_name) {
			Func finder{_func_name};
			if (auto iter = func_set.find(finder); iter != func_set.end()) {
				return iter->_func_id;
			}
			return Total;
		}

		inline ID_AND_INFO get_func_info(u_Func_ID _func_id) {
			if (_func_id < func_vec.size()) return std::make_pair(_func_id,func_vec.at(_func_id));
			
			return { Total, { } };
		}

		static TrashFuncVec* GetSingleton();
		inline ID_AND_INFO GetFuncInfo(const std::string& _func_name) { return get_func_info(get_func_id(_func_name)); }

		FuncInfo ConstructFuncInfo(u_Func_ID _func_id) {
			switch (_func_id) {
				
				RET_FUNC_INFO

			default:
				FuncInfoNotFind;
				break;
			}
		}

		TrashFuncVec(TrashFuncVec& another) = delete;
		TrashFuncVec(const TrashFuncVec& another) = delete;
		void operator=(const TrashFuncVec&) = delete;
	private:
		TrashFuncVec(){
			for (u_Func_ID func_id = 0; func_id < Total; func_id++) {
				func_set.emplace(Func{ static_cast<Func_ID>(func_id) });
				func_vec.emplace_back(ConstructFuncInfo(func_id));
			}
		};
		~TrashFuncVec(){};
	};

	TrashFuncVec* TrashFuncVec::GetSingleton() { 
		static TrashFuncVec* tfm_singleton = new TrashFuncVec();
		return tfm_singleton;
	}
}


namespace TrashConsoleAddon::ExecuteFunc
{
	using namespace Trash_Function;
	using namespace Trash_Function::SpellApplying;
	using namespace TrashConsoleAddon::FuncInformation;
	using namespace TrashUtility;
	using namespace TrashUtility::DumpCollection;

	enum Arg_flag : std::size_t
	{
		arg_unk = 0,
		arg_int,
		arg_float,
		arg_bool,
		arg_str,
		arg_form,
		arg_ref
	};

	enum Ret_flag : std::size_t
	{
		ret_unk = 0,
		ret_int =1,
		ret_float = 2,
		ret_bool = 3,
		ret_form = 4,
		ret_ref = 5,
		ret_vec_flt = 6,
		ret_vec_form = 7,
		ret_vec_ref = 8,
		ret_void = 9
	};
	struct Ret_Void{ };
	using variant_ret = std::variant<std::monostate, int, float, bool, RE::TESForm*, RE::TESObjectREFR*,std::vector<float>, std::vector<RE::TESForm*>,std::vector<RE::TESObjectREFR*>,Ret_Void>;
	using variant_arg = std::variant<std::monostate, int, float, bool, std::string, RE::TESForm*,RE::TESObjectREFR*>;

	class ExecuteFunction
	{
		using u_Func_ID = std::uint16_t;
	public:
		ExecuteFunction(){};
		~ExecuteFunction(){};
		static variant_ret ExeFunc(u_Func_ID func_id, std::vector<variant_arg>& args);
		static bool PrintResult(variant_ret& res);
	private:
	};


	variant_ret ExecuteFunction::ExeFunc(u_Func_ID func_id, std::vector<variant_arg>& args)
	{
		switch (func_id) {

			EXEC_FUNC

		default:
			MismatchFunc;
		}

	}

	bool ExecuteFunction::PrintResult(variant_ret& ret) {
#define ConsolePrint  RE::ConsoleLog::GetSingleton()->Print
#define Ret_Ref		  std::get<RE::TESObjectREFR*>(ret)
#define Ret_Form	  std::get<RE::TESForm*>(ret)
#define Ret_Flt       std::get<float>(ret)
#define Ret_Int       std::get<int>(ret)
#define Ret_Bool      std::get<bool>(ret)
#define Ret_Str       std::get<std::string>(ret)

#define Ret_Vec_Flt      std::get<std::vector<float>>(ret)
#define Ret_Vec_Ref      std::get<std::vector<RE::TESObjectREFR*>>(ret)
#define Ret_Vec_Form     std::get<std::vector<RE::TESForm*>>(ret)
		switch (ret.index()) {
		case ret_void: return true;
		case ret_int: ConsolePrint("%d",Ret_Int); break;
		case ret_float: ConsolePrint("%.7f",Ret_Flt); break;
		case ret_bool: ConsolePrint( (Ret_Bool==true ? "true" : "false") );break;
		case ret_form: PrintForm(Ret_Form);break;
		case ret_ref: PrintRef(Ret_Ref);break;
		case ret_vec_flt: PrintFltVec(Ret_Vec_Flt);break;
		case ret_vec_form: PrintFormVec(Ret_Vec_Form);break;
		case ret_vec_ref: PrintFormVec(Ret_Vec_Ref);break;
		
		case ret_unk:break;
		default:
			break;
		}

		return false;
	}

}

namespace TrashConsoleAddon
{
	using namespace TrashConsoleAddon::FuncInformation;
	using namespace TrashConsoleAddon::ExecuteFunc;
	using namespace Trash_Function::Misc;

	class ConsoleAddon
	{
	public:
		static void Hook();

	private:
		static void PrintFuncList();
		static bool PraseArgs(const std::string& s_arg, const param_type_id& ptype_id, std::vector<variant_arg>& params); 
		static bool PraseCmd(const std::string& _cmd); 
		static void CompileAndRun(RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name, RE::TESObjectREFR* a_targetRef);
		inline static REL::Relocation<decltype(CompileAndRun)> _Ori_CompileAndRun;
	};

	
	void ConsoleAddon::PrintFuncList()
	{
		std::string output = "";
		std::string s_temp = "";
		for (u_Func_ID i = 0; i < Total; i++) { 
			switch (i)
			{
			case LookMovementScale:
				RE::ConsoleLog::GetSingleton()->Print(" ");
				RE::ConsoleLog::GetSingleton()->Print("The Flag Argument In Player Control Function, Represents The Direction ");
				RE::ConsoleLog::GetSingleton()->Print("In Most Cases As: |flag = 0 -X | flag =1 -Y | flag =2 -X&&Y | flag =3 -X||Y | other -X||Y ");
				RE::ConsoleLog::GetSingleton()->Print(" ");
				break;
			default:
				break;
			}
			
			output = get_func_name(i);
			auto func_info = TrashFuncVec::GetSingleton()->GetFuncInfo( GetLowercaseStr(output) ).second;
			output += " (  ";

			if(func_info.params_count == 0) output.append(" )");
			else{
				std::vector<param_type_id> params_types;
				func_info.GetParamsTypeVec(params_types);

				std::size_t idx = 0;
				for (const auto& id : params_types) {
					output.append(FuncInfo::GetParamTypeStr(id));
					if (idx++ < params_types.size() -1 ) output.append(", ");
					if (idx == params_types.size() ) output.append(" )");
				}
			}

			RE::ConsoleLog::GetSingleton()->Print(output.data());
			output.clear();
		}
		
	}

	bool ConsoleAddon::PraseArgs(const std::string& s_arg, const param_type_id& ptype_id, std::vector<variant_arg>& params)
	{
#define ErrorMsg(massage,...) RE::ConsoleLog::GetSingleton()->Print(massage,##__VA_ARGS__ ); return false
		if (s_arg.empty()) { ErrorMsg("An error occurred while parsing a parameter. the parameter is none"); }

		switch (ptype_id) 
		{
		case pBool:
			if (s_arg == "true" || s_arg == "1") params.emplace_back((bool)true);
			else if (s_arg == "false" || s_arg == "0") params.emplace_back((bool)false);
			else { ErrorMsg("An error occurred while parsing a bool parameter. expected true(1) or false(0) "); }
			break;
		case pInt: 
			if (s_arg.at(0) == '-' ? std::isdigit(s_arg.at(1)) : std::isdigit(s_arg.at(0))) {
				int int_arg;
				try { int_arg = std::stoi(s_arg); }
				catch (...) { ErrorMsg("Invalid int argument"); }
				params.emplace_back(int_arg);
			}
			else { ErrorMsg("An error occurred while parsing a int parameter. expected digit number ");}
			break;
		case pFlt:
			if (s_arg.at(0) == '-' ? std::isdigit(s_arg.at(1)) : std::isdigit(s_arg.at(0))) {
				float flt_arg;
				try { flt_arg = std::stof(s_arg); }
				catch (...) { ErrorMsg("Invalid float argument"); }
				params.emplace_back(flt_arg);
			} 
			else { ErrorMsg("An error occurred while parsing a float parameter. expected digit number ");}
			break;
		case pRef:
			{
				if (GetLowercaseStr(s_arg).compare("player") == 0) {
					params.emplace_back(RE::PlayerCharacter::GetSingleton()->AsReference());
					break;
				}

				if (IsXDigitsStr(s_arg)) {
					RE::FormID f_id = static_cast<RE::FormID>(std::stoul(s_arg, nullptr, 16));
					if (auto _form = RE::TESForm::LookupByID(f_id)) {
						if (auto _ref = _form->AsReference()) {
							params.emplace_back(_ref);
						}
						else { ErrorMsg("An error occurred while parsing a reference parameter. find a form but it is not a object reference");}
					}
					else { ErrorMsg("An error occurred while parsing a reference parameter. can't not find the object reference with formid %s",&s_arg);}
				} 
				else { ErrorMsg("An error occurred while parsing a ref parameter. only accept formid of a object reference"); }
				break;
			}
		case pForm:
			{
				if (IsXDigitsStr(s_arg)) {
					RE::FormID f_id = static_cast<RE::FormID>(std::stoul(s_arg, nullptr, 16));
					if (auto _form = RE::TESForm::LookupByID(f_id)) {
						params.emplace_back(_form);
					}
					else { ErrorMsg("An error occurred while parsing a form parameter. can't not find the form with formid %s",&s_arg); }
				} 
				else 
				{
					if (auto _form = RE::TESForm::LookupByEditorID(s_arg)) {
						params.emplace_back(_form);
					} 
					else { ErrorMsg("An error occurred while parsing a form parameter. can't not find the form"); }
				}
				break;
			}
		case pStr:
			params.emplace_back(s_arg);
			break;
		default:
			break;
		}
		return true;
	}
	
	enum TrashFuncPraseState
	{
		PraseFailed = false,		// function is not belong to trash utility will make console prase continue
		PraseSuccess = true			// function is belong to trash utility
	};

	bool ConsoleAddon::PraseCmd(const std::string& _cmd)
	{
constexpr auto HELP = "trashutility";
#define PrintMsg(Msg, ...) RE::ConsoleLog::GetSingleton()->Print(Msg ,##__VA_ARGS__);
		if (_cmd.empty()) return PraseFailed;

		std::vector<std::string> cmd_sections;
		StringSplit(_cmd, ' ', cmd_sections);
		ToLowercaseStr(cmd_sections[0]);

		if (cmd_sections[0] == HELP) {
			PrintFuncList();
			return PraseSuccess;
		} 

		if (const FuncName& func_name = cmd_sections[0]; !func_name.empty()) {

			auto id_and_info = TrashFuncVec::GetSingleton()->GetFuncInfo(func_name);

			if ( id_and_info.first != Total && id_and_info.second.Vaild() ) {
				if (id_and_info.second.params_count != cmd_sections.size() - 1) {
					PrintMsg("An error occurred: The function received fewer arguments than expected: expected %d arguments.", id_and_info.second.params_count);
					PrintMsg("Enter \" TrashUtility \" to view the function signature of TrashUtility plugin.");
					return PraseSuccess;
				}
				
				
				auto& f_info = id_and_info.second;

				std::vector<param_type_id> params_types;
				f_info.GetParamsTypeVec(params_types);

				std::vector<variant_arg> params;
				for (size_t type_idx = 0, arg_idx = 1; type_idx < params_types.size(); type_idx++, arg_idx++) {

					if (! PraseArgs(cmd_sections.at(arg_idx), params_types.at(type_idx), params)) {
						PrintMsg("Failed to prase paramster.");
						return PraseSuccess;
					}
				}
				
				if (params.size() == params_types.size()) {
					
					if (auto res = ExecuteFunction::ExeFunc(id_and_info.first, params); res.index() != 0) {
						ExecuteFunction::PrintResult(res);
						return PraseSuccess;
					} 
					else 
					{
						PrintMsg("Failed to execute function: %s", &cmd_sections[0]);
						return PraseSuccess;
					}
				}
			}
			else 
			{
				return PraseFailed;
			}
		}
		return PraseFailed;
	}
	
	void ConsoleAddon::Hook()
	{
		SKSE::AllocTrampoline(1 << 4);
	
		
		TrashFuncVec::GetSingleton();
		//REL::Relocation<std::uintptr_t> _original{ REL::ID(52065).address() + 0xE2 };
		REL::Relocation<std::uintptr_t> _original{ REL::VariantID(52065, 52952,0), REL::VariantOffset(0xE2, 0x52, 0) };
		auto& trampoline = SKSE::GetTrampoline();
		_Ori_CompileAndRun = trampoline.write_call<5>(_original.address(), CompileAndRun);
	
	}

	void ConsoleAddon::CompileAndRun(RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name, RE::TESObjectREFR* a_targetRef)
	{
		auto cmd = a_script->GetCommand();
		if (PraseCmd(cmd) == PraseSuccess) return;
		_Ori_CompileAndRun(a_script, a_compiler, a_name, a_targetRef);
	}
}
