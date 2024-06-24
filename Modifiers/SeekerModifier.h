//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Modifiers/ModifierBase.h"

//==============================================================================================================

#include "SeekerModifier.generated.h"

//==============================================================================================================
/*
 * SEEKER Modifier
*/
//==============================================================================================================

UCLASS()
class PROJECT_NIOBIUM_API USeekerModifier : public UModifierBase
{
	GENERATED_BODY()

public:
	USeekerModifier();
	~USeekerModifier();


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

protected:
	void InitialiseParameters() override;

	//=== Function============================================
private:
	void SetTracer(AActor* target);
	void ClearTracer();

	void AddSeeker(AAmmunitionBase* ammunition);
	void RemoveSeeker(AAmmunitionBase* ammunition);
	bool IsSeeker(AAmmunitionBase* ammunition);

private:
	struct SeekerInfo
	{
		AActor* Target;
		bool Recalibrating;
		float RecalibrationTimer;

		SeekerInfo()
		{
			Recalibrating = false;
			RecalibrationTimer = 0.0f;
		}

		void Recalibrate(float deltaTime)
		{
			RecalibrationTimer -= deltaTime;
			if (RecalibrationTimer <= 0.0f)
			{
				Recalibrating = false;
			}
		}
	};

	AActor* Tracer;
	TMap<AAmmunitionBase*, SeekerInfo> Seekers;
	int SeekerCounter;

	float TracerLifetime;

	// Parameters
	/// Tracer Lifetime
private:
	const FString TracerLifetimeParameter = "Tracer_Lifetime";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetTracerLifetimeValue();

	/// Seeker Count
private:
	const FString SeekerCountParameter = "Seeker_Count";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	int GetSeekerCountValue();

	/// Seeker Arc Speed
private:
	const FString SeekerArcSpeedParameter = "Seeker_ArcSpeed";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetSeekerArcSpeedValue();

	/// Seeker Recalibration
private:
	const FString SeekerRecalibrationParameter = "Seeker_Recalibration";

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Modifiers")
	float GetSeekerRecalibrationValue();

};

//==============================================================================================================