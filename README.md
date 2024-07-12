# Introduce
**TrashUtility** is a plugin built with CommonLibSSE-NG. It aims to provide new papyrus functions for The Elder Scrolls V: Skyrim Special Edition.

Download :[NEXUS PAGE](https://www.nexusmods.com/skyrimspecialedition/mods/124076)

# Credit
[SKSE Team](https://skse.silverlock.org/).  
[People who contribute to commonlib-ng](https://github.com/CharmedBaryon/CommonLibSSE-NG).  
[JIP that inspired me a lot](https://github.com/jazzisparis/JIP-LN-NVSE).  
[meh321 for Address Library](https://www.nexusmods.com/skyrimspecialedition/mods/32444?).  
[Fenix31415 for his patience and guidance](https://github.com/fenix31415).  
[Qudix for ng template](https://github.com/qudix/commonlibsse-ng-template).  

# Function
## Player Movement and Player Camera Movement Override
TrashUtility provides a set of functions that can be used to override `player movement` and `player camera movement`.  
**1.** In Player Control, the player's movement to the left and right is considered movement along the x-axis, while movement forward and backward is considered movement along the y-axis.  
**2.** Similarly, the player's camera movement to the left and right is considered movement along the x-axis, while movement up and down is considered movement along the y-axis.  
**3.** All Override to movement will recover after loading a save

In the following functions, the `flag` variable is used to represent the override of movement in the specified axis direction.  
| Direction   | Flag        |
| :---        |    :----    |
| X           | 0           |
| Y           | 1           |
| X AND Y       | 2           |
| X OR Y      | 3           |

**For convenience**, I will explain the values of `flag` parameter using the following format like:.  
```papyrus
; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | flag =3 -X||Y | other -X||Y
```
If you want to use function below you need to add
```papyrus
Import Trash_PlayerControl
```

```papyrus
; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X & Y
;Reverse player camera movement direction when **_Reverse ** is true
Function ReverseLook( bool _Reverse = False, int flag = -1) global native.

; |flag = 0 -X | flag =1 -Y | flag =2 -X & Y | other -X & Y
;Reverse player movement direction when **_Reverse ** is true
Function ReverseMove( bool _Reverse = False, int flag = -1) global native.

;Reverse both player movement and player camera movement direction when **_Reverse ** is true
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
**Example**
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

## Auxiliary Collection
There are two different types of collection: `Auxiliary Array` and `Ref Map`
The inspiration and ideas for the Auxiliary Collection come from the JIP plugin, and its Properties is similar to the auxiliary variable interface provided by the JIP plugin.  
(Some of contents below quoted from: <https://geckwiki.com/index.php?title=Category:Auxiliary-Variable_Functions_(JIP)> but there is a difference between them).  


### Properties
**1.** An `Auxiliary Collection` is defined by a `Name (string)` and an `Object (object reference/base form)` that "owns" it. This Object:Name pair must, therefore, be unique. Note that **different Objects** "own" a Collection with the same Name, or a **same Object** "own" different type of collections with the same name, will still count as a unique pair. Any Object may "own" any number of Collection.  
**2.** `Auxiliary Collection` can store either float, reference/form values.their type changes dynamically, according to the type of value assigned to them.

### Duration
An `Auxiliary Collection`'s duration setting determines its "life-span", i.e. whether it is permanent(save with game) or temporary.  
#### permanent
permanent collection will save with game.
#### temporary
temporary collection will not save with game, instead they persistent (retain their values) for the duration of the current game session but are discarded when the game is exited.  

Determine its `duration` by adding a special prefix to collection name.  
| Prefix   | Duration        |
| :---        |    :----    |
| *           | temporary           |
| None           |  permanent          |

**Example:**  _"*MyAuxColl"_ is a way to name a `temporary` collection

### Accessibility
`Auxiliary Collection` more like a global collection. Once the `Auxiliary Collection` is created, all scripts can access it with function

### The difference with auxiliary variable interface and ref map from JIP
**1.** The name of `Auxiliary Collection` is not case-insensitive( I must admit I carelessly overlooked this ). However, you should try to ensure that you use unique names when using them.  
**2.** Lack of accessibility setting.  
**3.** Lack of an easy way to traverse ref map.  
**4.** Don't support string type element for now.  

### Auxiliary Collection Function
As you can see below, you need to explicitly pass in `Holder(Reference\Form)` and `Collection Name` to call those function.  
You don't need to construct a `Auxiliary Collection`.When you use a setter function, the `Auxiliary Collection` will be created **automatically**.  

If you want to use function below you need to add
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
Map is an associative container, and ref map here is use `reference/form` as a key value.  
Internally, the elements in a RefMap are ordered by FormID (ascending).  
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
#### Others
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
## Misc Function
If you want to use function below you need to add 
```papyrus
Import Trash_Function
```
### Distance
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
### Pos
```papyrus
;Get reference current position as a float array. index 0,1,2 correspond x,y,z respectively 
float[] Function GetPosAsArray(ObjectReference _obj) Global Native

;Set reference position to (x,y,z). 
;When transform==true will do a transform base on reference current position( Set reference position to (current position) + (x,y,z).)
Function SetPosAlt(ObjectReference _obj,float x,float y,float z,bool transform = False) Global Native
;Do the same thing but read index 0,1,2 from PosArr as x,y,z
Function SetPosFromArray(ObjectReference _obj, float[] PosArr,bool transform = False) Global Native
```

### HeadingAngle
Check original function: <https://ck.uesp.net/wiki/GetHeadingAngle>
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
### Sound
```papyrus
;Just play sound in specific coordinate
;_Sound only accept the SoundDescriptor(SNDR) as input
bool Function PlaySoundAtPoint (Form _Sound,float x,float y,float z) Global Native
```

### CastSpellToGroup
Specify a `_Caster` and `_Spell(Spell,Enchantment,AlchemyItem,Scroll,Ingredient)`.  
When `_CastToCaster` == false ,The `_Spell` will not cast to `_Caster` even if the `_Caster` is within the `_Radiu`.  
When `_Center` = None, will regard `_Caster` as center, for targets within the `_Radiu` cast `_Spell` immediately.  
```papyrus
Form[] Function InstantCastToActors(ObjectReference _Caster,Form _Spell,float _Radius = 0.0,bool _BlameCaster=False,bool _CastToCaster = false,ObjectReference _Center = None) Global Native

; only cast spell to those who have specific keyword
; _MatchAllKeywords only works when Keyword_or_FormList is a formlist
Form[] Function InstantCastToActorsWithFilter(ObjectReference _Caster,Form _Spell, float _Radius = 0.0,bool _BlameCaster = false, bool _CastToCaster = false,ObjectReference _Center = None, Form Keyword_or_FormList =None,bool _MatchAllKeywords = False) Global Native

; Same as InstantCastToActorsWithFilter but given a array of keywords
Form[] Function InstantCastToActorsWithFilterAlt(ObjectReference _Caster,Form _Spell, float _Radius = 0.0,bool _BlameCaster = false, bool _CastToCaster = false,ObjectReference _Center = None,Keyword[] Keyword_Arr = None,bool _MatchAllKeywords = False) Global Native
```
### Slowtime
Can set slowtime mult respectively.
```papyrus
; If have no any input will recover both timemult
; If _setter = false, the _WorldTimeMult and _PlayerOnlyTimeMult will regard as a multiplier
Function SetSlowTimeMult(float _WorldTimeMult = 0.0, float _PlayerOnlyTimeMult = 1.0, bool _Setter = true) Global Native

; trashutility only tracks the timemult set by SetSlowTimeMult, so this function only return the mult set by SetSlowTimeMult
float Function GetSlowTimeMult(bool _GetWorldTimeMult = true) Global Native
```

### ApplyHit
Apply hit immediately to the victim.  
Functions below will trigger `OnHit` Event.  
```papyrus

Function ApplyMeleeHit(Actor _attacker,Actor _victim,bool lefthand = false) Global Native

;specific a weapon to hit
;when _applyench = true will apply the enchantment of weapon(if have) to _victim
Function ApplyHit(Actor _attacker,Actor _victim,Weapon _weapon,bool _applyench = true) Global Native
```

### Debug And Misc
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

# Console integration
Some of the above functions have been integrated into the console and can be run on the console.  
Case-insensitive, but all the passed arguments need to be given **explicitly**.  
### Query function in console
input `trashutility` will print **all available console function**.  
input `trashutility-function` will print all available console function of **Misc Function**.  
input `trashutility-playerctrl` will print all available console function of **Player Movement and Player Camera Movement Override**.  
input `trashutility-collection` will print all available console function of **Auxiliary Collection**.  
