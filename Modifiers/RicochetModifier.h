//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Modifiers/ModifierBase.h"

//==============================================================================================================

#include "RicochetModifier.generated.h"

//==============================================================================================================
/*
 * RICOCHET Modifier
*/
//==============================================================================================================

UCLASS()
class PROJECT_NIOBIUM_API URicochetModifier : public UModifierBase
{
	GENERATED_BODY()
	
public:
	URicochetModifier();
	~URicochetModifier();

	//=== UModifierBase Interface ============================
public:
	void InitialiseModifier() override;
	void UpdateModifier(float deltaTime) override;

	//=== UNiobiumBase Interface =============================
protected:
	void AmmunitionFired(AAmmunitionBase* ammunition) override;
	void AmmunitionHit(AAmmunitionBase* ammunition, FHitResult hitResult) override;
	void AmmunitionExpire(AAmmunitionBase* ammunition) override;

	void ExpireOnHitConflict(AAmmunitionBase* ammunition, bool value) override;

protected:
	void InitialiseParameters() override;

	//=== Function============================================
private:
	bool DebugReflections;

	struct RicochetInfo
	{
		int ReflectCounter;

		RicochetInfo(int reflectCount)
		{
			ReflectCounter = reflectCount;
		}
	};

	TMap<AAmmunitionBase*, RicochetInfo> ShotInfo;

	// Parameters
	/// Reflections
private:
	const FString ReflectionsParameter = "Reflections";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	int GetReflectionsValue();

};

//==============================================================================================================