//==============================================================================================================

#include "BeamAmmunition.h"

//==============================================================================================================

#include "Kismet/KismetMathLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

//==============================================================================================================

ABeamAmmunition::ABeamAmmunition()
	: AAmmunitionBase()
{ }

//==============================================================================================================

ABeamAmmunition::~ABeamAmmunition()
{ }

//==============================================================================================================

void ABeamAmmunition::InitialiseAmmunition()
{
	SetTickLifetime(false);

	//AddSegment(FRotator(0.0f));
	AddSegment(GetActorRotation());
}

//==============================================================================================================

void ABeamAmmunition::UpdateAmmunition(float deltaTime)
{
	//UpdateBeamLocation(deltaTime);
	UpdateBeamHits(deltaTime);

	if(DebugEnabled)
	{
		DebugDrawBeam();
	}
}

//==============================================================================================================

void ABeamAmmunition::AmmunitionHit(FHitResult hitResult)
{
	if (GetExpireOnHit())
	{
		//int index = GetSegmentIndex();
		//FVector dist = GetSegmentEndLocation(index) - hitResult.Location;
		//BeamSegments[index].UpdateLength(dist.Size(), GetSegmentStartLocation(index));

		//UE_LOG(NiobiumAmmunition, Display, TEXT("Beam Segment blocked"));
	}
}

//==============================================================================================================

void ABeamAmmunition::AmmunitionExpire()
{
	CancelExpire();
}

//==============================================================================================================

void ABeamAmmunition::MoveAmmunition(FVector direction, float deltaTime)
{
	if (!HasReachedMaxLength())
	{
		float currentLength = GetCurrentBeamLength();
		float maxLength = GetMaxBeamLength();

		float distanceToMove = (GetSpeed() * deltaTime);
		float distanceToEnd = GetMaxBeamLength() - GetCurrentBeamLength();
		float distance = FMath::Min(distanceToMove, distanceToEnd);

		int index = GetSegmentIndex();
		BeamSegments[index].UpdateLength(distance, GetSegmentStartLocation(index));
	}
}

//==============================================================================================================

void ABeamAmmunition::RotateAmmunition(FRotator rotation)
{
	if (!HasReachedMaxLength())
	{
		AddSegment(rotation);
	}
}

//==============================================================================================================

FVector ABeamAmmunition::GetAmmunitionLocation()
{
	return GetBeamEndLocation();
}

//==============================================================================================================

FRotator ABeamAmmunition::GetAmmunitionRotation()
{
	return (BeamSegments.Num() > 0) ? BeamSegments[GetSegmentIndex()].Rotation : GetActorRotation();
}

//==============================================================================================================

void ABeamAmmunition::UpdateBeamLocation(float deltaTime)
{
	SetActorLocation(GetFirePosition()->GetComponentLocation());
	SetActorRotation(GetFirePosition()->GetComponentRotation());
}

//==============================================================================================================

void ABeamAmmunition::UpdateBeamHits(float deltaTime)
{
	for (int i = 0; i < BeamSegments.Num(); ++i)
	{
		FHitResult hit;
		if (DetectHit(GetSegmentStartLocation(i), GetSegmentEndLocation(i), hit))
		{
			// Damage actors which are not currently on cooldown for damage
			AActor* hitActor = hit.GetActor();
			if (CanDamageTarget(hitActor, true))
			{
				StartDamageCooldown(hitActor);
				DamageTarget(hitActor);
			}
		}
	}
}

//==============================================================================================================

void ABeamAmmunition::DebugDrawBeam()
{
	for (int i = 0; i < BeamSegments.Num(); ++i)
	{
		UKismetSystemLibrary::DrawDebugLine(this, GetSegmentStartLocation(i), GetSegmentEndLocation(i), FColor::Red, 0.01f, 2.0f);
	}
}

//==============================================================================================================

float ABeamAmmunition::GetCurrentBeamLength()
{
	float length = 0;
	for (int i = 0; i < BeamSegments.Num(); ++i)
	{
		length += BeamSegments[i].Length;
	}
	return length;
}

//==============================================================================================================

FVector ABeamAmmunition::GetSegmentStartLocation(int index)
{
	if (index >= 0 || index < BeamSegments.Num())
	{
		if (index == 0)
		{
			return GetActorLocation();
		}
		else
		{
			return BeamSegments[index - 1].EndLocation;
		}
	}
	else
	{
		return GetActorLocation();
	}
}

//==============================================================================================================

FVector ABeamAmmunition::GetSegmentEndLocation(int index)
{
	if (index >= 0 || index < BeamSegments.Num())
	{
		return BeamSegments[index].EndLocation;
	}
	else
	{
		return GetActorLocation();
	}
}

//==============================================================================================================

FRotator ABeamAmmunition::GetSegmentRotation(int index)
{
	if (index >= 0 || index < BeamSegments.Num())
	{
		return BeamSegments[index].Rotation;
	}
	else
	{
		return GetActorRotation();
	}
}

//==============================================================================================================

FVector ABeamAmmunition::GetSegmentDirection(int index)
{
	if (index >= 0 || index < BeamSegments.Num())
	{
		return BeamSegments[index].Rotation.Vector();
		//return UKismetMathLibrary::ComposeRotators(GetActorRotation(), BeamSegments[index].Rotation).Vector();
	}
	else
	{
		return FVector::ForwardVector;
	}
}

//==============================================================================================================

float ABeamAmmunition::GetSegmentLength(int index)
{
	if (index >= 0 || index < BeamSegments.Num())
	{
		return BeamSegments[index].Length;
	}
	else
	{
		return 0.0f;
	}
}

//==============================================================================================================

void ABeamAmmunition::AddSegment(FRotator rotation)
{
	FVector location;
	if(BeamSegments.Num() > 0)
	{ 
		location = GetBeamEndLocation();
	}
	else
	{
		location = GetActorLocation();
	}

	BeamSegments.Add(BeamSegment(location, rotation));

	BP_AddSegment();

	UE_LOG(NiobiumAmmunition, Display, TEXT("Beam Segment added [Rotation %f %f %f]"), rotation.Pitch, rotation.Yaw, rotation.Roll);
}

//==============================================================================================================