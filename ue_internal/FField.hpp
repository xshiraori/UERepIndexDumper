#pragma once
#include <string>
#include "FNamePool.hpp"

namespace UE503 {

	struct FFieldClass;
	struct FField;
	struct UObject;

	struct FFieldVariant {
		union FFieldObjectUnion
		{
			FField* Field;
			UObject* Object;
		} Container;

		// Removed in UE5.3
		// bool bIsUObject;
	};

	struct FField {
		void** VFTable;

		/** Pointer to the class object representing the type of this FField */
		FFieldClass* ClassPrivate;

		FFieldVariant Owner;

		/** Next Field in the linked list */
		FField* Next;

		/** Name of this field */
		FName NamePrivate;

		uint32_t padding; // EObjectFlags FlagsPrivate;

		std::string GetName();
	};

	struct FFieldClass {
		FName Name;

		char pad_08[0x30]; // 0x08 (0x30)

		std::string GetName();
	};

}