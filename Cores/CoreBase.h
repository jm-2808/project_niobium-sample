//==============================================================================================================

#pragma once

//==============================================================================================================

#include "Project_Niobium.h"

//==============================================================================================================

#include "Components/SceneComponent.h"

#include "NiobiumSystem/Modifiers/ModifierBase.h"

//==============================================================================================================

#include "NiobiumSystem/NiobiumBase.h"
#include "CoreBase.generated.h"

//==============================================================================================================

class UCoreBase;

USTRUCT(BlueprintType)
struct FCoreInformation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCoreBase> CoreClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAmmunitionBase> AmmunitionClass;
};

//==============================================================================================================

UCLASS(Abstract, Blueprintable)
class PROJECT_NIOBIUM_API UCoreBase : public UNiobiumBase
{
	GENERATED_BODY()

public:
	UCoreBase();
	~UCoreBase();

	//=== Basic Function =====================================
public:
	void Initialise(FCoreInformation information);

	void Update(float deltaTime);

protected:
	virtual void InitialiseCore() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Initialise Core"))
	void BP_InitialiseCore();
	void BP_InitialiseCore_Implementation() {}

	virtual void UpdateCore(float deltaTime) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Update Core"))
	void BP_UpdateCore(float deltaTime);
	void BP_UpdateCore_Implementation(float deltaTime) {}

	//=== UNiobiumBase Interface =============================
public:
	void BindDelegates(AAmmunitionBase* ammuntion) override;

	//=== Information ========================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	FName GetName() { return CoreInformation.Name; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	FString GetDescription() { return CoreInformation.Description; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	UTexture2D* GetIcon() { return CoreInformation.Icon; }

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	bool CheckAmmunitionType(TSubclassOf<AAmmunitionBase> ammunitionClass);

protected:
	FCoreInformation CoreInformation;

	//=== Firing =============================================
public:
	void SetFirePosition(USceneComponent* firePosition) { FirePosition = firePosition; }
	
	void SetFireHeld(bool state);

protected:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fire"))
	void Fire();

	virtual void FireCore() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Fire Core"))
	void BP_FireCore();
	void BP_FireCore_Implementation() {}

	virtual void OnFireHeld() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On Fire Held"))
	void BP_OnFireHeld();
	void BP_OnFireHeld_Implementation() {}

	virtual void OnFireReleased() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On Fire Released"))
	void BP_OnFireReleased();
	void BP_OnFireReleased_Implementation() {}

protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Cores")
	AAmmunitionBase* SpawnAmmunition(FVector location, FRotator rotation);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Cores")
	AAmmunitionBase* SpawnAmmunitionAtFirePosition();

	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	USceneComponent* GetFirePosition() { return FirePosition; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	bool IsFiring() { return FireHeld; }

private:
	UPROPERTY()
	USceneComponent* FirePosition;

	bool FireHeld;

	//=== Modifiers ==========================================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void AddModifier(FString name, UModifierBase* modifier);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void RemoveModifier(FString name);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void RemoveAllModifiers();

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	UModifierBase* GetModifier(FString name);

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	TArray<FString> GetModifierNames();

private:
	UPROPERTY()
	TMap<FString, UModifierBase*> Modifiers;

	//=== Parameters =========================================
	/// Damage
private:
	const FString DamageParameter = "Damage";

protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void InitialiseDamageParameter(FString displayName, float defaultValue, float minimumValue, float maxiumumValue, float increment);

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetDamageParameterValue();

	/// Speed
private:
	const FString SpeedParameter = "Speed";

protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void InitialiseSpeedParameter(FString displayName, float defaultValue, float minimumValue, float maxiumumValue, float increment);

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetSpeedParameterValue();

	/// Size multiplier
private:
	const FString SizeMultiplierParameter = "SizeMultiplier";

protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void InitialiseSizeMultiplierParameter(FString displayName, float defaultValue, float minimumValue, float maxiumumValue, float increment);

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetSizeMultiplierParameterValue();

	/// Lifetime
private:
	const FString LifetimeParameter = "Lifetime";

protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void InitialiseLifetimeParameter(FString displayName, float defaultValue, float minimumValue, float maxiumumValue, float increment);

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetLifetimeParameterValue();

};

//==============================================================================================================