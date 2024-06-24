//==============================================================================================================

#pragma once

//==============================================================================================================

#include "Project_Niobium.h"

//==============================================================================================================

#include "NiobiumSystem/NiobiumDataAsset.h"

#include "NiobiumSystem/Cores/CoreBase.h"
#include "NiobiumSystem/Modifiers/ModifierBase.h"

#include "Components/SceneComponent.h"

//==============================================================================================================

#include "Components/ActorComponent.h"
#include "NiobiumComponent.generated.h"

//==============================================================================================================

USTRUCT()
struct FNiobiumComponentConfig
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Name;

	UPROPERTY()
	TMap<FString, FString> Parameters;
};

USTRUCT()
struct FNiobiumConfig
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FNiobiumComponentConfig Core;

	UPROPERTY()
	TArray<FNiobiumComponentConfig> Modifiers;

};

//==============================================================================================================

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_NIOBIUM_API UNiobiumComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNiobiumComponent();

	//=== UActorComponent interface ==========================
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//=== System Information =================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|System")
	UNiobiumDataAsset* GetSystemInformation() { return SystemInformation; }

private:
	void InitialiseCompiler();
	void InitialiseComponents();

private:
	UPROPERTY()
	UNiobiumDataAsset* SystemInformation;

	//=== Cores/Modifiers ====================================
	// Initialisation/Access
public:
	UCoreBase* InitialiseCore(FName name);
	UModifierBase* InitialiseModifier(FName name);

	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	UCoreBase* GetCore(FName name);
	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	TArray<FName> GetCoreNames();

	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	UModifierBase* GetModifier(FName name);
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	TArray<FName> GetModifierNames();

private:
	UPROPERTY()
	TMap<FName, UCoreBase*> InitialisedCores;
	UPROPERTY()
	TMap<FName, UModifierBase*> InitialisedModifiers;
	
	// Installation
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Cores")
	UCoreBase* GetInstalledCore() { return InstalledCore; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Cores")
	void InstallCore(UCoreBase* core);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void AddModifier(UModifierBase* modifier);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Modifiers")
	void RemoveModifier(UModifierBase* modifier);

private:
	UPROPERTY()
	UCoreBase* InstalledCore;

	// Serialisation
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|System")
	FString GenerateConfig();
	UFUNCTION(BlueprintCallable, Category = "Niobium|System")
	void LoadConfig(FString configString);

	UFUNCTION()
	void GenerateConfig_UIEvent();
private:
	FString SerialiseConfig(FNiobiumConfig config);
	FNiobiumConfig DeserialiseConfig(FString jsonString);

	//=== Firing =============================================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|System")
	void SetFirePosition(USceneComponent* firePosition);

	UFUNCTION(BlueprintCallable, Category = "Niobium|System")
	void FirePressed();
	UFUNCTION(BlueprintCallable, Category = "Niobium|System")
	void FireReleased();

private:
	UPROPERTY()
	USceneComponent* FirePosition;

	//=== User Interface =====================================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Compiler")
	void SetCompilerVisible(bool state);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Compiler")
	void ToggleCompilerVisible() { SetCompilerVisible(!CompilerVisible); }

	UFUNCTION(BlueprintPure, Category = "Niobium|Compiler")
	bool IsCompilerVisible() { return CompilerVisible; }

private:
	UPROPERTY()
	UNiobiumCompiler* CompilerUI;

	bool CompilerVisible;

};

//==============================================================================================================