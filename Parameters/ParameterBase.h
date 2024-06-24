//==============================================================================================================

#pragma once

//==============================================================================================================

#include "Project_Niobium.h"

//==============================================================================================================

#include "UObject/NoExportTypes.h"
#include "ParameterBase.generated.h"

//==============================================================================================================

UCLASS(BlueprintType, Abstract)
class PROJECT_NIOBIUM_API UParameterBase : public UObject
{
	GENERATED_BODY()

public:
	UParameterBase();

protected:
	void InitialiseBase(FString displayName);

	//=== Name ===============================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const FString GetDisplayName() const { return DisplayName; }

private:
	FString DisplayName;

	//=== String Conversion ==================================
public:
	virtual void FromString(FString value) {}
	virtual FString ToString() { return ""; }

	//=== Reset ==============================================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void ResetValue() { Reset(); }
	virtual void Reset() {}

	//=== Editability =======================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Parameters")
	const bool IsEditable() const { return Editable; }
	UFUNCTION(BlueprintCallable, Category = "Niobium|Parameters")
	void SetEditable(bool state) { Editable = state; }

private:
	bool Editable;
};

//==============================================================================================================