#pragma once
#include "TrashFunction.h"
#define FORPLAYERCTRL Trash_Function::Control::ForPlayerControl
#define PEX_NAME_PCCTRL "Trash_PlayerControl"sv

namespace Trash_Function::Control
{
	using namespace Trash_Function::Misc;
	#define FORPLAYERCTRL_PTR ForPlayerControl::GetSingleton()
	
	enum Ctrl_Flag
	{
		Flag_X = 0,
		Flag_Y = 1,
		Flag_X_AND_Y = 2,
		Flag_X_OR_Y = 3
	};
	
	enum Bit_Flag
	{
		MoveDirection_X =0,
		MoveDirection_Y =1,
		LookDirection_X =2,
		LookDirection_Y =3,
		LookMult_X = 4,
		LookMult_Y = 5,
		StopMove_X = 6,
		StopMove_Y = 7
	};

	enum Overwrite_Flag
	{
		Overwrite_MD_X = 0,
		Overwrite_MD_Y = 1,
		Overwrite_LM_X = 2,
		Overwrite_LM_Y = 3
	};

	class ForPlayerControl
	{
		ForPlayerControl(){};
		~ForPlayerControl(){};

		static void PCUpdate(RE::PlayerCharacter* pc, float a_delta);
		static inline REL::Relocation<decltype(PCUpdate)> _Original_PCUpdate;

		inline void ReverseMoveCtrl();
		inline void ReverseLookCtrl();
		inline void ApplyMultiplierToLookCtrl();
		inline void ApplyStopMoveCtrl();
		inline void ApplySetMoveCtrl();
		inline void ApplyLookMovementCtrl();
		//member
		std::bitset<8> ReverseFlag;  // |0|0|0|0|0|0|0|0 -> StopMove -Y | StopMove -X | LookMult -Y | LookMult -X | LookDirection -Y | LookDirection -X | MoveDirection -Y | MoveDirection -X
		float LM_Multiplier_X = 1.0;
		float LM_Multiplier_Y = 1.0;
		
		std::bitset<4> OverwriteFlag; // |0|0|0|0 -> Overwrite LM Y | Overwrite LM X | Overwrite MD Y | Overwrite MD X
		float MD_X_Overwrite = 0;
		float MD_Y_Overwrite = 0;

		float LM_X_Overwrite = 0;
		float LM_Y_Overwrite = 0;
	public:
// Overwrite Msg
		inline float GetMDOverwrite(const int& flag) const;
		inline float GetLMOverwrite(const int& flag) const;
// Flag Msg
		inline bool isDisableMoveX();
		inline bool isDisableMoveY();

		inline int isMoveDirectionX();
		inline int isMoveDirectionY();
		inline int isLookMovementX();
		inline int isLookMovementY();

// LookMovement
		inline bool IsLookMovementScale();
		inline bool isLookMovementScaleXY();
		inline bool isLookMovementScaleX();
		inline bool isLookMovementScaleY();

		inline void setLookMovementScaleXY(const float& flt_X, const float& flt_Y);
		inline void setLookMovementScaleX(const float& flt_X);
		inline void setLookMovementScaleY(const float& flt_Y);

		inline float getLookMovementScale(const int& flag) const;



// ReverseMove
		inline bool IsReverseMove();
		inline bool isReverseMoveXY();
		inline bool isReverseMoveX();
		inline bool isReverseMoveY();

		inline void setReverseMoveXY(const bool& _ReverseXY);
		inline void setReverseMoveX(const bool& _ReverseX);
		inline void setReverseMoveY(const bool& _ReverseY);
// ReverseLook
		inline bool IsReverseLook();
		inline bool isReverseLookXY();
		inline bool isReverseLookX();
		inline bool isReverseLookY();

		inline void setReverseLookXY(const bool& _val);
		inline void setReverseLookX(const bool& _val);
		inline void setReverseLookY(const bool& _val);

		
// SetMove
		inline void SetMoveOverwrite(const int& _Direction);
		inline void setMoveDirectionX(const int& _Direction);
		inline void setMoveDirectionY(const int& _Direction);

// SetLookMovement
		inline void SetLookMoveOverwrite(const float& _MovementAmount);
		inline void setLookMovementX(const float& _MovementAmount);
		inline void setLookMovementY(const float& _MovementAmount);

