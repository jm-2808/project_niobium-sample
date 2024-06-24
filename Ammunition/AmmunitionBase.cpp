//==============================================================================================================

#include "AmmunitionBase.h"

//==============================================================================================================

#include "Kismet/KismetMathLibrary.h"

#include "Engine/World.h"

#include "NiobiumSystem/NiobiumDamageInterface.h"

//==============================================================================================================

AAmmunitionBase::AAmmunitionBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

//==============================================================================================================

AAmmunitionBase::~AAmmunitionBase()
{ }

//==============================================================================================================

void AAmmunitionBase::BeginPlay()
{
	Super::BeginPlay();

	Origin = GetActorLocation();

	InitialiseDamage(0.0f);
	DamageTickInterval = 0.02f;

	InitialiseSpeed(0.0f);

	InitialiseSize(1.0f);

	InitialiseLifetime(0.0f);
	TickLifetime = false;

	HitTickInterval = 0.5f;

	Expired = false;

	InitialiseDetonateOnHit();
	InitialiseExpireOnHit();
}

//==============================================================================================================

void AAmmunitionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Expired)
	{
		DelayedExpire();
	}

	UpdateLifetime(DeltaTime);

	DamageTick.Update(DeltaTime);
	HitTick.Update(DeltaTime);

	if (!Expired)
	{
		BP_UpdateAmmunition(DeltaTime);
		UpdateAmmunition(DeltaTime);

		BroadcastOnUpdate(DeltaTime);
	}
}

//==============================================================================================================

void AAmmunitionBase::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	if (!Cast<AAmmunitionBase>(otherActor))
	{
		BP_AmmunitionBeginOverlap(otherActor);
		AmmunitionBeginOverlap(otherActor);

		BroadcastOnBeginOverlap(otherActor);
	}
}

//==============================================================================================================

void AAmmunitionBase::NotifyActorEndOverlap(AActor* otherActor)
{
	Super::NotifyActorEndOverlap(otherActor);

	if (!Cast<AAmmunitionBase>(otherActor))
	{
		//if (HitTick.HasTimer(otherActor))
		//{
			//HitTick.SetTimer(otherActor, 0.0f);
		//}

		BP_AmmunitionEndOverlap(otherActor);
		AmmunitionEndOverlap(otherActor);

		BroadcastOnEndOverlap(otherActor);
	}
}

//==============================================================================================================

void AAmmunitionBase::Initialise(float damage, float speed, float size, float lifetime)
{
	InitialiseDamage(damage);
	InitialiseSpeed(speed);
	InitialiseSize(size);
	InitialiseLifetime(lifetime);

	BP_InitialiseAmmunition();
	InitialiseAmmunition();

	UE_LOG(NiobiumAmmunition, Display, TEXT("Initialised [Damage: %.2f | Speed: %.2fm/s | Size: %.2fx | Lifetime: %.2fs]"), damage, speed, size, lifetime);
}

//==============================================================================================================

void AAmmunitionBase::Detonate()
{
	UE_LOG(NiobiumAmmunition, Display, TEXT("Detonation occurred"));

	BP_AmmunitionDetonation();
	AmmunitionDetonation();

	BroadcastOnDetonation();
}

//==============================================================================================================

void AAmmunitionBase::AddMovement(float deltaTime)
{
	if (!Expired)
	{
		LastLocation = GetLocation();

		FVector direction = GetActorForwardVector();

		BP_MoveAmmunition(direction, deltaTime);
		MoveAmmunition(direction, deltaTime);

		FHitResult hit;
		if (DetectHit(LastLocation, GetLocation(), hit))
		{
			RegisterHit(hit);
		}
	}
}

//==============================================================================================================

void AAmmunitionBase::SetRotation(FRotator rotation)
{
	if (!Expired)
	{
		LastRotation = GetRotation();

		BP_RotateAmmunition(rotation);
		RotateAmmunition(rotation);
	}
}

//==============================================================================================================

