//==============================================================================================================

#include "IntegerParameter.h"

//==============================================================================================================

UIntegerParameter::UIntegerParameter()
{
	Value = 0;

	DefaultValue = 0;

	MinimumValue = 0;
	MaximumValue = 0;

	Increment = 0;
}

//==============================================================================================================

void UIntegerParameter::Initialise(FString displayName, int defaultValue, int minimumValue, int maximumValue, int increment)
{
	UParameterBase::InitialiseBase(displayName);

	Value = defaultValue;

	DefaultValue = defaultValue;

	MinimumValue = minimumValue;
	MaximumValue = maximumValue;

	Increment = increment;
}

//==============================================================================================================

void UIntegerParameter::FromString(FString value)
{
	int val = FCString::Atoi(*value);
	SetValue(val);
}

//==============================================================================================================

FString UIntegerParameter::ToString()
{
	return FString::FromInt(Value);
}

//==============================================================================================================

void UIntegerParameter::Reset()
{
	Value = DefaultValue;
}

//==============================================================================================================

void UIntegerParameter::SetValue(int value)
{
	if (IsEditable())
	{
		Value = FMath::Clamp(value, MinimumValue, MaximumValue);
	}
}

//==============================================================================================================

void UIntegerParameter::IncrementValue()
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

void UIntegerParameter::DecrementValue()
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