		inline void _setMoveDisableX(const bool& _DisableX);
		inline void _setMoveDisableY(const bool& _DisableY);
		inline void _setMoveDisableXY(const bool& _DisableY);
		inline void _SetAllReverse(const bool& _val);

		static ForPlayerControl* GetSingleton();
		ForPlayerControl(ForPlayerControl& another) = delete;
		ForPlayerControl(const ForPlayerControl& another) = delete;
		void operator=(const ForPlayerControl&) = delete;

		
		inline void Hook()
		{
			_Original_PCUpdate = REL::Relocation<uintptr_t>(RE::VTABLE_PlayerCharacter[0]).write_vfunc(0x0AD, PCUpdate);
		}

	};
}


namespace Trash_Function::Control
{
	ForPlayerControl* ForPlayerControl::GetSingleton()
	{
		static ForPlayerControl* fpc_Singleton = new ForPlayerControl();
		return fpc_Singleton;
	}

	inline float ForPlayerControl::GetMDOverwrite(const int& flag) const {
		switch (flag)
		{
		case Flag_X:return MD_X_Overwrite;
		case Flag_Y:return MD_Y_Overwrite;
		default:return MD_X_Overwrite + MD_Y_Overwrite;
		}
	}

	inline float ForPlayerControl::GetLMOverwrite(const int& flag) const {
		switch (flag)
		{
		case Flag_X:return LM_X_Overwrite;
		case Flag_Y:return LM_Y_Overwrite;
		default:return LM_X_Overwrite + LM_Y_Overwrite;
		}
	}
	//CheckMoveDisableFlag
	inline bool ForPlayerControl::isDisableMoveX() { return ReverseFlag.test(StopMove_X); }
	inline bool ForPlayerControl::isDisableMoveY() { return ReverseFlag.test(StopMove_Y); }
//SetMoveDisable
	inline void ForPlayerControl::_setMoveDisableX(const bool& _DisableX) { ReverseFlag.set(StopMove_X, _DisableX); }
	inline void ForPlayerControl::_setMoveDisableY(const bool& _DisableY) { ReverseFlag.set(StopMove_Y, _DisableY); }
	inline void ForPlayerControl::_setMoveDisableXY(const bool& _DisableY) 
	{ 
		ReverseFlag.set(StopMove_X, _DisableY);
		ReverseFlag.set(StopMove_Y, _DisableY); 
	}



//Check Flag - Look Scale
	inline bool ForPlayerControl::IsLookMovementScale() { return ReverseFlag.test(LookMult_X) || ReverseFlag.test(LookMult_Y); }
	inline bool ForPlayerControl::isLookMovementScaleXY() { return ReverseFlag.test(LookMult_X) && ReverseFlag.test(LookMult_Y); }
	inline bool ForPlayerControl::isLookMovementScaleX() { return ReverseFlag.test(LookMult_X); }
	inline bool ForPlayerControl::isLookMovementScaleY() { return ReverseFlag.test(LookMult_Y); }
//_setLookMovementScaleXY
	inline void ForPlayerControl::setLookMovementScaleXY(const float& flt_X, const float& flt_Y) { 
		setLookMovementScaleX(flt_X);
		setLookMovementScaleY(flt_Y);
	}
//_setLookMovementScaleX
	inline void ForPlayerControl::setLookMovementScaleX(const float& flt_X) {
		if (flt_X == 1.0) ReverseFlag.set(LookMult_X, false);
		else ReverseFlag.set(LookMult_X, true);
		LM_Multiplier_X = flt_X;
	}
//_setLookMovementScaleY
	inline void ForPlayerControl::setLookMovementScaleY(const float& flt_Y) {
		if (flt_Y == 1.0) ReverseFlag.set(LookMult_Y, false);
		else ReverseFlag.set(LookMult_Y, true);
		LM_Multiplier_Y = flt_Y;
	}
//_getLookMovementScaleX
	inline float ForPlayerControl::getLookMovementScale(const int& flag) const {
		switch (flag)
		{
		case Flag_X:return LM_Multiplier_X;
		case Flag_Y:return LM_Multiplier_Y;
		default:return LM_Multiplier_X * LM_Multiplier_Y;
		}
	}


//Check Flag - MOVE
	inline bool ForPlayerControl::IsReverseMove() { return ReverseFlag.test(MoveDirection_X) || ReverseFlag.test(MoveDirection_Y); }
	inline bool ForPlayerControl::isReverseMoveXY() { return ReverseFlag.test(MoveDirection_X) && ReverseFlag.test(MoveDirection_Y); }
	inline bool ForPlayerControl::isReverseMoveX() { return ReverseFlag.test(MoveDirection_X); }
	inline bool ForPlayerControl::isReverseMoveY() { return ReverseFlag.test(MoveDirection_Y); }

