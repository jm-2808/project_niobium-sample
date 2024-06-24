//==============================================================================================================

#include "CoreBase.h"

//==============================================================================================================

#include "Engine/World.h"

//==============================================================================================================

UCoreBase::UCoreBase()
	: UNiobiumBase()
{ }

//==============================================================================================================

UCoreBase::~UCoreBase()
{ }

//==============================================================================================================

void UCoreBase::Initialise(FCoreInformation information)
{
	CoreInformation = information;

	UNiobiumBase::InitialiseBase();

	FireHeld = false;

	BP_InitialiseCore();
	InitialiseCore();
}

//==============================================================================================================

void UCoreBase::Update(float deltaTime)
{
	BP_UpdateCore(deltaTime);
	UpdateCore(deltaTime);

	for (TPair<FString, UModifierBase*> mod : Modifiers)
	{
		mod.Value->Update(deltaTime);
	}
}

//==============================================================================================================

void UCoreBase::BindDelegates(AAmmunitionBase* ammunition)
{
	// Events
	ammunition->OnCoreUpdate.AddDynamic(this, &UCoreBase::OnAmmunitionUpdate);
	ammunition->OnCoreBeginOverlap.AddDynamic(this, &UCoreBase::OnAmmunitionBeginOverlap);
	ammunition->OnCoreEndOverlap.AddDynamic(this, &UCoreBase::OnAmmunitionEndOverlap);
	ammunition->OnCoreHit.AddDynamic(this, &UCoreBase::OnAmmunitionHit);
	ammunition->OnCoreDetonation.AddDynamic(this, &UCoreBase::OnAmmunitionDetonation);
	ammunition->OnCoreExpire.AddDynamic(this, &UModifierBase::OnAmmunitionExpire);

}

//==============================================================================================================

bool UCoreBase::CheckAmmunitionType(TSubclassOf<AAmmunitionBase> ammunitionClass)
{
	return CoreInformation.AmmunitionClass->IsChildOf(ammunitionClass);
}

//==============================================================================================================

void UCoreBase::SetFireHeld(bool state)
{
	FireHeld = state;
	if (FireHeld)
	{
		BP_OnFireHeld();
		OnFireHeld();
	}
	else
	{
		BP_OnFireReleased();
		OnFireReleased();
	}
}

//==============================================================================================================

void UCoreBase::Fire()
{
	BP_FireCore();
	FireCore();
}

//==============================================================================================================

AAmmunitionBase* UCoreBase::SpawnAmmunition(FVector location, FRotator rotation)
{
	if ((CoreInformation.AmmunitionClass != nullptr))
	{
		AAmmunitionBase* ammo = Cast<AAmmunitionBase>(GetWorld()->SpawnActor(CoreInformation.AmmunitionClass));
		if (ammo != nullptr)
		{
			// Set intial position
			ammo->SetActorLocationAndRotation(location, rotation);
			
			// Bind core delegates
			BindDelegates(ammo);
			OnAmmunitionFired(ammo);

			// Bind modifier delegates
			for (TPair<FString, UModifierBase*> mod : Modifiers)
			{
				mod.Value->BindDelegates(ammo);
				mod.Value->OnAmmunitionFired(ammo);
			}

			return ammo;
		}
	}
	return nullptr;
}

//==============================================================================================================

AAmmunitionBase* UCoreBase::SpawnAmmunitionAtFirePosition()
{
	if (FirePosition != nullptr)
	{
		FVector location = FirePosition->GetComponentLocation();
		FRotator rotation = FirePosition->GetComponentRotation();

		if ((CoreInformation.AmmunitionClass != nullptr))
		{
			AAmmunitionBase* ammo = Cast<AAmmunitionBase>(GetWorld()->SpawnActor(CoreInformation.AmmunitionClass));
			if (ammo != nullptr)
			{
				// Set intial position
				ammo->SetActorLocationAndRotation(location, rotation);

				// Set fire position
				ammo->SetFirePosition(FirePosition);

				// Bind core delegates
				BindDelegates(ammo);
				OnAmmunitionFired(ammo);

				// Bind modifier delegates
				for (TPair<FString, UModifierBase*> mod : Modifiers)
				{
					mod.Value->BindDelegates(ammo);
					mod.Value->OnAmmunitionFired(ammo);
				}

				return ammo;
			}
		}
	}
	return nullptr;
}

