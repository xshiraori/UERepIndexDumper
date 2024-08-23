#pragma once
#include "FField.hpp"
#include "FNamePool.hpp"
#include "UObject.hpp"

namespace UE503 {

	struct UStruct;
	struct UClass;
	struct UFunction;
	struct UEnum;
	struct FFieldClass;

	enum class EPropertyFlags : uint64_t
	{
		CPF_None = 0,
		CPF_Edit = 0x0000000000000001,
		CPF_ConstParm = 0x0000000000000002,
		CPF_BlueprintVisible = 0x0000000000000004,
		CPF_ExportObject = 0x0000000000000008,
		CPF_BlueprintReadOnly = 0x0000000000000010,
		CPF_Net = 0x0000000000000020,
		CPF_EditFixedSize = 0x0000000000000040,
		CPF_Parm = 0x0000000000000080,
		CPF_OutParm = 0x0000000000000100,
		CPF_ZeroConstructor = 0x0000000000000200,
		CPF_ReturnParm = 0x0000000000000400,
		CPF_DisableEditOnTemplate = 0x0000000000000800,
		CPF_Transient = 0x0000000000002000,
		CPF_Config = 0x0000000000004000,
		CPF_DisableEditOnInstance = 0x0000000000010000,
		CPF_EditConst = 0x0000000000020000,
		CPF_GlobalConfig = 0x0000000000040000,
		CPF_InstancedReference = 0x0000000000080000,
		CPF_DuplicateTransient = 0x0000000000200000,
		CPF_SaveGame = 0x0000000001000000,
		CPF_NoClear = 0x0000000002000000,
		CPF_ReferenceParm = 0x0000000008000000,
		CPF_BlueprintAssignable = 0x0000000010000000,
		CPF_Deprecated = 0x0000000020000000,
		CPF_IsPlainOldData = 0x0000000040000000,
		CPF_RepSkip = 0x0000000080000000,
		CPF_RepNotify = 0x0000000100000000,
		CPF_Interp = 0x0000000200000000,
		CPF_NonTransactional = 0x0000000400000000,
		CPF_EditorOnly = 0x0000000800000000,
		CPF_NoDestructor = 0x0000001000000000,
		CPF_AutoWeak = 0x0000004000000000,
		CPF_ContainsInstancedReference = 0x0000008000000000,
		CPF_AssetRegistrySearchable = 0x0000010000000000,
		CPF_SimpleDisplay = 0x0000020000000000,
		CPF_AdvancedDisplay = 0x0000040000000000,
		CPF_Protected = 0x0000080000000000,
		CPF_BlueprintCallable = 0x0000100000000000,
		CPF_BlueprintAuthorityOnly = 0x0000200000000000,
		CPF_TextExportTransient = 0x0000400000000000,
		CPF_NonPIEDuplicateTransient = 0x0000800000000000,
		CPF_ExposeOnSpawn = 0x0001000000000000,
		CPF_PersistentInstance = 0x0002000000000000,
		CPF_UObjectWrapper = 0x0004000000000000,
		CPF_HasGetValueTypeHash = 0x0008000000000000,
		CPF_NativeAccessSpecifierPublic = 0x0010000000000000,
		CPF_NativeAccessSpecifierProtected = 0x0020000000000000,
		CPF_NativeAccessSpecifierPrivate = 0x0040000000000000,
		CPF_SkipSerialization = 0x0080000000000000,
	};

	enum ELifetimeCondition
	{
		COND_None = 0,
		COND_InitialOnly = 1,
		COND_OwnerOnly = 2,
		COND_SkipOwner = 3,
		COND_SimulatedOnly = 4,
		COND_AutonomousOnly = 5,
		COND_SimulatedOrPhysics = 6,
		COND_InitialOrOwner = 7,
		COND_Custom = 8,
		COND_ReplayOrOwner = 9,
		COND_ReplayOnly = 10,
		COND_SimulatedOnlyNoReplay = 11,
		COND_SimulatedOrPhysicsNoReplay = 12,
		COND_SkipReplay = 13,
		COND_Never = 15,
		COND_Max = 16
	};

	struct FProperty : FField
	{
		int32_t	ArrayDim; 
		int32_t	ElementSize; //0x3C 0x34
		EPropertyFlags PropertyFlags; //0x40 0x38
		uint16_t RepIndex; //0x48 0x40
		uint16_t BlueprintReplicationCondition; //UE::TEnumAsByte<ELifetimeCondition> 
		int32_t Offset_Internal; 
		FProperty* PropertyLinkNext;
		FProperty* NextRef;
		FProperty* DestructorLinkNext;
		FProperty* PostConstructLinkNext; 
		FName RepNotifyFunc;
	};

	struct FStructProperty : FProperty
	{
		UStruct* Struct; // Should be UScriptStruct (not implemented)
	};

	struct FObjectPropertyBase : FProperty
	{
		UClass* PropertyClass;
	};

	struct FArrayProperty : FProperty
	{
		enum class EArrayPropertyFlags
		{
			None,
			UsesMemoryImageAllocator
		};
		// Swapped in UE5.3
		EArrayPropertyFlags ArrayFlags;
		FProperty* Inner;
	};

	struct FByteProperty : FProperty
	{
		UEnum* Enum;
	};

	struct FBoolProperty : FProperty
	{
		uint8_t FieldSize;
		uint8_t ByteOffset;
		uint8_t ByteMask;
		uint8_t FieldMask;
	};

	struct FNumericProperty : FProperty {};

	struct FEnumProperty : FProperty
	{
		FNumericProperty* UnderlyingProp;
		UEnum* Enum;
	};

	struct FObjectProperty : FObjectPropertyBase {};

	struct FClassProperty : FObjectProperty
	{
		UClass* MetaClass;
	};


	struct FSetProperty : FProperty
	{
		FProperty* ElementProp;
		char pad_SetLayout[0x20];
	};

	struct FMapProperty : FProperty
	{
		enum class EMapPropertyFlags
		{
			None,
			UsesMemoryImageAllocator
		};

		FProperty* KeyProp;
		FProperty* ValueProp;
		char pad_MapLayout[0x24];
		EMapPropertyFlags MapFlags;
	};

	struct FInterfaceProperty : FProperty
	{
		UClass* InterfaceClass;
	};

	struct FFieldPathProperty : FProperty
	{
		FFieldClass* PropertyClass;
	};

	struct FDelegateProperty : FProperty
	{
		UFunction* SignatureFunction;
	};

	struct FMulticastDelegateProperty : FProperty
	{
		UFunction* SignatureFunction;
	};

	struct FMulticastSparseDelegateProperty : FMulticastDelegateProperty
	{};

	struct FWeakObjectProperty : FObjectPropertyBase {};

	struct FSoftObjectProperty : FObjectPropertyBase {};

	struct FSoftClassProperty : FSoftObjectProperty
	{
		UClass* MetaClass;
	};

}