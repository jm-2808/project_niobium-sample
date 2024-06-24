//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Ammunition/AmmunitionBase.h"
#include "BeamAmmunition.generated.h"

//==============================================================================================================

UCLASS(Abstract, Blueprintable)
class PROJECT_NIOBIUM_API ABeamAmmunition : public AAmmunitionBase
{
	GENERATED_BODY()
	
public:
	ABeamAmmunition();
	~ABeamAmmunition();

	//=== AAmmunitionBase Interface ==========================
protected:
	void InitialiseAmmunition() override;
	void UpdateAmmunition(float deltaTime) override;
	void AmmunitionHit(FHitResult hitResult) override;
	void AmmunitionExpire() override;

	void MoveAmmunition(FVector direction, float deltaTime) override;
	void RotateAmmunition(FRotator rotation) override;

	FVector GetAmmunitionLocation() override;
	FRotator GetAmmunitionRotation() override;

	//=== Beam function ======================================
private:
	void UpdateBeamLocation(float deltaTime);
	void UpdateBeamHits(float deltaTime);
	void DebugDrawBeam();

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetMaxBeamLength() { return GetSpeed() * GetLifetime(); }
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetCurrentBeamLength();

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetBeamStartLocation() { return GetSegmentStartLocation(0); }
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetBeamEndLocation() { return GetSegmentEndLocation(GetSegmentIndex()); }

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetSegmentStartLocation(int index);
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetSegmentEndLocation(int index);
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FRotator GetSegmentRotation(int index);
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetSegmentDirection(int index);
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetSegmentLength(int index);

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	bool HasReachedMaxLength() { return GetCurrentBeamLength() >= GetMaxBeamLength(); }

	void AddSegment(FRotator rotation);

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On Add Segment"))
	void BP_AddSegment();
	void BP_AddSegment_Implementation() {}

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	int GetSegmentIndex() { return (BeamSegments.Num() - 1); }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void EnableDebug() { DebugEnabled = true; }

private:
	struct BeamSegment
	{
		FVector EndLocation;

		FRotator Rotation;

		float Length;

		BeamSegment(FVector startLocation, FRotator rotation)
		{
			EndLocation = startLocation;

			Rotation = rotation;

			Length = 0.0f;
		}

		void UpdateLength(float value, FVector startLocation)
		{
			Length += value;
			EndLocation = startLocation + (Rotation.Vector() * Length);
		}
	};

	TArray<BeamSegment> BeamSegments;
	int SegmentIndex;

	bool DebugEnabled;
};

//==============================================================================================================