//==============================================================================================================

#include "FiremodeEnumParameter.h"

//==============================================================================================================

UFireModeEnumParameter::UFireModeEnumParameter()
{
	Value = EFireMode::FIREMODE_AUTOMATIC;

	DefaultValue = EFireMode::FIREMODE_AUTOMATIC;
}

//==============================================================================================================

void UFireModeEnumParameter::Initialise(FString displayName, EFireMode defaultValue)
{
	UParameterBase::InitialiseBase(displayName);

	Value = defaultValue;

	DefaultValue = defaultValue;
}

//==============================================================================================================

void UFireModeEnumParameter::FromString(FString value)
{
	if (value == "SEMI")
	{
		Value = EFireMode::FIREMODE_SEMIAUTOMATIC;
	}
	else if (value == "AUTO")
	{
		Value = EFireMode::FIREMODE_AUTOMATIC;
	}
	else
	{
		Value = DefaultValue;
	}
	OnValueChanged.Broadcast(Value);
}

//==============================================================================================================

FString UFireModeEnumParameter::ToString()
{
	switch (Value)
	{
	case EFireMode::FIREMODE_SEMIAUTOMATIC:
		return "SEMI";
		break;
	case EFireMode::FIREMODE_AUTOMATIC:
		return "AUTO";
		break;
	default:
		return "";
		break;
	}
}

//==============================================================================================================

void UFireModeEnumParameter::Reset()
{
	Value = DefaultValue;

	OnValueChanged.Broadcast(Value);
}

//==============================================================================================================

void UFireModeEnumParameter::SetValue(EFireMode value)
{
	if (IsEditable())
	{
		Value = value;

		OnValueChanged.Broadcast(Value);
	}
}

//==============================================================================================================