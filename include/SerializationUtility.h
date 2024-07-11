#pragma once
#include "TrashSerialization.h"

namespace TrashSerialization::Utility
{
	inline bool ReadString(SKSE::SerializationInterface* _interface, std::string& _str)
	{
		std::size_t size = 0;
		logs::info("Reading string size...");
		if (!_interface->ReadRecordData(size)) {
			return false;
		}
		logs::info("string size = {}", size);
		_str.reserve(size);
		auto c_str = _str.data();
		if (!_interface->ReadRecordData(_str.data(), static_cast<std::uint32_t>(size))) {
			return false;
		}
		logs::info("Reading a string - \"{}\", string size = {}", c_str, size);
		return true;
	}

	inline bool WriteString(SKSE::SerializationInterface* _interface, const std::string& _str)
	{
		std::size_t size = _str.length() + 1;
		logs::info("Writing a string - \"{}\", string size = {}",_str,size);
		return _interface->WriteRecordData(size) && _interface->WriteRecordData(_str.data(), static_cast<std::uint32_t>(size));
	}

	inline bool SaveFormID(SKSE::SerializationInterface* _interface, const RE::TESForm* _form) {
		RE::FormID _fid = (_form ? _form->GetFormID() : 0 );
		if (!_interface->WriteRecordData(_fid)) {
			logs::error("Failed to save formID ({:x})", _form->GetFormID() );
			return false;
		}
		return true;
	}
	
	inline bool ReadFormID(SKSE::SerializationInterface* a_intfc, RE::FormID& a_formID)
	{
		a_intfc->ReadRecordData(a_formID);
		if (a_formID != 0) {
			return a_intfc->ResolveFormID(a_formID, a_formID);
		}
		return true;
	}

}
