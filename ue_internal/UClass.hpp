#pragma once
#include "UObject.hpp"
#include <cstdint>
#include "../SDK/SDK.h"
#include "FNamePool.hpp"


namespace UE503 {

	enum class EClassCastFlags : uint32_t
	{
		None = 0x0000000000000000,

		UField = 0x0000000000000001,
		FInt8Property = 0x0000000000000002,
		UEnum = 0x0000000000000004,
		UStruct = 0x0000000000000008,
		UScriptStruct = 0x0000000000000010,
		UClass = 0x0000000000000020,
		FByteProperty = 0x0000000000000040,
		FIntProperty = 0x0000000000000080,
		FFloatProperty = 0x0000000000000100,
		FUInt64Property = 0x0000000000000200,
		FClassProperty = 0x0000000000000400,
		FUInt32Property = 0x0000000000000800,
		FInterfaceProperty = 0x0000000000001000,
		FNameProperty = 0x0000000000002000,
		FStrProperty = 0x0000000000004000,
		FProperty = 0x0000000000008000,
		FObjectProperty = 0x0000000000010000,
		FBoolProperty = 0x0000000000020000,
		FUInt16Property = 0x0000000000040000,
		UFunction = 0x0000000000080000,
		FStructProperty = 0x0000000000100000,
		FArrayProperty = 0x0000000000200000,
		FInt64Property = 0x0000000000400000,
		FDelegateProperty = 0x0000000000800000,
		FNumericProperty = 0x0000000001000000,
		FMulticastDelegateProperty = 0x0000000002000000,
		FObjectPropertyBase = 0x0000000004000000,
		FWeakObjectProperty = 0x0000000008000000,
		FLazyObjectProperty = 0x0000000010000000,
		FSoftObjectProperty = 0x0000000020000000,
		FTextProperty = 0x0000000040000000,
		FInt16Property = 0x0000000080000000,
		FDoubleProperty = 0x0000000100000000,
		FSoftClassProperty = 0x0000000200000000,
		UPackage = 0x0000000400000000,
		ULevel = 0x0000000800000000,
		AActor = 0x0000001000000000,
		APlayerController = 0x0000002000000000,
		APawn = 0x0000004000000000,
		USceneComponent = 0x0000008000000000,
		UPrimitiveComponent = 0x0000010000000000,
		USkinnedMeshComponent = 0x0000020000000000,
		USkeletalMeshComponent = 0x0000040000000000,
		UBlueprint = 0x0000080000000000,
		UDelegateFunction = 0x0000100000000000,
		UStaticMeshComponent = 0x0000200000000000,
		FMapProperty = 0x0000400000000000,
		FSetProperty = 0x0000800000000000,
		FEnumProperty = 0x0001000000000000,
		USparseDelegateFunction = 0x0002000000000000,
		FMulticastInlineDelegateProperty = 0x0004000000000000,
		FMulticastSparseDelegateProperty = 0x0008000000000000,
		FFieldPathProperty = 0x0010000000000000,
		FObjectPtrProperty = 0x0020000000000000,
		FClassPtrProperty = 0x0040000000000000,
		FLargeWorldCoordinatesRealProperty = 0x0080000000000000,
		AllFlags = 0xFFFFFFFFFFFFFFFF,
	};


