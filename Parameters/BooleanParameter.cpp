//==============================================================================================================

#include "BooleanParameter.h"

//==============================================================================================================

UBooleanParameter::UBooleanParameter()
{
	Value = false;

	DefaultValue = false;
}

//==============================================================================================================

void UBooleanParameter::Initialise(FString displayName, bool defaultValue)
{
	UParameterBase::InitialiseBase(displayName);

	Value = defaultValue;

	DefaultValue = defaultValue;
}

//==============================================================================================================

void UBooleanParameter::FromString(FString value)
{
	Value = (value == "T");
	OnValueChanged.Broadcast(Value);
}

//==============================================================================================================

FString UBooleanParameter::ToString()
{
	return (Value ? "T" : "F");
}

//==============================================================================================================

void UBooleanParameter::Reset()
{
	Value = DefaultValue;

	OnValueChanged.Broadcast(Value);
}

//==============================================================================================================

void UBooleanParameter::SetValue(bool value)
{
	if (IsEditable())
	{
		Value = value;

		OnValueChanged.Broadcast(Value);
	}
}

//==============================================================================================================

void UBooleanParameter::ToggleValue()
{
	if (IsEditable())
	{
		Value = !Value;

		OnValueChanged.Broadcast(Value);
	}
}

//==============================================================================================================