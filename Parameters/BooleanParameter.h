//==============================================================================================================

#pragma once

//==============================================================================================================

#include "Delegate.h"
#include "NiobiumSystem/Utility/Parameters/ParameterBase.h"
#include "BooleanParameter.generated.h"

//==============================================================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNiobiumBooleanValueChangedDelegate, bool, Value);

//==============================================================================================================

UCLASS(BlueprintType)
class PROJECT_NIOBIUM_API UBooleanParameter : public UParameterBase
{
	GENERATED_BODY()

public:
	UBooleanParameter();

	//=== UParameterBase Interface ===========================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void Initialise(FString displayName, bool defaultValue);

	void FromString(FString value) override;
	FString ToString() override;

	void Reset() override;

	//=== Value ==============================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const bool GetValue() const { return Value; }
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void SetValue(bool value);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void ToggleValue();
	
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const bool GetDefaultValue() const { return DefaultValue; }

private:
	bool Value;

	bool DefaultValue;

	//=== Delegate ===========================================
public:
	UPROPERTY(BlueprintAssignable)
	FNiobiumBooleanValueChangedDelegate OnValueChanged;
};

//==============================================================================================================