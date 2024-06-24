//==============================================================================================================

#pragma once

//==============================================================================================================

#include "Project_Niobium.h"

//==============================================================================================================

#include "NiobiumSystem/Cores/CoreBase.h"
#include "NiobiumSystem/Modifiers/ModifierBase.h"

#include "NiobiumSystem/Utility/Widgets/NiobiumCompiler.h"

//==============================================================================================================

#include "Engine/DataAsset.h"
#include "NiobiumDataAsset.generated.h"

//==============================================================================================================

UCLASS(BlueprintType)
class PROJECT_NIOBIUM_API UNiobiumDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCoreInformation> Cores;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FModifierInformation> Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UNiobiumCompiler> Compiler;

public:
	UFUNCTION(BlueprintPure, Category = "Niobium")
	int GetCoreIndexByName(FName name);

	UFUNCTION(BlueprintPure, Category = "Niobium")
	int GetModifierIndexByName(FName name);

	UFUNCTION(BlueprintPure, Category = "Niobium")
	TArray<FName> GetCoreNames();

	UFUNCTION(BlueprintPure, Category = "Niobium")
	TArray<FName> GetModifierNames();
};

//==============================================================================================================