 //Check Flag - LOOK
	inline bool ForPlayerControl::IsReverseLook() { return ReverseFlag.test(LookDirection_X) || ReverseFlag.test(LookDirection_Y); }
	inline bool ForPlayerControl::isReverseLookXY() { return ReverseFlag.test(LookDirection_X) && ReverseFlag.test(LookDirection_Y); }
	inline bool ForPlayerControl::isReverseLookX() { return ReverseFlag.test(LookDirection_X); }
	inline bool ForPlayerControl::isReverseLookY() { return ReverseFlag.test(LookDirection_Y); }

//Set Flag - MOVE
	inline void ForPlayerControl::setReverseMoveXY(const bool& _ReverseXY) { 
		ReverseFlag.set(MoveDirection_X, _ReverseXY); 
		ReverseFlag.set(MoveDirection_Y, _ReverseXY); 
	}
	inline void ForPlayerControl::setReverseMoveX(const bool& _ReverseX) { ReverseFlag.set(MoveDirection_X, _ReverseX); }
	inline void ForPlayerControl::setReverseMoveY(const bool& _ReverseY) { ReverseFlag.set(MoveDirection_Y, _ReverseY); }
 //Set Flag - Look
	inline void ForPlayerControl::setReverseLookXY(const bool& _ReverseXY) { 
		ReverseFlag.set(LookDirection_X, _ReverseXY);
		ReverseFlag.set(LookDirection_Y, _ReverseXY); 
	}
	inline void ForPlayerControl::setReverseLookX(const bool& _ReverseX) { ReverseFlag.set(LookDirection_X, _ReverseX); }
	inline void ForPlayerControl::setReverseLookY(const bool& _ReverseY) { ReverseFlag.set(LookDirection_Y, _ReverseY); }
 //Set All Flag - Look & Move
	inline void ForPlayerControl::_SetAllReverse(const bool& _ReverseAll) {
		if (_ReverseAll) {
			ReverseFlag.set(MoveDirection_X, _ReverseAll);
			ReverseFlag.set(MoveDirection_Y, _ReverseAll);
			ReverseFlag.set(LookDirection_X, _ReverseAll);
			ReverseFlag.set(LookDirection_Y, _ReverseAll);
		}
		else ReverseFlag.reset();
	}
	
	inline int ForPlayerControl::isMoveDirectionX() { return MISC::sgn(RE::PlayerControls::GetSingleton()->data.moveInputVec.x); }
	inline int ForPlayerControl::isMoveDirectionY() { return MISC::sgn(RE::PlayerControls::GetSingleton()->data.moveInputVec.y); }

	inline int ForPlayerControl::isLookMovementX() { return MISC::sgn(RE::PlayerControls::GetSingleton()->data.lookInputVec.x); }
	inline int ForPlayerControl::isLookMovementY() { return MISC::sgn(RE::PlayerControls::GetSingleton()->data.lookInputVec.y); }

// Set Move
	

	inline void ForPlayerControl::setMoveDirectionX(const int& _Direction) {
		OverwriteFlag.set(Overwrite_MD_X, (_Direction == 0.0f? false : true) );
		MD_X_Overwrite = static_cast<float>(MISC::sgn(_Direction));
	}

	inline void ForPlayerControl::setMoveDirectionY(const int& _Direction){
		OverwriteFlag.set(Overwrite_MD_Y, (_Direction == 0.0f ? false : true) );
		MD_Y_Overwrite = static_cast<float>(MISC::sgn(_Direction));
	}