//==============================================================================================================

void UCoreBase::AddModifier(FString name, UModifierBase* modifier)
{
	if (!Modifiers.Contains(name))
	{
		Modifiers.Add(name, modifier);
		Modifiers[name]->SetInstalled(true);

		UE_LOG(NiobiumCore, Display, TEXT("%s: Modifier [%s] added"), *GetName().ToString(), *name);
	}
	else
	{
		UE_LOG(NiobiumCore, Display, TEXT("%s: Modifier [%s] is already equipped, remove before trying to add another instance"), *GetName().ToString(), *name);
	}
}

//==============================================================================================================

void UCoreBase::RemoveModifier(FString name)
{
	if (Modifiers.Contains(name))
	{
		Modifiers[name]->SetInstalled(false);
		Modifiers[name]->ResetAllParameters();
		Modifiers.Remove(name);

		UE_LOG(NiobiumCore, Display, TEXT("%s: Modifier [%s] removed"), *GetName().ToString(), *name);
	}
	else
	{
		UE_LOG(NiobiumCore, Display, TEXT("%s: Modifier [%s] not found on core, unable to remove"), *GetName().ToString(), *name);
	}
}

//==============================================================================================================

void UCoreBase::RemoveAllModifiers()
{
	UE_LOG(NiobiumCore, Display, TEXT("%s: Removing all modifiers"), *GetName().ToString());

	TArray<FString> names = GetModifierNames();
	for (int i = 0; i < names.Num(); ++i)
	{
		RemoveModifier(names[i]);
	}
}

//==============================================================================================================

UModifierBase* UCoreBase::GetModifier(FString name)
{
	if (Modifiers.Contains(name))
	{
		return Modifiers[name];
	}
	else
	{
		return nullptr;
	}
}

//==============================================================================================================

TArray<FString> UCoreBase::GetModifierNames()
{
	TArray<FString> names;
	for (TPair<FString, UModifierBase*> mod : Modifiers)
	{
		names.Add(mod.Key);
	}

	return names;
}

//==============================================================================================================

void UCoreBase::InitialiseDamageParameter(FString displayName, float defaultValue, float minimumValue, float maxiumumValue, float increment)
{
	AddParameter(DamageParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(DamageParameter))
	{
		param->Initialise(displayName, defaultValue, minimumValue, maxiumumValue, increment);
	}
}

//==============================================================================================================

float UCoreBase::GetDamageParameterValue()
{ 
	UFloatParameter* param = GetParameter<UFloatParameter>(DamageParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================

void UCoreBase::InitialiseSpeedParameter(FString displayName, float defaultValue, float minimumValue, float maxiumumValue, float increment)
{
	AddParameter(SpeedParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(SpeedParameter))
	{
		param->Initialise(displayName, defaultValue, minimumValue, maxiumumValue, increment);
	}
}

//==============================================================================================================

float UCoreBase::GetSpeedParameterValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(SpeedParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================

void UCoreBase::InitialiseSizeMultiplierParameter(FString displayName, float defaultValue, float minimumValue, float maxiumumValue, float increment)
{
	AddParameter(SizeMultiplierParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(SizeMultiplierParameter))
	{
		param->Initialise(displayName, defaultValue, minimumValue, maxiumumValue, increment);
	}
}

//==============================================================================================================

float UCoreBase::GetSizeMultiplierParameterValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(SizeMultiplierParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================

void UCoreBase::InitialiseLifetimeParameter(FString displayName, float defaultValue, float minimumValue, float maxiumumValue, float increment)
{
	AddParameter(LifetimeParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(LifetimeParameter))
	{
		param->Initialise(displayName, defaultValue, minimumValue, maxiumumValue, increment);
	}
}

//==============================================================================================================

float UCoreBase::GetLifetimeParameterValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(LifetimeParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================