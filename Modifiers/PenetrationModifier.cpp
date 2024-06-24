//==============================================================================================================

#include "PenetrationModifier.h"

//==============================================================================================================

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

//==============================================================================================================

UPenetrationModifier::UPenetrationModifier()
	: UModifierBase()
{ }

//==============================================================================================================

UPenetrationModifier::~UPenetrationModifier()
{ }

//==============================================================================================================

void UPenetrationModifier::InitialiseModifier()
{
	AdvancedLogic = false;
	if (AdvancedLogic)
	{
		UE_LOG(NiobiumModifier, Display, TEXT("%s: Advanced logic: ENABLED"), *GetName().ToString());
	}
}

//==============================================================================================================

void UPenetrationModifier::UpdateModifier(float deltaTime)
{

}

//==============================================================================================================

void UPenetrationModifier::AmmunitionFired(AAmmunitionBase* ammunition)
{
	ammunition->RequestExpireOnHitChange(false);

	ShotInfo.Add(ammunition, PenetrationInfo(GetDistanceValue() * UnitsToMeters));
}

//==============================================================================================================

void UPenetrationModifier::AmmunitionUpdate(AAmmunitionBase* ammunition, float deltaTime)
{
	if (ShotInfo.Contains(ammunition))
	{
		if (AdvancedLogic)
		{
			AdvancedPenetrationLogic(ammunition, deltaTime);
		}
		else
		{
			BasicPenetrationLogic(ammunition, deltaTime);
		}
	}
}

//==============================================================================================================

void UPenetrationModifier::AmmunitionBeginOverlap(AAmmunitionBase* ammunition, AActor* overlappedActor)
{
	if (ShotInfo.Contains(ammunition))
	{
		ShotInfo[ammunition].AddOverlap(overlappedActor);

		UE_LOG(NiobiumModifier, Display, TEXT("%s: Shot entered object (Distance left: %f)"), *GetName().ToString(), ShotInfo[ammunition].DistanceLeft);
	}
}

//==============================================================================================================

void UPenetrationModifier::AmmunitionEndOverlap(AAmmunitionBase* ammunition, AActor* overlappedActor)
{
	if (ShotInfo.Contains(ammunition))
	{
		ShotInfo[ammunition].RemoveOverlap(overlappedActor);

		UE_LOG(NiobiumModifier, Display, TEXT("%s: Shot exited object (Distance left: %f)"), *GetName().ToString(), ShotInfo[ammunition].DistanceLeft);
	}
}

//==============================================================================================================

void UPenetrationModifier::AmmunitionExpire(AAmmunitionBase* ammunition)
{
	if (ShotInfo.Contains(ammunition))
	{
		ShotInfo.Remove(ammunition);
	}
}

//==============================================================================================================

void UPenetrationModifier::ExpireOnHitConflict(AAmmunitionBase* ammunition, bool value)
{
	if (value)
	{
		ammunition->VetoExpireOnHitChange("Penetration must have ExpireOnHit disabled");
	}
}

//==============================================================================================================

void UPenetrationModifier::InitialiseParameters()
{
	UModifierBase::InitialiseParameters();

	AddParameter(DistanceParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(DistanceParameter))
	{
		param->Initialise("Distance (meters)", 3.0f, 0.0f, 10.0f, 0.2f);
	}
}

//==============================================================================================================

void UPenetrationModifier::BasicPenetrationLogic(AAmmunitionBase* ammunition, float deltaTime)
{
	if (ShotInfo[ammunition].IsInsideObject())
	{
		// Check penetration distance
		FVector distanceVector = ammunition->GetLocation() - ammunition->GetLastLocation();
		float distance = distanceVector.Size();
		ShotInfo[ammunition].DistanceLeft -= (distance > 0.0f ? distance : (distance * -1.0f));

		if (ShotInfo[ammunition].DistanceLeft <= 0.0f)
		{
			ShotInfo[ammunition].DistanceLeft = 0.0f;
			ammunition->Expire();
			UE_LOG(NiobiumModifier, Display, TEXT("%s: Shot detonated in object"), *GetName().ToString());
		}
	}
}

void UPenetrationModifier::AdvancedPenetrationLogic(AAmmunitionBase* ammunition, float deltaTime)
{
	if (ShotInfo[ammunition].IsInsideObject())
	{
		// Check the shot is visibly inside the object and not just overlapping its collider
		FHitResult hit;
		bool visiblyInside = ammunition->GetWorld()->LineTraceSingleByChannel(hit, ammunition->GetLastLocation(), ammunition->GetLocation(), ECC_Visibility);
		if (visiblyInside)
		{
			// Check penetration distance
			FVector distanceVector = ammunition->GetLocation() - ammunition->GetLastLocation();
			float distance = distanceVector.Size();
			ShotInfo[ammunition].DistanceLeft -= (distance > 0.0f ? distance : (distance * -1.0f));

			if (ShotInfo[ammunition].DistanceLeft <= 0.0f)
			{
				ShotInfo[ammunition].DistanceLeft = 0.0f;
				ammunition->Expire();
				UE_LOG(NiobiumModifier, Display, TEXT("%s: Shot detonated in object"), *GetName().ToString());
			}
		}
	}
}

//==============================================================================================================

float UPenetrationModifier::GetDistanceValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(DistanceParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================