	inline void ForPlayerControl::SetMoveOverwrite(const int& _Direction) {
		setMoveDirectionX(_Direction);
		setMoveDirectionY(_Direction);
	}

// SetLookMovement
	inline void ForPlayerControl::setLookMovementX(const float& _MovementAmount) {
		OverwriteFlag.set(Overwrite_LM_X, (_MovementAmount == 0.0f ? false: true) );
		LM_X_Overwrite = _MovementAmount;
	}
	inline void ForPlayerControl::setLookMovementY(const float& _MovementAmount) {
		OverwriteFlag.set(Overwrite_LM_Y, (_MovementAmount == 0.0f ? false : true) );
		LM_Y_Overwrite = _MovementAmount;
	}
	inline void ForPlayerControl::SetLookMoveOverwrite(const float& _MovementAmount) {
		setLookMovementX(_MovementAmount);
		setLookMovementY(_MovementAmount);
	}

// Reverse Move Control
	inline void ForPlayerControl::ReverseMoveCtrl()
	{
		if(isReverseMoveX())RE::PlayerControls::GetSingleton()->data.moveInputVec.x = -(RE::PlayerControls::GetSingleton()->data.moveInputVec.x);
		if(isReverseMoveY())RE::PlayerControls::GetSingleton()->data.moveInputVec.y = -(RE::PlayerControls::GetSingleton()->data.moveInputVec.y);
	}
// Reverse Look Control
	inline void ForPlayerControl::ReverseLookCtrl()
	{
		if(isReverseLookX())RE::PlayerControls::GetSingleton()->data.lookInputVec.x = -(RE::PlayerControls::GetSingleton()->data.lookInputVec.x);
		if(isReverseLookY())RE::PlayerControls::GetSingleton()->data.lookInputVec.y = -(RE::PlayerControls::GetSingleton()->data.lookInputVec.y);
	}

// Apply Multiplier To Look Ctrl
	inline void ForPlayerControl::ApplyMultiplierToLookCtrl() 
	{
		if(isLookMovementScaleX())RE::PlayerControls::GetSingleton()->data.lookInputVec.x *= LM_Multiplier_X;
		if(isLookMovementScaleY())RE::PlayerControls::GetSingleton()->data.lookInputVec.y *= LM_Multiplier_Y;
	}

// ApplyStopMoveCtrl
	inline void ForPlayerControl::ApplyStopMoveCtrl() 
	{
		if(isDisableMoveX())RE::PlayerControls::GetSingleton()->data.moveInputVec.x = 0;
		if(isDisableMoveY())RE::PlayerControls::GetSingleton()->data.moveInputVec.y = 0;
	}

// ApplyMoveOverwrite
	inline void ForPlayerControl::ApplySetMoveCtrl() 
	{
		if(OverwriteFlag.test(Overwrite_MD_X)) RE::PlayerControls::GetSingleton()->data.moveInputVec.x = MD_X_Overwrite;
		if(OverwriteFlag.test(Overwrite_MD_Y)) RE::PlayerControls::GetSingleton()->data.moveInputVec.y = MD_Y_Overwrite;
	}

// ApplyLookMovementOverwrite
	inline void ForPlayerControl::ApplyLookMovementCtrl() 
	{
		if(OverwriteFlag.test(Overwrite_LM_X)) RE::PlayerControls::GetSingleton()->data.lookInputVec.x = LM_X_Overwrite;
		if(OverwriteFlag.test(Overwrite_LM_Y)) RE::PlayerControls::GetSingleton()->data.lookInputVec.y = LM_Y_Overwrite;
	}

// FUNCTION HOOKED
	void ForPlayerControl::PCUpdate(RE::PlayerCharacter* pc, float a_delta)
	{
		FORPLAYERCTRL_PTR->ApplySetMoveCtrl();
		FORPLAYERCTRL_PTR->ApplyLookMovementCtrl();
		FORPLAYERCTRL_PTR->ReverseMoveCtrl();
		FORPLAYERCTRL_PTR->ReverseLookCtrl();
		FORPLAYERCTRL_PTR->ApplyStopMoveCtrl();
		FORPLAYERCTRL_PTR->ApplyMultiplierToLookCtrl();
		FORPLAYERCTRL_PTR->_Original_PCUpdate(pc, a_delta);
	}
	//GetMoveOverwrite
	inline float GetMoveOverwrite(const int& flag)
	{
		return FORPLAYERCTRL_PTR->GetMDOverwrite(flag);
	}
	//GetLookMoveOverwrite
	inline float GetLookMoveOverwrite(const int& flag)
	{
		return FORPLAYERCTRL_PTR->GetLMOverwrite(flag);
	}

