//==============================================================================================================

#include "RicochetModifier.h"

#include "Math/UnrealMathUtility.h"

#include "Kismet/KismetSystemLibrary.h"

//==============================================================================================================

URicochetModifier::URicochetModifier()
	: UModifierBase()
{ }

//==============================================================================================================

URicochetModifier::~URicochetModifier()
{ }

//==============================================================================================================

void URicochetModifier::InitialiseModifier()
{
	DebugReflections = false;
	if (DebugReflections)
	{
		UE_LOG(NiobiumModifier, Display, TEXT("%s: Debug Reflections: ENABLED"), *GetName().ToString());
	}
}

//==============================================================================================================

void URicochetModifier::UpdateModifier(float deltaTime)
{

}

//==============================================================================================================

void URicochetModifier::AmmunitionFired(AAmmunitionBase* ammunition)
{
	ShotInfo.Add(ammunition, RicochetInfo(GetReflectionsValue()));
	ammunition->RequestExpireOnHitChange(false);
}

//==============================================================================================================

void URicochetModifier::AmmunitionHit(AAmmunitionBase* ammunition, FHitResult hitResult)
{
	if (ShotInfo.Contains(ammunition))
	{
		// If we're out of reflections, detonate
		if (ShotInfo[ammunition].ReflectCounter == 0)
		{
			ammunition->RequestExpireOnHitChange(true);
		}

		if (ShotInfo[ammunition].ReflectCounter > 0)
		{
			ShotInfo[ammunition].ReflectCounter--;

			// Reflect shot
			FVector impactPoint = hitResult.Location;
			FVector incoming = impactPoint - ammunition->GetLastLocation();
			FVector planeNormal = hitResult.Normal;
			incoming.Normalize();

			FVector reflect = FMath::GetReflectionVector(incoming, planeNormal);
			FRotator reflectRot = reflect.Rotation();
			ammunition->SetRotation(reflectRot);

			if (DebugReflections)
			{
				UKismetSystemLibrary::DrawDebugFrustum(this, FTransform(ammunition->GetLastLocation()), FColor::White, 10.0f, 2.0f);

				UKismetSystemLibrary::DrawDebugLine(this, impactPoint, impactPoint + (-incoming * 30.0f), FColor::Red, 10.0f, 2.0f);
				UKismetSystemLibrary::DrawDebugLine(this, impactPoint, impactPoint + (planeNormal * 30.0f), FColor::Yellow, 10.0f, 2.0f);
				UKismetSystemLibrary::DrawDebugLine(this, impactPoint, impactPoint + (reflect * 30.0f), FColor::Blue, 10.0f, 2.0f);
			}

			UE_LOG(NiobiumModifier, Display, TEXT("%s: Shot reflected (Reflections left: %i)"), *GetName().ToString(), ShotInfo[ammunition].ReflectCounter);
		}
	}
}

//==============================================================================================================

void URicochetModifier::AmmunitionExpire(AAmmunitionBase* ammunition)
{
	if (ShotInfo.Contains(ammunition))
	{
		ShotInfo.Remove(ammunition);
	}
}

//==============================================================================================================

void URicochetModifier::ExpireOnHitConflict(AAmmunitionBase* ammunition, bool value)
{
	if (value)
	{
		if (ShotInfo.Contains(ammunition))
		{
			if (ShotInfo[ammunition].ReflectCounter > 0)
			{
				ammunition->VetoExpireOnHitChange("Ricochet has reflections left");
			}
		}
	}
}

//==============================================================================================================

void URicochetModifier::InitialiseParameters()
{
	UModifierBase::InitialiseParameters();

	AddParameter(ReflectionsParameter, NewObject<UIntegerParameter>());
	if (UIntegerParameter* param = GetParameter<UIntegerParameter>(ReflectionsParameter))
	{
		param->Initialise("Reflection Limit", 3, 1, 6, 1);
	}
}

//==============================================================================================================

int URicochetModifier::GetReflectionsValue()
{
	UIntegerParameter* param = GetParameter<UIntegerParameter>(ReflectionsParameter);
	return (param ? param->GetValue() : 0);
}

//==============================================================================================================