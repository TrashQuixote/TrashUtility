# 介绍

**TrashUtility**是基于CommonLibSSE-NG构建的插件，旨在为《上古卷轴5：天际 特别版》提供新的papyrus函数。

下载：[N网页面](https://www.nexusmods.com/skyrimspecialedition/mods/124076)

# 致谢

[SKSE Team](https://skse.silverlock.org/)；
[People who contribute to commonlib-ng](https://github.com/CharmedBaryon/CommonLibSSE-NG).
[JIP that inspired me a lot](https://github.com/jazzisparis/JIP-LN-NVSE).
[meh321 for Address Library](https://www.nexusmods.com/skyrimspecialedition/mods/32444?).
[Fenix31415 for his patience and guidance](https://github.com/fenix31415).
[Qudix for ng template](https://github.com/qudix/commonlibsse-ng-template).
[ZhangYueZhi制作中文文档](https://next.nexusmods.com/profile/ZhangYueZhi/about-me).
# 函数

## 覆盖玩家和玩家镜头的运动

TrashUtility提供了一系列函数来覆盖玩家运动 `player movement` 和玩家镜头运动 `player camera movement`.  
**1.** 将玩家控制的左右运动视为沿着X轴的运动，将前后移动视为沿着Y轴的运动.  
**2.** 玩家镜头的运动同理，左右对应沿X轴，上下对应沿Y轴.  

使用 `flag` 变量来表示要在哪个方向上覆盖运动.  

| 方向   | Flag |
| :----- | :--- |
| X      | 0    |
| Y      | 1    |
| X 和 Y | 2    |
| X 或 Y | 3    |

**方便起见**，我使用如下格式来解释 `flag` 的各个参数：

```papyrus
; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | flag =3 -X||Y | other -X||Y
```

为了使用以下函数，你需要在脚本中使用 Import 引入：

```papyrus
Import Trash_PlayerControl
```

```papyrus
; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X & Y
; 若**_Reverse **的值为真，反转玩家镜头的运动方向
Function ReverseLook( bool _Reverse = False, int flag = -1) global native.

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X & Y
; 若**_Reverse **的值为真，反转玩家的运动方向
Function ReverseMove( bool _Reverse = False, int flag = -1) global native.

; 若**_Reverse **的值为真，同时反转玩家的运动方向和玩家镜头的运动方向
Function SetAllReverse(bool _ReverseAll = False) global native.

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | flag =3 -X||Y | other -X||Y
bool Function IsReverseMove(int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | flag =3 -X||Y | other -X||Y
bool Function IsReverseLook(int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | other = -X & Y
; Disable player movement direction when **_Disabled ** is true
Function SetMoveDisabled(bool _Disabled = False,int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | other = -X || Y 
bool Function IsMoveDisabled(int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X & Y
; The effect of this function is similar to sensitivity adjustment, but essentially it affects the speed of player camera movement.
; example : When a_scale = 0.5, the  speed of player camera movement is half of the original.
Function LookMoveScale( float a_scale = 1.0,int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | other =2 -X & Y
; Return the current scale which applied to the player camera movement speed.
float Function GetLookMoveScale(int flag = -1) Global Native

; |flag = 0 -X | flag =1 -Y | other -X & Y
; Returns true when player moving in a specific direction.
int Function GetMoveDirection(int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X & Y
; Returns true when player camera moving in a specific direction.
int Function GetLookMoveDirection(int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X & Y
; This function will not change speed of player movement
; _direction == 0 it will not apply any overwrite
Function SetMoveOverwrite(int _direction=0 ,int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | other -X || Y
; Return the current direction which applied to player
int Function GetMoveOverwrite(int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X & Y
; Will moves the player along a specific axis.
; _direction == 0 it will not apply any overwrite
Function SetMoveOverwrite(int _direction=0 ,int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X||Y
; Will moves the player camera along a specific axis.
; movement == 0 it will not apply any overwrite
Function SetLookMoveOverwrite(float _movement = 0.0,int flag = -1) global native

; |flag = 0 -X | flag =1 -Y | other -X||Y
; Return the current movement which Overwrite to the player camera movement speed.
float Function GetLookMoveOverwrite(int flag = -1) global native   
```

**例**

```papyrus
scriptName RandomScript extends activemagiceffect
Import Trash_PlayerControl

Event OnEffectStart(Actor akTarget, Actor akCaster)
    if akTarget == game.getplayer()
        ReverseLook(true)    ; Reverse player camera movement direction
    endif
EndEvent

Event OnEffectFinish(Actor akTarget, Actor akCaster)
    if akTarget == game.getplayer()
        ReverseLook()    ; Recover 
    endif
EndEvent
```

## 辅助容器（Auxiliary Collection）

有两种不同类型的容器 `Auxiliary Array` 和 `Ref Map`.  
辅助容器的启发来自JIP NVSE插件，其性质和 JIP NVSE 插件的 auxiliary variable interface 类似.  
（下文部分引用自：[https://geckwiki.com/index.php?title=Category:Auxiliary-Variable_Functions_(JIP)](https://geckwiki.com/index.php?title=Category:Auxiliary-Variable_Functions_(JIP))，但有所区别。）.  

### 性质

**1.** 一个 `Auxiliary Collection` 由 `Name (string)` 和它的“所有者” `Object (object reference/base form)` 定义。可以理解为键值对或映射 Object : Name 所以必须是唯一的.  

注意，**不同的Object** 可以 “拥有” 同样名字的Collection，**同一个Object** 也可以 “拥有” 类型不同但名称（Name)相同的 容器，这两种情况不违反组合的独特性。一个 Object 可以 “拥有” 任意数量的容器.  
**2.** `Auxiliary Collection` 可以存储的值有float，reference/form。它们的类型可以根据赋值的类型动态改变.  

### 生命周期(Duration)

一个 `Auxiliary Collection` 的生命周期，即它是永久（permanent）的（随存档存储）还是临时（temporary）的.  

#### 永久容器

永久容器的元素会写入存档.  

#### 临时容器

临时容器不会写入存档，其只存在于当前的游戏会话，在退出游戏时就被移除.  

为容器名称添加前缀来决定其 `duration`：

| Prefix | Duration  |
| :----- | :-------- |
| *      | temporary |
| 无前缀 | permanent |

**例：**_"*MyAuxColl"_ 命名了一个 `temporary` 的collection.  

### 可读取性

`Auxiliary Collection` 更像一个全局容器，一经创建即可被所有脚本读取.  

### 与JIP NVSE插件的auxiliary variable interface和ref map的区别

**1.** `Auxiliary Collection` 的名字是区分大小写的（我必须承认我疏忽了）。不过在使用时你应当确保使用独特的命名.  
**2.** 没有可读取性设定.  
**3.** 缺少遍历 refmap的简易方法.  
**4.** 目前不支持string类型的元素.  

### 辅助容器 相关函数

如下所示，调用这些函数需要你明确地给出 `Holder(Reference\Form)` 和 `Collection Name`.  
你不需要手动构建 `Auxiliary Collection`，它会在你使用任意setter函数时被自动创建.  

为了使用以下函数，你需要在脚本中使用 Import 引入：

```papyrus
Import Trash_Collection
```

#### Auxiliary Array

```papyrus
Bool function AuxArrayEmpty(Form Holder, String CollectionName) global native

; Specifies an index to erase the element otherwise erase all element.
Bool function AuxArrayErase(Form Holder, String CollectionName, int _index = -1) global native

; Specifies an index to get the type of the element.
; float : return 1 | reference/form : return 2.
int function AuxArrayGetType(Form Holder, String CollectionName, int _index = 0) global native

; Specifies an index to set the float element.By default, it is written to index 0
; If _index == -1, will append the element to the end of array
Bool function AuxArraySetFloat(Form Holder, String CollectionName, Float _flt, Int _index = 0) global native

; Specifies an index to get the float element.By default, it gets element from index 0
; If the value type is not float, it will return 0.0
Float function AuxArrayGetFloat(Form Holder, String CollectionName, Int _index = 0) global native

; Specifies an index to set the reference/form element.By default, it is written to index 0
; If _index == -1, will append the element to the end of array
Bool function AuxArraySetRef(Form Holder, String CollectionName, Form _form, Int _index = 0) global native

; Specifies an index to get the reference/form element.By default, it gets element from index 0
; If the value type is not reference/form, it will return None
Form function AuxArrayGetRef(Form Holder, String CollectionName, Int _index = 0) global native

; Get all the reference/form element form auxiliary array
; Will return a empty array if specific auxiliary collection not exist
form[] function AuxArrayGetRefArray(Form Holder, String CollectionName) global native

; Get all the float element form auxiliary array
; Will return a empty array if specific auxiliary collection not exist
Float[] function AuxArrayGetFloatArray(Form Holder, String CollectionName) global native

;The function below use to fill auxiliary array with an array or formlist
;Note that they will clear the auxiliary array at first(if auxiliary array already exist) and return the new size of auxiliary array.

Int function AuxArraySetFromFloatArray(Form Holder, String CollectionName, Float[] floatArr) global native

Int function AuxArraySetFromRefArray(Form Holder, String CollectionName, form[] FormArr) global native

Int function AuxArrayGetSize(Form Holder, String CollectionName) global native

Int function AuxArraySetFromFormList(Form Holder, String CollectionName, FormList _formlist) global native
```

#### Reference Map

Map是一个关联容器，这里的ref map使用 `reference/form`作为key value.  
RefMap内部的元素按FormID升序排列.  

```papyrus
;Gets the value type of the element with a Key. If no such collection exists, no such Key exists in collection or value type is not float, return 0.
; float : return 1 | reference/form : return 2.
int function MapGetValueType(Form Holder, String CollectionName, Form _key) global native

;Set the float value of the element with a Key. If no such Key exists in collection, it is created.
Bool function RefMapSetFloat(Form Holder, String CollectionName, Form _key, Float _elem) global native

;Gets the float value of the element with a Key. If no such collection exists, no such Key exists in collection or value type is not float, return 0.0.
Float function RefMapGetFloat(Form Holder, String CollectionName, Form _key) global native

;Set the reference/form value of the element with a Key. If no such Key exists in collection, it is created.
Bool function RefMapSetRef(Form Holder, String CollectionName, Form _key, Form _elem) global native

;Gets the float value of the element with a Key. If no such collection exists, no such Key exists in collection or value type is not reference/form, return None.
Form function RefMapGetRef(Form Holder, String CollectionName, Form _key) global native

;Get all reference/form elements from ref map
form[] function RefMapGetRefValues(Form Holder, String CollectionName) global native

Int function RefMapGetSize(Form Holder, String CollectionName) global native

;Get all key values from ref map
form[] function RefMapGetKeys(Form Holder, String CollectionName) global native

Bool function RefMapEraseAll(Form Holder, String CollectionName) global native

;Remove specific key and its value
Bool function RefMapErase(Form Holder, String CollectionName, Form _key) global native

;Get all float values from ref map
Float[] function RefMapGetFloatValues(Form Holder, String CollectionName) global native

Bool function RefMapClear(Form Holder, String CollectionName) global native

;Erase all invalid key
Bool function RefMapEraseInvalidKey(Form Holder, String CollectionName) global native
```

#### 其它

```papyrus

; Destroy collection and release memory
bool function DestroyAuxArr(Form Holder, String CollectionName) global native 
bool function DestroyRefMap(Form Holder, String CollectionName) global native 

; Functions below for debug. Will dump auxiliary to console
; CollectionType: 0 = AuxArr, 1 = RefMap


function DumpCollection(Form Holder, String CollectionName, int CollectionType) global native
function DumpAllAuxArr() global native
function DumpAllRefMap() global native
function DumpAllCollection() global native
```

## 杂项函数

为了使用以下函数，你需要在脚本中使用 Import 引入：

```papyrus
Import Trash_Function
```

### 距离

```papyrus
;Same as original getdistance function, Get 3D Distance between two reference
;Will return a the maximum of float if two reference not in the same interior or worldspace
float Function GetDistance3D(ObjectReference A_ref, ObjectReference B_ref) Global Native

;Get 3D Distance between a reference and given coordinate
float Function GetDistanceFromPoint3D(ObjectReference _ref, float x, float y, float z) Global Native

;Get 3D Distance between two given coordinate
float Function GetDistanceBetweenPoints3D(float A_x, float A_y, float A_z, float B_x, float B_y, float B_z) Global Native

;they do the same thing but doesn't care about the Z axis
float Function GetDistance2D(ObjectReference A_ref, ObjectReference B_ref) Global Native
float Function GetDistanceFromPoint2D(ObjectReference _ref,float x, float y) Global Native
float Function GetDistanceBetweenPoints2D(float A_x, float A_y, float B_x, float B_y) Global Native

; return true only when two reference in the same interior or worldspace
bool Function GetInSameCellOrWorldSpace(ObjectReference A_ref, ObjectReference B_ref) Global Native
```

### 位置

```papyrus
;Get reference current position as a float array. index 0,1,2 correspond x,y,z respectively 
float[] Function GetPosAsArray(ObjectReference _obj) Global Native

;Set reference position to (x,y,z). 
;When transform==true will do a transform base on reference current position( Set reference position to (current position) + (x,y,z).)
Function SetPosAlt(ObjectReference _obj,float x,float y,float z,bool transform = False) Global Native
;Do the same thing but read index 0,1,2 from PosArr as x,y,z
Function SetPosFromArray(ObjectReference _obj, float[] PosArr,bool transform = False) Global Native
```

### 朝向角

查看原生函数：[https://ck.uesp.net/wiki/GetHeadingAngle](https://ck.uesp.net/wiki/GetHeadingAngle)

```papyrus
;Same as original GetHeadingAngle function but GetHeadingAngle between a reference and given coordinate
float Function GetHeadingPointAngle(ObjectReference _obj,float x,float y,float z,bool a_abs = false) Global Native

;GetHeadingAngle between two given coordinate, Note that you need to input a specific angle z
float Function GetHeadingAngleBetweenPoints(float A_x, float A_y, float A_z, float ang_z, float B_x, float B_y,float B_z, bool a_abs = False) Global Native

;Functions below are different from Original GetHeadingAngle function or above function,they returns the X angle between reference/coordinate
float Function GetHeadingAngleX(ObjectReference fst_ref, ObjectReference sec_ref,  bool a_abs = false) Global Native

float Function GetHeadingPointAngleX(ObjectReference _ref,float _x, float _y, float _z, bool a_abs = false) Global Native

float Function GetHeadingAngleBetweenPointsX(float A_x, float A_y, float A_z, float ang_x, float B_x, float B_y,float B_z, bool a_abs = False) Global Native
```

### 声音

```papyrus
;Just play sound in specific coordinate
;_Sound only accept the SoundDescriptor(SNDR) as input
bool Function PlaySoundAtPoint (Form _Sound,float x,float y,float z) Global Native
```

### 对群体施法

输入 `_Caster` 和 `_Spell(Spell,Enchantment,AlchemyItem,Scroll,Ingredient)`.  
若 `_CastToCaster` == false，即便施法者 `_Caster` 位于施法半径 `_Radiu` 内，`_Spell` 也不会被施法给 `_Caster`.  
若 `_Center` = None，默认以 `_Caster` 为施法中心，立即对施法半径 `_Radiu` 内的目标施法 `_Spell`.  

```papyrus
Form[] Function InstantCastToActors(ObjectReference _Caster,Form _Spell,float _Radius = 0.0,bool _BlameCaster=False,bool _CastToCaster = false,ObjectReference _Center = None) Global Native

; only cast spell to those who have specific keyword
; _MatchAllKeywords only works when Keyword_or_FormList is a formlist
Form[] Function InstantCastToActorsWithFilter(ObjectReference _Caster,Form _Spell, float _Radius = 0.0,bool _BlameCaster = false, bool _CastToCaster = false,ObjectReference _Center = None, Form Keyword_or_FormList =None,bool _MatchAllKeywords = False) Global Native

; Same as InstantCastToActorsWithFilter but given a array of keywords
Form[] Function InstantCastToActorsWithFilterAlt(ObjectReference _Caster,Form _Spell, float _Radius = 0.0,bool _BlameCaster = false, bool _CastToCaster = false,ObjectReference _Center = None,Keyword[] Keyword_Arr = None,bool _MatchAllKeywords = False) Global Native
```

### 时间流速

单独设定时间流速倍率.  

```papyrus
; If have no any input will recover both timemult
; If _setter = false, the _WorldTimeMult and _PlayerOnlyTimeMult will regard as a multiplier
Function SetSlowTimeMult(float _WorldTimeMult = 0.0, float _PlayerOnlyTimeMult = 1.0, bool _Setter = true) Global Native

; trashutility only tracks the timemult set by SetSlowTimeMult, so this function only return the mult set by SetSlowTimeMult
float Function GetSlowTimeMult(bool _GetWorldTimeMult = true) Global Native
```

### 施加打击

立即对 victim 施加打击.  
以下函数会触发 `OnHit` 事件.  

```papyrus

Function ApplyMeleeHit(Actor _attacker,Actor _victim,bool lefthand = false) Global Native

;specific a weapon to hit
;when _applyench = true will apply the enchantment of weapon(if have) to _victim
Function ApplyHit(Actor _attacker,Actor _victim,Weapon _weapon,bool _applyench = true) Global Native
```

### 调试与杂项

```papyrus
Function ConsoleInfo(string _str) Global Native

string Function GetFormIDString(form _form) Global Native
  
string Function ToHexString(int _num) Global Native
  
Function PrintFlt(float _flt) Global Native
  
Function PrintForm(Form _form) Global Native

Function DumpFormArray(Form[] _FormArr) Global Native
  
Function DumpRefArray(ObjectReference[] _RefArr) Global Native
  
Function DumpIntArray(int[] _IntArr) Global Native
  
Function DumpFloatArray(float[] _form) Global Native

int Function GetIntMax() Global Native
  
float Function GetFloatMax() Global Native
  
int Function GetIntMin() Global Native
  
float Function GetFloatMin() Global Native
```

# 控制台集成

部分上述函数已被集成为控制台命令，此时其不区分大小写但所有输入参数都应**明确给定**.  

### 用控制台查询函数

输入 `trashutility` 会输出**所有可用的控制台函数**.  
输入 `trashutility-function` 会输出**所有控制台可用的杂项函数**.  
输入 `trashutility-playerctrl` 会输出**所有控制台可用的玩家运动与玩家镜头运动覆盖函数**.  
输入 `trashutility-collection` 会输出**所有控制台可用的辅助容器函数**.  
