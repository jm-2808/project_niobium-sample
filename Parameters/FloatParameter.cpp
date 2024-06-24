//==============================================================================================================

#include "FloatParameter.h"

//==============================================================================================================

UFloatParameter::UFloatParameter()
{
	Value = 0.0f;

	DefaultValue = 0.0f;

	MinimumValue = 0.0f;
	MaximumValue = 0.0f;

	Increment = 0.0f;
}

//==============================================================================================================

void UFloatParameter::Initialise(FString displayName, float defaultValue, float minimumValue, float maximumValue, float increment)
{
	UParameterBase::InitialiseBase(displayName);

	Value = defaultValue;

	DefaultValue = defaultValue;

	MinimumValue = minimumValue;
	MaximumValue = maximumValue;

	Increment = increment;
}

//==============================================================================================================

void UFloatParameter::FromString(FString value)
{
	float val = FCString::Atof(*value);
	SetValue(val);
}

//==============================================================================================================

FString UFloatParameter::ToString()
{
	return FString::SanitizeFloat(Value);
}

//==============================================================================================================

void UFloatParameter::Reset()
{
	Value = DefaultValue;
}

//==============================================================================================================

void UFloatParameter::SetValue(float value)
{
	if (IsEditable())
	{
		Value = FMath::Clamp(value, MinimumValue, MaximumValue);
	}
}

//==============================================================================================================

void UFloatParameter::IncrementValue()
{
	if (IsEditable())
	{
		if (Value < MaximumValue)
		{
			Value += Increment;
			if (Value > MaximumValue)
			{
				Value = MaximumValue;
			}
		}
	}
}

//==============================================================================================================

void UFloatParameter::DecrementValue()
{
	if (IsEditable())
	{
		if (Value > MinimumValue)
		{
			Value -= Increment;
			if (Value < MinimumValue)
			{
				Value = MinimumValue;
			}
		}
	}
}

//==============================================================================================================