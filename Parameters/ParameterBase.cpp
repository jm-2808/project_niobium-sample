//==============================================================================================================

#include "ParameterBase.h"

//==============================================================================================================

UParameterBase::UParameterBase()
{
	DisplayName = "";

	Editable = false;
}

//==============================================================================================================

void UParameterBase::InitialiseBase(FString displayName)
{
	DisplayName = displayName;

	Editable = true;
}

//==============================================================================================================