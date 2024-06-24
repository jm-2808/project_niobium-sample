//==============================================================================================================

#pragma once

//==============================================================================================================

#include "Project_Niobium.h"

//==============================================================================================================

#include "NiobiumSystem/NiobiumBase.h"
#include "ModifierBase.generated.h"

//==============================================================================================================
class UModifierBase;

USTRUCT(BlueprintType)
struct FModifierInformation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UModifierBase> Class;
};

//==============================================================================================================
UCLASS(Abstract, Blueprintable)
class PROJECT_NIOBIUM_API UModifierBase : public UNiobiumBase
{
	GENERATED_BODY()
	
public:
	UModifierBase();
	~UModifierBase();

	//=== Basic Function =====================================
public:
	void Initialise(FModifierInformation information);

	void Update(float deltaTime);

protected:
	virtual void InitialiseModifier() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Initialise Modifier"))
	void BP_InitialiseModifier();
	void BP_InitialiseModifier_Implementation() {}

	virtual void UpdateModifier(float deltaTime) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Update Modifier"))
	void BP_UpdateModifier(float deltaTime);
	void BP_UpdateModifier_Implementation(float deltaTime) {}


	//=== UNiobiumBase Interface =============================
public:
	void BindDelegates(AAmmunitionBase* ammuntion) override;

	//=== Information ========================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	FName GetName() { return ModifierInformation.Name; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	FString GetDescription() { return ModifierInformation.Description; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	UTexture2D* GetIcon() { return ModifierInformation.Icon; }

protected:
	FModifierInformation ModifierInformation;

};

//==============================================================================================================