	enum class EClassFlags : uint32_t
	{
		None = 0x00000000u,
		Abstract = 0x00000001u,
		DefaultConfig = 0x00000002u,
		Config = 0x00000004u,
		Transient = 0x00000008u,
		Optional = 0x00000010u,
		MatchedSerializers = 0x00000020u,
		ProjectUserConfig = 0x00000040u,
		Native = 0x00000080u,
		NoExport = 0x00000100u,
		NotPlaceable = 0x00000200u,
		PerObjectConfig = 0x00000400u,
		ReplicationDataIsSetUp = 0x00000800u,
		EditInlineNew = 0x00001000u,
		CollapseCategories = 0x00002000u,
		Interface = 0x00004000u,
		CustomConstructor = 0x00008000u,
		Const = 0x00010000u,
		NeedsDeferredDependencyLoading = 0x00020000u,
		CompiledFromBlueprint = 0x00040000u,
		MinimalAPI = 0x00080000u,
		RequiredAPI = 0x00100000u,
		DefaultToInstanced = 0x00200000u,
		TokenStreamAssembled = 0x00400000u,
		HasInstancedReference = 0x00800000u,
		Hidden = 0x01000000u,
		Deprecated = 0x02000000u,
		HideDropDown = 0x04000000u,
		GlobalUserConfig = 0x08000000u,
		Intrinsic = 0x10000000u,
		Constructed = 0x20000000u,
		ConfigDoNotCheckDefaults = 0x40000000u,
		NewerVersionExists = 0x80000000u,
		Inherit = Transient | Optional | DefaultConfig | Config | PerObjectConfig | ConfigDoNotCheckDefaults | NotPlaceable | Const | HasInstancedReference |
		Deprecated | DefaultToInstanced | GlobalUserConfig | ProjectUserConfig | NeedsDeferredDependencyLoading,
		RecompilerClear = Inherit | Abstract | Native | Intrinsic | TokenStreamAssembled,
		ShouldNeverBeLoaded = Native | Optional | Intrinsic | TokenStreamAssembled,
		ScriptInherit = Inherit | EditInlineNew | CollapseCategories,
		SaveInCompiledInClasses = Abstract | DefaultConfig | GlobalUserConfig | ProjectUserConfig | Config | Transient | Optional | Native | NotPlaceable | PerObjectConfig |
		ConfigDoNotCheckDefaults | EditInlineNew | CollapseCategories | Interface | DefaultToInstanced | HasInstancedReference | Hidden | Deprecated |
		HideDropDown | Intrinsic | Const | MinimalAPI | RequiredAPI | MatchedSerializers | NeedsDeferredDependencyLoading,
	};

	struct UClassInternal
	{
		uint64_t vtable;
		EObjectFlags ObjectFlags;
		uint32_t InternalIndex;
		UClass* ClassPrivate;
		FName NamePrivate;
		UObject* OuterPrivate;
		UField* Next;
		void** StructBaseChainArray;
		int32_t NumStructBasesInChainMinusOne;
		uint64_t SuperStruct;
		UField* Children; // TObjectPtr
		FField* ChildProperties;
		int32_t PropertiesSize;
		int32_t MinAlignment;
		uint8_t Script[16];
		FProperty* PropertyLink;
		FProperty* RefLink;
		FProperty* DestructorLink;
		FProperty* PostConstructLink;
		uint8_t ScriptAndPropertyObjectReferences[16];
		void* UnresolvedScriptProperties;
		void* UnversionedGameSchema;
		void (*ClassConstructor)();
		UObject* (*ClassVTableHelperCtorCaller)();
		uint64_t CppClassStaticFunctions;
		int32_t ClassUnique;
		int32_t FirstOwnedClassRep;
		uint8_t padding[4];
		EClassFlags ClassFlags;
		EClassCastFlags ClassCastFlags;
		uint64_t ClassWithin;
		FName ClassConfigName;
		uint8_t ClassReps[16];
		uint8_t NetFields[16];
		void* ClassDefaultObject;
		void* SparseClassData;
		uint64_t SparseClassDataStruct;
		// TODO(DOU): DEFINE TMAP
		TMap<FName, UE::TObjectPtr<UFunction>> FuncMap; // TMap is invalid
		uint64_t FuncMapLock;
		TMap<FName, UFunction*> AllFunctionsCache; // TMap is invalid
		// ...
		uint64_t AllFunctionsCacheLock;
		uint8_t Interfaces[16];
		uint64_t ReferenceSchema;
		uint8_t NativeFunctionLookupTable[16];
	};

}