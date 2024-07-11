#pragma once
// Use to reflect function id

namespace ReflectionFuncID
{
	#define _MaxCount_ RE::FUNCTION_DATA::FunctionID::kTotal
	#define _Int_MaxCount_ static_cast<int>(RE::FUNCTION_DATA::FunctionID::kTotal)
	using FuncID = RE::FUNCTION_DATA::FunctionID;
	using FuncName = std::string;
	
	template <int N>
	struct simple_integral_constant
	{
		static constexpr int value = N;
	};

	namespace detail
	{
		template <int Begin = 0, int End = _Int_MaxCount_, class F>
		void static_for(const F& func)
		{
			if constexpr (Begin == End) return;
			else {
				func(simple_integral_constant<Begin>());
				static_for<Begin + 1, End>(func);
			}
		}

		template <class T, T N>
		FuncName get_func_name()
		{
			FuncName s = __FUNCSIG__;
			if (s.empty()) return "";
			auto pos_start = s.find_last_of("::");
			pos_start += 1;
			auto pos_end = s.find_first_of('>', pos_start);
			if (auto sub_s = s.substr(pos_start, pos_end - pos_start);sub_s.empty()) return sub_s;

			return "";
		}
	}


	template <class T>
	FuncName get_func_name_dynamic(T n)
	{
		FuncName ret;
		detail::static_for< 0, _Int_MaxCount_ >([&](auto _const) {
			if (n == (T)_const.value)
				ret = detail::get_func_name<T, (T)_const.value>();
		});
		return ret;
	}

	template <class T, int Begin = 0, int End = _Int_MaxCount_ >
	T get_id_from_func(const FuncName& s)
	{
		for (int i = Begin; i < End; i++) {
			if (s == get_func_name_dynamic((T)i)) {
				return (T)i;
			}
		}
		return (T)_MaxCount_
	}

	FuncName GetFunctionName(int func_id) {
		if (func_id < 0 || func_id > _Int_MaxCount_ ) return "";
		return get_func_name_dynamic<FuncID>((FuncID)func_id);
	}

	FuncID GetFunctionID(const FuncName& func_name) {
		return get_id_from_func<FuncID>(func_name);
	}

	class FuncMap
	{
	private:
		FuncMap();
		~FuncMap(){};
		std::map<FuncName,FuncID> Func_Map;

	public:
		FuncMap(FuncMap& another) = delete;
		FuncMap(const FuncMap& another) = delete;
		void operator=(const FuncMap&) = delete;

		static FuncMap* GetSingleton();
		FuncID GetFunctionID(FuncName& _funcName);
	};

	FuncMap* FuncMap::GetSingleton() { 
		static FuncMap* fm_Singleton = new FuncMap();
		return fm_Singleton;
	}

	inline FuncID FuncMap::GetFunctionID(FuncName& _funcName) { 
		if (_funcName.empty()) return _MaxCount_;
		if (_funcName[0] != 'k') _funcName.insert(_funcName.begin(),'k');
		if (auto iter = Func_Map.find(_funcName); iter != Func_Map.end()) return iter->second;
		return _MaxCount_;
	}

	FuncMap::FuncMap() { 
		for (int i = 0; i < _Int_MaxCount_ ; i++) {
			FuncID _funcID = (FuncID)i;
			FuncName _funcName = get_func_name_dynamic<FuncID>(_funcID);
			Func_Map.insert(std::make_pair(_funcName, _funcID));
		}
	}

	
}

namespace FuncInfoGetter
{
	using namespace ReflectionFuncID;
	using FuncID = RE::FUNCTION_DATA::FunctionID;
	using FuncName = std::string;
	using OpCodeStr = std::string;
	using FuncOpCode = RE::CONDITION_ITEM_DATA::OpCode;

	enum class OpCode : std::uint8_t
	{
		kEqualTo,               // ==
		kNotEqualTo,            // !=
		kGreaterThan,           // >
		kGreaterThanOrEqualTo,  // >=
		kLessThan,              // <
		kLessThanOrEqualTo,     // <=
	};

	enum GetterResult
	{
		SuccessGet = 0,
		Info = 1
	};

	inline std::tuple<bool, FuncID> GetFunctionID(FuncName& _funcName) {
		FuncID ret = FuncMap::GetSingleton()->GetFunctionID(_funcName);
		if (ret == _MaxCount_ ) return {false,ret};
		return { true, ret };
	}

	inline std::tuple<bool, FuncOpCode> GetFunctionOpCode(const OpCodeStr& _op_code)
	{
		if (_op_code.length()==0 || _op_code.length() > 2) return {false,(FuncOpCode)0};

		if (_op_code.length()==1) {
			switch (_op_code.at(0)) {
			case '>':return {true,FuncOpCode::kGreaterThan};
			case '<':return {true,FuncOpCode::kLessThan};
			default :return {false,(FuncOpCode)0};
			}
		}

		if (_op_code.at(1) == '=') {

			switch (_op_code.at(0)) {
				case '=': return {true,FuncOpCode::kEqualTo};

				case '!': return {true,FuncOpCode::kNotEqualTo};

				case '>': return {true,FuncOpCode::kGreaterThanOrEqualTo};

				case '<': return {true,FuncOpCode::kLessThanOrEqualTo};
			default:
				break;
			}
		}
		return { false, (FuncOpCode)0 };
	}
}
