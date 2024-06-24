//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Cores/CoreBase.h"

//==============================================================================================================

#include "NiobiumSystem/Utility/Parameters/FiremodeEnumParameter.h"

//==============================================================================================================

#include "StructCore.generated.h"

//==============================================================================================================
/*
 * STRUCT Core
 * Produces a cone of plasma shrapnel
 * Represents Shotgun type weapons
*/
//==============================================================================================================

UCLASS()
class PROJECT_NIOBIUM_API UStructCore : public UCoreBase
{
	GENERATED_BODY()

public:
	UStructCore();
	~UStructCore();

	//=== UCoreBase Interface ================================
protected:
	void InitialiseCore() override;
	void UpdateCore(float deltaTime);

public:
	void OnFireHeld() override;
	void OnFireReleased() override;

	void FireCore() override;

	//=== UNiobiumBase Interface =============================
protected:
	void AmmunitionFired(AAmmunitionBase* ammunition) override;
	void AmmunitionUpdate(AAmmunitionBase* ammunition, float deltaTime) override;

protected:
	void InitialiseParameters() override;

	//=== Function============================================
private:
	void SetFireDelay();

	void SetBurstCounter();

private:
	float FireDelay;

	int BurstCounter;
	float BurstDelay;

	// Parameters
	/// Shards
private:
	const FString ShardsParameter = "Shards";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	int GetShardsValue();

	/// Spread
private:
	const FString SpreadParameter = "Spread";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetSpreadValue();

	/// Shutter - Fire Mode
private:
	const FString ShutterFireModeParameter = "Shutter_FireMode";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	EFireMode GetShutterFireModeValue();

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	bool DoesShutterFireModeEqual(EFireMode value);

private:
	UFUNCTION()
	void OnShutterFireModeChanged(EFireMode value);

	/// Shutter - Delay
private:
	const FString ShutterDelayParameter = "Shutter_Delay";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetShutterDelayValue();

	/// Shutter - Burst
private:
	const FString ShutterBurstParameter = "Shutter_Burst";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	bool GetShutterBurstValue();

private:
	UFUNCTION()
	void OnShutterBurstValueChanged(bool value);

	/// Shutter - Burst Count
private:
	const FString ShutterBurstCountParameter = "Shutter_BurstCount";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	int GetShutterBurstCountValue();

	/// Shutter - Burst Delay
private:
	const FString ShutterBurstDelayParameter = "Shutter_BurstDelay";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetShutterBurstDelayValue();

};

//==============================================================================================================