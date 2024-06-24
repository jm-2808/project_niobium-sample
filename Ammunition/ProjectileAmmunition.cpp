//==============================================================================================================

#include "ProjectileAmmunition.h"

//==============================================================================================================

AProjectileAmmunition::AProjectileAmmunition()
	: AAmmunitionBase()
{ }

//==============================================================================================================

AProjectileAmmunition::~AProjectileAmmunition()
{ }

//==============================================================================================================

void AProjectileAmmunition::MoveAmmunition(FVector direction, float deltaTime)
{
	FVector movement = direction * (GetSpeed() * deltaTime);
	FVector newPos = GetActorLocation() + movement;

	SetActorLocation(newPos);
}

//==============================================================================================================

void AProjectileAmmunition::RotateAmmunition(FRotator rotation)
{
	SetActorRotation(rotation);
}

//==============================================================================================================