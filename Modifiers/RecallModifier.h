//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Modifiers/ModifierBase.h"

//==============================================================================================================

#include "RecallModifier.generated.h"

//==============================================================================================================
/*
 * RECALL Modifier
*/
//==============================================================================================================

UCLASS()
class PROJECT_NIOBIUM_API URecallModifier : public UModifierBase
{
	GENERATED_BODY()
	
public:
	URecallModifier();
	~URecallModifier();

	//=== UModifierBase Interface ============================
public:
	void InitialiseModifier() override;
	void UpdateModifier(float deltaTime) override;

	//=== UNiobiumBase Interface =============================
protected:
	void AmmunitionFired(AAmmunitionBase* ammunition) override;
	void AmmunitionUpdate(AAmmunitionBase* ammunition, float deltaTime) override;
	void AmmunitionHit(AAmmunitionBase* ammunition, FHitResult hitResult) override;
	void AmmunitionExpire(AAmmunitionBase* ammunition) override;

	void ExpireOnHitConflict(AAmmunitionBase* ammunition, bool value) override;

protected:
	void InitialiseParameters() override;

	//=== Function============================================
private:
	void Rewind(AAmmunitionBase* ammunition);
	bool IsRewinding(AAmmunitionBase* ammunition);

	FVector GetRecallPoint(AAmmunitionBase* ammunition);

	void RewindLogMessage(AAmmunitionBase* ammunition);

private:
	struct RecallInfo
	{
		bool Rewinding;
		int HitIndex;

		RecallInfo()
		{
			Rewinding = false;
		}

		void BeginRewind(int hits)
		{
			Rewinding = true;
			HitIndex = hits - 1;
		}

		void DecrementIndex() { HitIndex--; }
	};

	TMap<AAmmunitionBase*, RecallInfo> ShotInfo;

	// Parameters
	/// Recall Multiplier
private:
	const FString RecallMultiplierParameter = "RecallMultiplier";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetRecallMultiplierValue();

};

//==============================================================================================================