	//LookMovementScale
	inline void LookMovementScale(const float& a_scale, const int& flag)
	{
		switch (flag) {
		case Flag_X:FORPLAYERCTRL_PTR->setLookMovementScaleX(a_scale); break;
		case Flag_Y:FORPLAYERCTRL_PTR->setLookMovementScaleY(a_scale); break;
		case Flag_X_AND_Y:FORPLAYERCTRL_PTR->setLookMovementScaleXY(a_scale, a_scale); break;
		default: FORPLAYERCTRL_PTR->setLookMovementScaleXY(a_scale, a_scale); break;
		}
	}
	//GetLookMovementScale
	inline float GetLookMovementScale(const int& flag)
	{
		return FORPLAYERCTRL_PTR->getLookMovementScale(flag);
	}
	//IsLookMovementScale
	inline bool IsLookMovementScale(const int& flag)
	{
		switch (flag) {
		case Flag_X: return FORPLAYERCTRL_PTR->isLookMovementScaleX();
		case Flag_Y: return FORPLAYERCTRL_PTR->isLookMovementScaleY();
		case Flag_X_AND_Y: return FORPLAYERCTRL_PTR->isLookMovementScaleXY();
		case Flag_X_OR_Y: return FORPLAYERCTRL_PTR->IsLookMovementScale();
		default: return FORPLAYERCTRL_PTR->IsLookMovementScale();
		}
	}
	//IsReverseMove
	inline bool IsReverseMove(const int& flag)
	{
		switch (flag) {
		case Flag_X: return FORPLAYERCTRL_PTR->isReverseMoveX();
		case Flag_Y: return FORPLAYERCTRL_PTR->isReverseMoveY();
		case Flag_X_AND_Y: return FORPLAYERCTRL_PTR->isReverseMoveXY();
		case Flag_X_OR_Y: return FORPLAYERCTRL_PTR->IsReverseMove();
		default: return FORPLAYERCTRL_PTR->IsReverseMove();
		}
	}
	//IsReverseLook
	inline bool IsReverseLook(const int& flag) {
		switch (flag) {
		case Flag_X: return FORPLAYERCTRL_PTR->isReverseLookX();
		case Flag_Y: return FORPLAYERCTRL_PTR->isReverseLookY();
		case Flag_X_AND_Y: return FORPLAYERCTRL_PTR->isReverseLookXY();
		case Flag_X_OR_Y: return FORPLAYERCTRL_PTR->IsReverseMove();
		default: return FORPLAYERCTRL_PTR->IsReverseLook();
		}
	}
	//SetMoveDisable
	inline void SetMoveDisabled(const bool& _Disabled, const int& flag) {
		switch (flag) {
		case Flag_X: return FORPLAYERCTRL_PTR->_setMoveDisableX(_Disabled);
		case Flag_Y: return FORPLAYERCTRL_PTR->_setMoveDisableY(_Disabled);
		case Flag_X_AND_Y: return FORPLAYERCTRL_PTR->_setMoveDisableXY(_Disabled);
		default: return FORPLAYERCTRL_PTR->_setMoveDisableXY(_Disabled);
		}
	}

