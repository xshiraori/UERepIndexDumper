#include "FField.hpp"

namespace UE503 {

	std::string FFieldClass::GetName()
	{
		return Name.GetName();
	}

	std::string FField::GetName()
	{
		return NamePrivate.GetName();
	}

}