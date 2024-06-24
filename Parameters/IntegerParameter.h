//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Utility/Parameters/ParameterBase.h"
#include "IntegerParameter.generated.h"

//==============================================================================================================

UCLASS(BlueprintType)
class PROJECT_NIOBIUM_API UIntegerParameter : public UParameterBase
{
	GENERATED_BODY()
	
public:
	UIntegerParameter();

	//=== UParameterBase Interface ===========================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void Initialise(FString displayName, int defaultValue, int minimumValue, int maximumValue, int increment);

	void FromString(FString value) override;
	FString ToString() override;

	void Reset() override;

	//=== Value ==============================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const int GetValue() const { return Value; }
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void SetValue(int value);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void IncrementValue();
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void DecrementValue();

	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const int GetDefaultValue() const { return DefaultValue; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const int GetMinimumValue() const { return MinimumValue; }
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const int GetMaximumValue() const { return MaximumValue; }
	
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const int GetIncrement() const { return Increment; }

private:
	int Value;

	int DefaultValue;

	int MinimumValue;
	int MaximumValue;

	int Increment;
};

//==============================================================================================================