//==============================================================================================================

#include "RecallModifier.h"

//==============================================================================================================

#include "Kismet/KismetMathLibrary.h"

//==============================================================================================================

URecallModifier::URecallModifier()
	: UModifierBase()
{ }

//==============================================================================================================

URecallModifier::~URecallModifier()
{ }

//==============================================================================================================

void URecallModifier::InitialiseModifier()
{

}

//==============================================================================================================

void URecallModifier::UpdateModifier(float deltaTime)
{

}

//==============================================================================================================

void URecallModifier::AmmunitionFired(AAmmunitionBase* ammunition)
{
	ShotInfo.Add(ammunition, RecallInfo());
}

//==============================================================================================================

void URecallModifier::AmmunitionUpdate(AAmmunitionBase* ammunition, float deltaTime)
{
	if (ShotInfo.Contains(ammunition))
	{
		if (ammunition->GetRemainingLifetime() <= 0.0f && !IsRewinding(ammunition))
		{
			Rewind(ammunition);
		}

		if (IsRewinding(ammunition))
		{
			FVector recallPoint = GetRecallPoint(ammunition);
			ammunition->RotateToFaceLocation(recallPoint);
		}
	}
}

//==============================================================================================================

void URecallModifier::AmmunitionHit(AAmmunitionBase* ammunition, FHitResult hitResult)
{
	if (IsRewinding(ammunition))
	{
		FVector recallPoint = GetRecallPoint(ammunition);
		if (hitResult.Location.Equals(recallPoint, 4.0f))
		{
			ShotInfo[ammunition].DecrementIndex();

			RewindLogMessage(ammunition);
		}
	}
}

//==============================================================================================================

void URecallModifier::AmmunitionExpire(AAmmunitionBase* ammunition)
{
	if (ShotInfo.Contains(ammunition))
	{
		if (!IsRewinding(ammunition))
		{
			ammunition->CancelExpire();
			Rewind(ammunition);
		}
		else
		{
			ShotInfo.Remove(ammunition);
		}
	}
}

//==============================================================================================================

void URecallModifier::ExpireOnHitConflict(AAmmunitionBase* ammunition, bool value)
{
	if (IsRewinding(ammunition) && value)
	{
		ammunition->VetoExpireOnHitChange("Recall must have ExpireOnHit disabled while rewinding");
	}
}

//==============================================================================================================

void URecallModifier::InitialiseParameters()
{
	UModifierBase::InitialiseParameters();

	AddParameter(RecallMultiplierParameter, NewObject<UFloatParameter>()); // Eg. RecallMultiplier = 2 -> Lifetime * 0.5, Speed * 2
	if (UFloatParameter* param = GetParameter<UFloatParameter>(RecallMultiplierParameter))
	{
		param->Initialise("Recall Multiplier", 2.0f, 1.0f, 3.0f, 0.1f);
	}
}

//==============================================================================================================

void URecallModifier::Rewind(AAmmunitionBase* ammunition)
{
	if (ShotInfo.Contains(ammunition))
	{
		ammunition->RequestExpireOnHitChange(false);

		ShotInfo[ammunition].BeginRewind(ammunition->GetNumHitLocations());

		float shotLifetime = (ammunition->GetLifetime() - ammunition->GetRemainingLifetime());
		ammunition->AddLifetime(-(ammunition->GetRemainingLifetime())); // Empty the lifetime of the shot
		ammunition->AddLifetime(shotLifetime / GetRecallMultiplierValue()); // Set it to the recall multiplied value

		ammunition->UpdateSpeedMultiplier(GetRecallMultiplierValue() - 1.0f);

		FVector recallPoint = GetRecallPoint(ammunition);
		if (ammunition->GetLocation().Equals(recallPoint, 10.0f))
		{
			ShotInfo[ammunition].DecrementIndex();

			recallPoint = GetRecallPoint(ammunition);
		}

		ammunition->RotateToFaceLocation(recallPoint);

		RewindLogMessage(ammunition);
	}
}

//==============================================================================================================

bool URecallModifier::IsRewinding(AAmmunitionBase* ammunition)
{
	if (ShotInfo.Contains(ammunition))
	{
		return ShotInfo[ammunition].Rewinding;
	}
	else
	{
		return false;
	}
}

//==============================================================================================================

FVector URecallModifier::GetRecallPoint(AAmmunitionBase* ammunition)
{
	return ((ShotInfo[ammunition].HitIndex >= 0) ? ammunition->GetHitLocation(ShotInfo[ammunition].HitIndex) : ammunition->GetFirePosition()->GetComponentLocation());
}

//==============================================================================================================

void URecallModifier::RewindLogMessage(AAmmunitionBase* ammunition)
{
	int idx = ShotInfo[ammunition].HitIndex;
	FVector recallPoint = GetRecallPoint(ammunition);

	if(idx >= 0)
	{
		UE_LOG(NiobiumModifier, Display, TEXT("%s: Shot rewinding to: %f %f %f [Index: %i]"), *GetName().ToString(), recallPoint.X, recallPoint.Y, recallPoint.Z, idx);
	}
	else
	{
		UE_LOG(NiobiumModifier, Display, TEXT("%s: Shot rewinding to: %f %f %f"), *GetName().ToString(), recallPoint.X, recallPoint.Y, recallPoint.Z);
	}
}

//==============================================================================================================

float URecallModifier::GetRecallMultiplierValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(RecallMultiplierParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================