	//SetMoveDirection
	inline void SetMoveOverwrite(const int& _direction, const int& flag ) {
		switch (flag) {
		case Flag_X: FORPLAYERCTRL_PTR->setMoveDirectionX(_direction); return;
		case Flag_Y: FORPLAYERCTRL_PTR->setMoveDirectionY(_direction); return;
		case Flag_X_AND_Y: FORPLAYERCTRL_PTR->SetMoveOverwrite(_direction); return;
		default: FORPLAYERCTRL_PTR->SetMoveOverwrite(_direction); return;
		}
	}
	//SetLookMovement
	inline void SetLookMoveOverwrite(const float& _movement, const int& flag) {
		switch (flag) {
		case Flag_X: FORPLAYERCTRL_PTR->setLookMovementX(_movement); return;
		case Flag_Y: FORPLAYERCTRL_PTR->setLookMovementY(_movement); return;
		case Flag_X_AND_Y: FORPLAYERCTRL_PTR->SetLookMoveOverwrite(_movement); return;
		default: FORPLAYERCTRL_PTR->SetLookMoveOverwrite(_movement); return;
		}
	}
	//GetMoveDirection
	inline int GetMoveDirection(const int& flag) {
		switch (flag) {
		case Flag_X: return FORPLAYERCTRL_PTR->isMoveDirectionX();
		case Flag_Y: return FORPLAYERCTRL_PTR->isMoveDirectionY();
		default: return (FORPLAYERCTRL_PTR->isMoveDirectionX() + FORPLAYERCTRL_PTR->isMoveDirectionY());
		}
	}
	//GetLookMovementDirection
	inline int GetLookMoveDirection(const int& flag ) {
		switch (flag) {
		case Flag_X: return FORPLAYERCTRL_PTR->isLookMovementX();
		case Flag_Y: return FORPLAYERCTRL_PTR->isLookMovementY();
		default: return (FORPLAYERCTRL_PTR->isLookMovementX() + FORPLAYERCTRL_PTR->isLookMovementY());
		}
	}
	//IsMoveDisable
	inline bool IsMoveDisabled(const int& flag) {
		switch (flag) {
		case Flag_X: return FORPLAYERCTRL_PTR->isDisableMoveX();
		case Flag_Y: return FORPLAYERCTRL_PTR->isDisableMoveY();
		default: return (FORPLAYERCTRL_PTR->isDisableMoveX() + FORPLAYERCTRL_PTR->isDisableMoveY());
		}
	}
	//ReverseMove
	inline void ReverseMove(const bool& _Reverse, const int& flag) {
		switch (flag) {
		case Flag_X: FORPLAYERCTRL_PTR->setReverseMoveX(_Reverse); return;
		case Flag_Y: FORPLAYERCTRL_PTR->setReverseMoveY(_Reverse); return;
		case Flag_X_AND_Y: FORPLAYERCTRL_PTR->setReverseMoveXY(_Reverse); return;
		default: FORPLAYERCTRL_PTR->setReverseMoveXY(_Reverse); return;
		}
	}
	//ReverseLook
	inline void ReverseLook(const bool& _Reverse, const int& flag)
	{
		switch (flag) {
		case Flag_X: FORPLAYERCTRL_PTR->setReverseLookX(_Reverse); return;
		case Flag_Y: FORPLAYERCTRL_PTR->setReverseLookY(_Reverse); return;
		case Flag_X_AND_Y: FORPLAYERCTRL_PTR->setReverseLookXY(_Reverse); return;
		default: FORPLAYERCTRL_PTR->setReverseLookXY(_Reverse); return;
		}
	}
	//SetAllReverse
	inline void SetAllReverse(const bool& _ReverseAll)
	{
		FORPLAYERCTRL_PTR->_SetAllReverse(_ReverseAll);
	}


