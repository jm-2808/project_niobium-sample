//==============================================================================================================

#pragma once

//==============================================================================================================

#include "Project_Niobium.h"

//==============================================================================================================

#include "Delegate.h"
#include "GameFramework/Actor.h"
#include "AmmunitionBase.generated.h"

//==============================================================================================================

// Ammunition Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoreUpdateDelegate, class AAmmunitionBase*, Ammunition, float, DeltaTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifierUpdateDelegate, class AAmmunitionBase*, Ammunition, float, DeltaTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoreBeginOverlapDelegate, class AAmmunitionBase*, Ammunition, class AActor*, OverlappedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoreEndOverlapDelegate, class AAmmunitionBase*, Ammunition, class AActor*, OverlappedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifierBeginOverlapDelegate, class AAmmunitionBase*, Ammunition, class AActor*, OverlappedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifierEndOverlapDelegate, class AAmmunitionBase*, Ammunition, class AActor*, OverlappedActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoreHitDelegate, class AAmmunitionBase*, Ammunition, FHitResult, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModifierHitDelegate, class AAmmunitionBase*, Ammunition, FHitResult, HitResult);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoreDetonateDelgate, class AAmmunitionBase*, Ammunition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierDetonateDelgate, class AAmmunitionBase*, Ammunition);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoreExpireDelgate, class AAmmunitionBase*, Ammunition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierExpireDelgate, class AAmmunitionBase*, Ammunition);

// Conflict Resolution
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDetonateOnHitConflictDelegate, class AAmmunitionBase*, Ammunition, bool, RequestedValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExpireOnHitConflictDelegate, class AAmmunitionBase*, Ammunition, bool, RequestedValue);

//==============================================================================================================

UCLASS(Abstract)
class PROJECT_NIOBIUM_API AAmmunitionBase : public AActor
{
	GENERATED_BODY()

public:	
	AAmmunitionBase();
	~AAmmunitionBase();

	//=== AActor Interface ===================================
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;
	virtual void NotifyActorEndOverlap(AActor* otherActor) override;

	//=== Function ===========================================
	/// Initialise
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void Initialise(float damage, float speed, float size, float lifetime);

protected:
	virtual void InitialiseAmmunition() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Initialise Ammunition"))
	void BP_InitialiseAmmunition();
	void BP_InitialiseAmmunition_Implementation() {}

	/// Update
protected:
	virtual void UpdateAmmunition(float deltaTime) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Update Ammunition"))
	void BP_UpdateAmmunition(float deltaTime);
	void BP_UpdateAmmunition_Implementation(float deltaTime) {}

	/// Begin Overlap
protected:
	virtual void AmmunitionBeginOverlap(AActor* otherActor) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Ammunition Begin Overlap"))
	void BP_AmmunitionBeginOverlap(AActor* otherActor);
	void BP_AmmunitionBeginOverlap_Implementation(AActor* otherActor) {}

	/// End Overlap
protected:
	virtual void AmmunitionEndOverlap(AActor* otherActor) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Ammunition End Overlap"))
	void BP_AmmunitionEndOverlap(AActor* otherActor);
	void BP_AmmunitionEndOverlap_Implementation(AActor* otherActor) {}

	/// Hit
protected:
	virtual void AmmunitionHit(FHitResult hitResult) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Ammunition Hit"))
	void BP_AmmunitionHit(FHitResult hitResult);
	void BP_AmmunitionHit_Implementation(FHitResult hitResult) {}

	/// Detonate
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void Detonate();

protected:
	virtual void AmmunitionDetonation() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Ammunition Detonation"))
	void BP_AmmunitionDetonation();
	void BP_AmmunitionDetonation_Implementation() {}

	/// Expire
protected:
	virtual void AmmunitionExpire() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Ammunition Expire"))
	void BP_AmmunitionExpire();
	void BP_AmmunitionExpire_Implementation() {}

	//=== Origin =============================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetOrigin() { return Origin; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	USceneComponent* GetFirePosition() { return FirePosition; }

	void SetFirePosition(USceneComponent* firePosition) { FirePosition = firePosition; }

private:
	FVector Origin;
	USceneComponent* FirePosition;

	//=== Movement ===========================================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void AddMovement(float deltaTime);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void SetRotation(FRotator rotation);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void RotateToFaceLocation(FVector location, float deltaTime = 0.0f, float lerpSpeed = 0.0f);

protected:
	virtual void MoveAmmunition(FVector direction, float deltaTime) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Move Ammunition"))
	void BP_MoveAmmunition(FVector direction, float deltaTime);
	void BP_MoveAmmunition_Implementation(FVector direction, float deltaTime) {}

	virtual void RotateAmmunition(FRotator rotation) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Rotate Ammunition"))
	void BP_RotateAmmunition(FRotator rotation);
	void BP_RotateAmmunition_Implementation(FRotator rotation) {}

