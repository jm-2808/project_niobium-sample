//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Modifiers/ModifierBase.h"

//==============================================================================================================

#include "PenetrationModifier.generated.h"

//==============================================================================================================
/*
 * PENETRATION Modifier
*/
//==============================================================================================================

UCLASS()
class PROJECT_NIOBIUM_API UPenetrationModifier : public UModifierBase
{
	GENERATED_BODY()

public:
	UPenetrationModifier();
	~UPenetrationModifier();

	//=== UModifierBase Interface ============================
public:
	void InitialiseModifier() override;
	void UpdateModifier(float deltaTime) override;

	//=== UNiobiumBase Interface =============================
protected:
	void AmmunitionFired(AAmmunitionBase* ammunition) override;
	void AmmunitionUpdate(AAmmunitionBase* ammunition, float deltaTime) override;
	void AmmunitionBeginOverlap(AAmmunitionBase* ammunition, AActor* overlappedActor) override;
	void AmmunitionEndOverlap(AAmmunitionBase* ammunition, AActor* overlappedActor) override;
	void AmmunitionExpire(AAmmunitionBase* ammunition) override;

	void ExpireOnHitConflict(AAmmunitionBase* ammunition, bool value) override;

protected:
	void InitialiseParameters() override;

	//=== Function============================================
private:
	void BasicPenetrationLogic(AAmmunitionBase* ammunition, float deltaTime);
	void AdvancedPenetrationLogic(AAmmunitionBase* ammunition, float deltaTime);

private:
	bool AdvancedLogic;

	struct PenetrationInfo
	{
		float DistanceLeft;
		TArray<AActor*> OverlappingActors;

		PenetrationInfo(float distance)
		{
			DistanceLeft = distance;
		}

		void AddOverlap(AActor* actor)
		{
			if (!OverlappingActors.Contains(actor))
			{
				OverlappingActors.Add(actor);
			}
		}
		void RemoveOverlap(AActor* actor)
		{
			if (OverlappingActors.Contains(actor))
			{
				OverlappingActors.Remove(actor);
			}
		}

		bool IsInsideObject() { return (OverlappingActors.Num() > 0); }
	};

	TMap<AAmmunitionBase*, PenetrationInfo> ShotInfo;
	const float UnitsToMeters = 100.0f;

	// Parameters
	/// Distance
private:
	const FString DistanceParameter = "Distance";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetDistanceValue();
};

//==============================================================================================================