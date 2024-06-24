//==============================================================================================================

#pragma once

//==============================================================================================================

#include "NiobiumSystem/Ammunition/AmmunitionBase.h"
#include "ProjectileAmmunition.generated.h"

//==============================================================================================================

UCLASS(Abstract, Blueprintable)
class PROJECT_NIOBIUM_API AProjectileAmmunition : public AAmmunitionBase
{
	GENERATED_BODY()
	
public:
	AProjectileAmmunition();
	~AProjectileAmmunition();

	//=== AAmmunitionBase Interface ==========================
protected:
	void MoveAmmunition(FVector direction, float deltaTime) override;
	void RotateAmmunition(FRotator rotation) override;

};

//==============================================================================================================