	inline void RecoverCtrl()
	{
		SetMoveOverwrite(0,-1);
		SetLookMoveOverwrite(0.0f,-1);
		SetAllReverse(false);
		SetMoveDisabled(false, 2);
		LookMovementScale(1.0f, 2);
	}

// GetLookMoveOverwrite
	inline float Papyrus_GetLookMoveOverwrite(RE::StaticFunctionTag*, int flag = -1) {
		return GetLookMoveOverwrite(flag);
	}

// GetMoveOverwrite
	inline int Papyrus_GetMoveOverwrite(RE::StaticFunctionTag*, int flag = -1) {
		return static_cast<int>(GetMoveOverwrite(flag));
	}

// LookMovementScale
	inline void Papyrus_LookMovementScale(RE::StaticFunctionTag*, float a_scale = 1.0f,int flag = -1) {
		LookMovementScale(a_scale, flag);
	}

// isLookMovementScale
	inline bool Papyrus_isLookMovementScale(RE::StaticFunctionTag*, int flag = -1) {
		return IsLookMovementScale(flag);
	}

// GetLookMovementScale
	inline float Papyrus_getLookMovementScale(RE::StaticFunctionTag*, int flag = -1) {
		return GetLookMovementScale(flag);
	}

// isReverseMove
	inline bool Papyrus_isReverseMove(RE::StaticFunctionTag*,int flag = -1) {
		return IsReverseMove(flag);
	}
// isReverseLook
	inline bool Papyrus_isReverseLook(RE::StaticFunctionTag*,int flag = -1) {
		return IsReverseMove(flag);
	}
// SetMoveDirection
	inline void Papyrus_setMoveOverwrite(RE::StaticFunctionTag*,int _direction = 0,int flag = -1) {
		SetMoveOverwrite(_direction, flag);
	}
// SetLookMovement
	inline void Papyrus_setLookMoveOverwrite(RE::StaticFunctionTag*,float _movement = 0.0f,int flag = -1) {
		SetLookMoveOverwrite(_movement, flag);
	}
// getMoveDirection
	inline int Papyrus_getMoveDirection(RE::StaticFunctionTag*,int flag = -1) {
		return GetMoveDirection(flag);
	}

// getLookDirection
	inline int Papyrus_getLookMovementDirection(RE::StaticFunctionTag*,int flag = -1) {
		return GetLookMoveDirection(flag);
	}

// isMoveDisabled
	inline bool Papyrus_isMoveDisable(RE::StaticFunctionTag*,int flag = -1) {
		return IsMoveDisabled(flag);
	}
// setMoveDisabled
	inline void Papyrus_setMoveDisable(RE::StaticFunctionTag*, bool _Disabled = false, int flag = -1) {
		SetMoveDisabled(_Disabled, flag);
	}
// ReverseMove
	inline void Papyrus_ReverseMove(RE::StaticFunctionTag*,bool _Reverse = false, int flag = -1) {
		return ReverseMove(_Reverse,flag);
	}

// ReverseLook
	inline void Papyrus_ReverseLook(RE::StaticFunctionTag*,bool _Reverse = false, int flag = -1) {
		return ReverseLook(_Reverse, flag);
	}

// SetAllReverse
	inline void Papyrus_SetAllReverse(RE::StaticFunctionTag*, bool _ReverseAll = false) { SetAllReverse(_ReverseAll); }

	inline bool binding(VM* _vm)
	{
		_vm->RegisterFunction("IsReverseMove"sv, PEX_NAME_PCCTRL, Papyrus_isReverseMove);
		_vm->RegisterFunction("IsReverseLook"sv, PEX_NAME_PCCTRL, Papyrus_isReverseLook);
		_vm->RegisterFunction("IsMoveDisabled"sv, PEX_NAME_PCCTRL, Papyrus_isMoveDisable);
		_vm->RegisterFunction("SetMoveDisabled"sv, PEX_NAME_PCCTRL, Papyrus_setMoveDisable);
		_vm->RegisterFunction("IsLookMoveScale"sv, PEX_NAME_PCCTRL, Papyrus_isLookMovementScale);
		_vm->RegisterFunction("GetLookMoveScale"sv, PEX_NAME_PCCTRL, Papyrus_getLookMovementScale);
		_vm->RegisterFunction("LookMoveScale"sv, PEX_NAME_PCCTRL, Papyrus_LookMovementScale);

		_vm->RegisterFunction("SetMoveOverwrite"sv, PEX_NAME_PCCTRL, Papyrus_setMoveOverwrite);  // test
		_vm->RegisterFunction("SetLookMoveOverwrite"sv, PEX_NAME_PCCTRL, Papyrus_setLookMoveOverwrite);    // test
		_vm->RegisterFunction("GetMoveOverwrite"sv, PEX_NAME_PCCTRL, Papyrus_GetMoveOverwrite);
		_vm->RegisterFunction("GetLookMoveOverwrite"sv, PEX_NAME_PCCTRL, Papyrus_GetLookMoveOverwrite);

		

		_vm->RegisterFunction("GetMoveDirection"sv, PEX_NAME_PCCTRL, Papyrus_getMoveDirection);
		_vm->RegisterFunction("GetLookMoveDirection"sv, PEX_NAME_PCCTRL, Papyrus_getLookMovementDirection);


		_vm->RegisterFunction("ReverseMove"sv, PEX_NAME_PCCTRL, Papyrus_ReverseMove);
		_vm->RegisterFunction("ReverseLook"sv, PEX_NAME_PCCTRL, Papyrus_ReverseLook);

		_vm->RegisterFunction("SetAllReverse"sv, PEX_NAME_PCCTRL, Papyrus_SetAllReverse);


		return true;
	}
}
