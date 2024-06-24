//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Utility/Parameters/ParameterBase.h"
#include "FloatParameter.generated.h"

//==============================================================================================================

UCLASS(BlueprintType)
class PROJECT_NIOBIUM_API UFloatParameter : public UParameterBase
{
	GENERATED_BODY()

public:
	UFloatParameter();

	//=== UParameterBase Interface ===========================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void Initialise(FString displayName, float defaultValue, float minimumValue, float maximumValue, float increment);

	void FromString(FString value) override;
	FString ToString() override;

	void Reset() override;

	//=== Value ==============================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const float GetValue() const { return Value; }
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void SetValue(float value);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void IncrementValue();
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void DecrementValue();

	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const float GetDefaultValue() const { return DefaultValue; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const float GetMinimumValue() const { return MinimumValue; }
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const float GetMaximumValue() const { return MaximumValue; }
	
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const float GetIncrement() const { return Increment; }

private:
	float Value;

	float DefaultValue;

	float MinimumValue;
	float MaximumValue;

	float Increment;
};

//==============================================================================================================