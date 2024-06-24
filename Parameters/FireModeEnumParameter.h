//==============================================================================================================

#pragma once

//==============================================================================================================

#include "Delegate.h"
#include "NiobiumSystem/Utility/Parameters/ParameterBase.h"
#include "FireModeEnumParameter.generated.h"

//==============================================================================================================

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	FIREMODE_SEMIAUTOMATIC UMETA(DisplayName = "Semi-Automatic"),
	FIREMODE_AUTOMATIC UMETA(DisplayName = "Automatic")
};

//==============================================================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNiobiumFireModeEnumChangedDelegate, EFireMode, Value);

//==============================================================================================================

UCLASS(BlueprintType)
class PROJECT_NIOBIUM_API UFireModeEnumParameter : public UParameterBase
{
	GENERATED_BODY()

public:
	UFireModeEnumParameter();

	//=== UParameterBase Interface ===========================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void Initialise(FString displayName, EFireMode defaultValue);

	void FromString(FString value) override;
	FString ToString() override;

	void Reset() override;

	//=== Value ==============================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const EFireMode GetValue() const { return Value; }
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void SetValue(EFireMode value);

	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	bool DoesValueEqual(EFireMode condition) { return (Value == condition); }

	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const EFireMode GetDefaultValue() const { return DefaultValue; }

private:
	EFireMode Value;

	EFireMode DefaultValue;

	//=== Delegate ===========================================
public:
	UPROPERTY(BlueprintAssignable)
	FNiobiumFireModeEnumChangedDelegate OnValueChanged;
};

//==============================================================================================================