	//=== Location / Rotation ================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetLocation() { return GetAmmunitionLocation(); }
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FRotator GetRotation() { return GetAmmunitionRotation(); }

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetLastLocation() { return LastLocation; }
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FRotator GetLastRotation() { return LastRotation; }

protected:
	virtual FVector GetAmmunitionLocation() { return GetActorLocation(); }
	virtual FRotator GetAmmunitionRotation() { return GetActorRotation(); }

private:
	FVector LastLocation;
	FRotator LastRotation;

	//=== Damage =============================================
protected:
	void InitialiseDamage(float damage);

public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetDamage() { return Damage * DamageMultiplier; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetBaseDamage() { return Damage; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void UpdateDamageMultiplier(float value) { DamageMultiplier += value; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void DamageTarget(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void SetDamageTickInterval(float interval) { DamageTickInterval = interval; }

protected:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	bool ImplementsDamageInterface(AActor* actor);

private:
	float Damage;
	float DamageMultiplier;

	float DamageTickInterval;

	//=== Speed ==============================================
protected:
	void InitialiseSpeed(float speed);

public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetSpeed() { return Speed * SpeedMultiplier; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetBaseSpeed() { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void UpdateSpeedMultiplier(float value) { SpeedMultiplier += value; }

private:
	float Speed;
	float SpeedMultiplier;

	const float UnitsToMeters = 100.0f;

	//=== Size ===============================================
protected:
	void InitialiseSize(float size);

	//=== Lifetime ===========================================
protected:
	void InitialiseLifetime(float lifetime);

private:
	virtual void UpdateLifetime(float deltaTime);

public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetLifetime() { return Lifetime; }

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	float GetRemainingLifetime() { return RemainingLifetime; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void AddLifetime(float value);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void SetTickLifetime(bool state) { TickLifetime = state; }

private:
	float Lifetime;
	float RemainingLifetime;
	bool TickLifetime;

	//=== Hit ================================================
public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	FVector GetHitLocation(int index);
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	int GetNumHitLocations() { return HitLocations.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void SetHitTickInterval(float interval) { HitTickInterval = interval; }

protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	bool DetectHit(FVector start, FVector end, FHitResult& hit);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void RegisterHit(FHitResult hitResult);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void AddHitLocation(FVector location);

private:
	TArray<FVector> HitLocations;

	float HitTickInterval;

	//=== Expiry =============================================
public:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void Expire();

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void CancelExpire();

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void ForceExpire() { DelayedExpire(); }

	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	bool HasExpired() { return Expired; }

private:
	void DelayedExpire();

private:
	bool Expired;

	//=== Timers ========================================
protected:
	struct AmmunitionEventTimer
	{
		AActor* Actor;
		float Value;

		AmmunitionEventTimer()
		{
			Actor = nullptr;
			Value = 0.0f;
		}
	};

	struct AmmunitionEventTimers
	{
		TArray<AmmunitionEventTimer> Timers;

		AmmunitionEventTimers(){ }

		void Update(float deltaTime)
		{
			for (int i = 0; i < Timers.Num(); ++i)
			{
				if (Timers[i].Value > 0.0f)
				{
					Timers[i].Value -= deltaTime;
					if (Timers[i].Value < 0.0f)
					{
						Timers[i].Value = 0.0f;
					}
				}
			}
		}

		void AddTimer(AActor* actor, float value = 0.0f)
		{
			if (!HasTimer(actor))
			{
				AmmunitionEventTimer t;
				t.Actor = actor;
				t.Value = value;
				Timers.Add(t);
			}
		}

		void RemoveTimer(AActor* actor)
		{
			if (HasTimer(actor))
			{
				Timers.RemoveAt(GetTimerIndex(actor));
			}
		}

		float GetTimerValue(AActor* actor)
		{
			return (HasTimer(actor) ? Timers[GetTimerIndex(actor)].Value : 0.0f);
		}

		bool HasTimer(AActor* actor)
		{
			return (GetTimerIndex(actor) > -1);
		}

		int GetTimerIndex(AActor* actor)
		{
			for (int i = 0; i < Timers.Num(); ++i)
			{
				if (Timers[i].Actor == actor)
				{
					return i;
				}
			}
			return -1;
		}

		void SetTimer(AActor* actor, float value)
		{
			if (HasTimer(actor))
			{
				Timers[GetTimerIndex(actor)].Value = value;
			}
		}
	};

	/// Damage Timers
protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	bool CanDamageTarget(AActor* actor, bool addTimer = false);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void StartDamageCooldown(AActor* actor);

private:
	AmmunitionEventTimers DamageTick;

	/// Hit Timers
protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	bool CanTriggerHit(AActor* actor, bool addTimer = false);

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void StartHitCooldown(AActor* actor);

private:
	AmmunitionEventTimers HitTick;

	//=== Inheritance Flags ==================================
protected:
	// Place for any flags to allow children to alter base functionality

	//=== Event Delegates ====================================
	/// OnUpdate
protected:
	void BroadcastOnUpdate(float deltaTime);

public:
	UPROPERTY(BlueprintAssignable)
	FOnCoreUpdateDelegate OnCoreUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnModifierUpdateDelegate OnModifierUpdate;

	/// On Overlap
protected:
	void BroadcastOnBeginOverlap(AActor* otherActor);

	void BroadcastOnEndOverlap(AActor* otherActor);

public:
	UPROPERTY(BlueprintAssignable)
	FOnCoreBeginOverlapDelegate OnCoreBeginOverlap;
	UPROPERTY(BlueprintAssignable)
	FOnCoreEndOverlapDelegate OnCoreEndOverlap;

	UPROPERTY(BlueprintAssignable)
	FOnModifierBeginOverlapDelegate OnModifierBeginOverlap;
	UPROPERTY(BlueprintAssignable)
	FOnModifierEndOverlapDelegate OnModifierEndOverlap;

	/// On Hit
protected:
	void BroadcastOnHit(FHitResult hitResult);

public:
	UPROPERTY(BlueprintAssignable)
	FOnCoreHitDelegate OnCoreHit;

	UPROPERTY(BlueprintAssignable)
	FOnModifierHitDelegate OnModifierHit;

	/// On Detonation
protected:
	void BroadcastOnDetonation();

public:
	UPROPERTY(BlueprintAssignable)
	FOnCoreDetonateDelgate OnCoreDetonation;

	UPROPERTY(BlueprintAssignable)
	FOnModifierDetonateDelgate OnModifierDetonation;

	/// On Expire
protected:
	void BroadcastOnExpire();

public:
	UPROPERTY(BlueprintAssignable)
	FOnCoreExpireDelgate OnCoreExpire;

	UPROPERTY(BlueprintAssignable)
	FOnModifierExpireDelgate OnModifierExpire;

	//=== Conflict resolution ================================
protected:
	template<typename T>
	struct ConflictVariable
	{
		FString Name;

		T Value;
		T RequestedValue;

		bool HasRequestedChange;
		int VetoCount;

		bool Locked;

		ConflictVariable()
		{
			HasRequestedChange = false;
			VetoCount = 0;

			Locked = false;
		}

		T& operator=(T rhs)
		{
			Value = rhs;
			RequestedValue = rhs;

			return Value;
		}

		void RequestChange(T value)
		{
			if (!Locked)
			{
				RequestedValue = value;

				HasRequestedChange = true;
				VetoCount = 0;

				UE_LOG(NiobiumAmmunition, Display, TEXT("%s requested change"), *Name);
			}
		}

		void VetoChange(FString reason)
		{
			VetoCount++;
			UE_LOG(NiobiumAmmunition, Display, TEXT("%s Veto: %s"), *Name, *reason);
		}

		void Resolve()
		{
			if (HasRequestedChange)
			{
				if (VetoCount == 0)
				{
					Value = RequestedValue;
					UE_LOG(NiobiumAmmunition, Display, TEXT("%s change resolved"), *Name);
				}
				else
				{
					RequestedValue = Value;
					UE_LOG(NiobiumAmmunition, Display, TEXT("%s change veto'd"), *Name);
				}
				HasRequestedChange = false;
				VetoCount = 0;
			}
		}

		void Lock(bool state)
		{
			Locked = state;
		}
	};

	/// Detonate On Hit
private:
	ConflictVariable<bool> DetonateOnHit;

private:
	void InitialiseDetonateOnHit();

public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	bool GetDetonateOnHit() { return DetonateOnHit.Value; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void RequestDetonateOnHitChange(bool state);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void VetoDetonateOnHitChange(FString reason) { DetonateOnHit.VetoChange(reason); }

protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void SetDetonateOnHit(bool state) { DetonateOnHit.Value = state; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void LockDetonateOnHit(bool state) { DetonateOnHit.Lock(state); }

public:
	UPROPERTY(BlueprintAssignable)
	FDetonateOnHitConflictDelegate OnDetonateOnHitConflict;

	/// Expire On Hit
private:
	ConflictVariable<bool> ExpireOnHit;

private:
	void InitialiseExpireOnHit();

public:
	UFUNCTION(BlueprintPure, Category = "Niobium|Ammunition")
	bool GetExpireOnHit() { return ExpireOnHit.Value; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void RequestExpireOnHitChange(bool state);
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void VetoExpireOnHitChange(FString reason) { ExpireOnHit.VetoChange(reason); }

protected:
	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void SetExpireOnHit(bool state) { ExpireOnHit.Value = state; }

	UFUNCTION(BlueprintCallable, Category = "Niobium|Ammunition")
	void LockExpireOnHit(bool state) { ExpireOnHit.Lock(state); }

public:
	UPROPERTY(BlueprintAssignable)
	FExpireOnHitConflictDelegate OnExpireOnHitConflict;

};

//==============================================================================================================