void AAmmunitionBase::RotateToFaceLocation(FVector location, float deltaTime, float lerpSpeed)
{
	FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(GetLocation(), location);

	if ((deltaTime > 0.0f) && (lerpSpeed > 0.0f))
	{
		FRotator lerp = UKismetMathLibrary::RInterpTo(GetRotation(), lookAt, deltaTime, lerpSpeed);
		SetRotation(lerp);
	}
	else
	{
		SetRotation(lookAt);
	}
}

//==============================================================================================================

void AAmmunitionBase::InitialiseDamage(float damage)
{
	Damage = damage;
	DamageMultiplier = 1.0f;
}

//==============================================================================================================

void AAmmunitionBase::DamageTarget(AActor* actor)
{
	if (!Expired)
	{
		if (ImplementsDamageInterface(actor))
		{
			INiobiumDamageInterface::Execute_Hit(actor, GetLocation(), GetDamage());
			UE_LOG(NiobiumAmmunition, Display, TEXT("Target [%s] Damaged"), *actor->GetName());
		}
	}
}

//==============================================================================================================

bool AAmmunitionBase::ImplementsDamageInterface(AActor* actor)
{
	return actor->GetClass()->ImplementsInterface(UNiobiumDamageInterface::StaticClass());
}

//==============================================================================================================

void AAmmunitionBase::InitialiseSpeed(float speed)
{
	Speed = (speed * UnitsToMeters);
	SpeedMultiplier = 1.0f;
}

//==============================================================================================================

void AAmmunitionBase::InitialiseSize(float size)
{
	SetActorScale3D(FVector(size));
}

//==============================================================================================================

void AAmmunitionBase::InitialiseLifetime(float lifetime)
{
	Lifetime = lifetime;
	RemainingLifetime = lifetime;
	TickLifetime = true;
}

//==============================================================================================================

void AAmmunitionBase::UpdateLifetime(float deltaTime)
{
	if (!Expired && TickLifetime)
	{
		if (RemainingLifetime <= 0.0f)
		{
			UE_LOG(NiobiumAmmunition, Display, TEXT("End of lifetime"));
			Detonate();
			Expire();
		}
		else
		{
			RemainingLifetime -= deltaTime;
		}
	}
}

//==============================================================================================================

void AAmmunitionBase::AddLifetime(float value)
{ 
	RemainingLifetime += value;

	if (RemainingLifetime > 0.0f)
	{
		Expired = false;
	}
}

//==============================================================================================================

bool AAmmunitionBase::DetectHit(FVector start, FVector end, FHitResult& hit)
{
	return GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility);
}

//==============================================================================================================

void AAmmunitionBase::RegisterHit(FHitResult hitResult)
{
	AActor* hitActor = hitResult.GetActor();

	// Damage actors which are not currently on cooldown for damage
	if (CanDamageTarget(hitActor, true))
	{
		StartDamageCooldown(hitActor);
		DamageTarget(hitActor);
	}

	// Trigger hit functions
	if (CanTriggerHit(hitActor, true))
	{
		StartHitCooldown(hitActor);

		UE_LOG(NiobiumAmmunition, Display, TEXT("Hit Registered on actor [%s]"), *hitActor->GetName());

		// Track hit locations
		AddHitLocation(hitResult.Location);

		// Inheritance functions
		BP_AmmunitionHit(hitResult);
		AmmunitionHit(hitResult);

		// Delegate functions
		BroadcastOnHit(hitResult);

		// Detonate
		if (GetDetonateOnHit())
		{
			Detonate();
		}

		// Expire
		if (GetExpireOnHit())
		{
			Expire();
		}
	}
	else
	{
		//UE_LOG(NiobiumAmmunition, Display, TEXT("Unable to hit actor [%s] (Hit cooldown > 0.0f)"), *hitActor->GetName());
	}
}

//==============================================================================================================

FVector AAmmunitionBase::GetHitLocation(int index)
{
	if (index >= 0 && index < HitLocations.Num())
	{
		return HitLocations[index];
	}

	return FVector::ZeroVector;
}

//==============================================================================================================

void AAmmunitionBase::AddHitLocation(FVector location)
{
	HitLocations.Add(location);
}

//==============================================================================================================

void AAmmunitionBase::Expire()
{
	Expired = true;

	BP_AmmunitionExpire();
	AmmunitionExpire();

	if (Expired)
	{
		BroadcastOnExpire();

		UE_LOG(NiobiumAmmunition, Display, TEXT("Expiration triggered"));
	}
}

//==============================================================================================================

void AAmmunitionBase::CancelExpire()
{
	Expired = false;

	UE_LOG(NiobiumAmmunition, Display, TEXT("Expiration cancelled"));
}

//==============================================================================================================

void AAmmunitionBase::DelayedExpire()
{
	Expired = true;

	UE_LOG(NiobiumAmmunition, Display, TEXT("Destroyed"));

	Destroy();
}

//==============================================================================================================

bool AAmmunitionBase::CanDamageTarget(AActor* actor, bool addTimer)
{
	if (addTimer && !DamageTick.HasTimer(actor))
	{
		DamageTick.AddTimer(actor);
	}

	if (DamageTick.GetTimerValue(actor) == 0.0f)
	{
		return true;
	}

	return false;
}

//==============================================================================================================

void AAmmunitionBase::StartDamageCooldown(AActor* actor)
{
	if (DamageTick.HasTimer(actor))
	{
		DamageTick.SetTimer(actor, DamageTickInterval);
	}
}

//==============================================================================================================

bool AAmmunitionBase::CanTriggerHit(AActor* actor, bool addTimer)
{
	if (addTimer && !HitTick.HasTimer(actor))
	{
		HitTick.AddTimer(actor);
	}

	if (HitTick.GetTimerValue(actor) == 0.0f)
	{
		return true;
	}

	return false;
}

//==============================================================================================================

void AAmmunitionBase::StartHitCooldown(AActor* actor)
{
	if (HitTick.HasTimer(actor))
	{
		HitTick.SetTimer(actor, HitTickInterval);
	}
}

//==============================================================================================================

void AAmmunitionBase::BroadcastOnUpdate(float deltaTime)
{
	OnCoreUpdate.Broadcast(this, deltaTime);
	OnModifierUpdate.Broadcast(this, deltaTime);
}

//==============================================================================================================

void AAmmunitionBase::BroadcastOnBeginOverlap(AActor* otherActor)
{
	OnCoreBeginOverlap.Broadcast(this, otherActor);
	OnModifierBeginOverlap.Broadcast(this, otherActor);
}

//==============================================================================================================

void AAmmunitionBase::BroadcastOnEndOverlap(AActor* otherActor)
{
	OnCoreEndOverlap.Broadcast(this, otherActor);
	OnModifierEndOverlap.Broadcast(this, otherActor);
}

//==============================================================================================================

void AAmmunitionBase::BroadcastOnHit(FHitResult hitResult)
{
	OnCoreHit.Broadcast(this, hitResult);
	OnModifierHit.Broadcast(this, hitResult);
}

//==============================================================================================================

void AAmmunitionBase::BroadcastOnDetonation()
{
	OnCoreDetonation.Broadcast(this);
	OnModifierDetonation.Broadcast(this);
}

//==============================================================================================================

void AAmmunitionBase::BroadcastOnExpire()
{
	OnCoreExpire.Broadcast(this);
	OnModifierExpire.Broadcast(this);
}

//==============================================================================================================

void AAmmunitionBase::InitialiseDetonateOnHit()
{
	DetonateOnHit.Name = "DetonateOnHit";
	DetonateOnHit = true;
}

//==============================================================================================================

void AAmmunitionBase::InitialiseExpireOnHit()
{
	ExpireOnHit.Name = "ExpireOnHit";
	ExpireOnHit = true;
}

//==============================================================================================================

void AAmmunitionBase::RequestDetonateOnHitChange(bool state)
{
	DetonateOnHit.RequestChange(state); 
	OnDetonateOnHitConflict.Broadcast(this, state); 	
	DetonateOnHit.Resolve();
}

//==============================================================================================================

void AAmmunitionBase::RequestExpireOnHitChange(bool state) 
{ 
	ExpireOnHit.RequestChange(state); 
	OnExpireOnHitConflict.Broadcast(this, state); 
	ExpireOnHit.Resolve(); 
}

//==============================================================================================================