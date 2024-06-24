//==============================================================================================================

#include "ScalarCore.h"

//==============================================================================================================

#include "NiobiumSystem/Ammunition/Projectile/ProjectileAmmunition.h"

#include "NiobiumSystem/Utility/Parameters/FiremodeEnumParameter.h"

//==============================================================================================================

UScalarCore::UScalarCore()
	: UCoreBase()
{ }

//==============================================================================================================

UScalarCore::~UScalarCore()
{ }

//==============================================================================================================

void UScalarCore::InitialiseCore()
{
	if (!CheckAmmunitionType(AProjectileAmmunition::StaticClass()))
	{
		UE_LOG(NiobiumCore, Display, TEXT("%s: Projectile Ammunition class expected, continuing may result in unexpected behaviour"), *GetName().ToString());
	}

	FireDelay = 0.0f;

	BurstCounter = 0;
	BurstDelay = 0.0f;
}

//==============================================================================================================

void UScalarCore::UpdateCore(float deltaTime)
{
	if (GetShutterBurstValue())
	{
		if (BurstCounter > 0)
		{
			BurstDelay -= deltaTime;
			if (BurstDelay <= 0.0f)
			{
				BurstDelay = GetShutterBurstDelayValue();
				BurstCounter--;
				Fire();
			}
		}
	}

	if (IsFiring() && DoesShutterFireModeEqual(EFireMode::FIREMODE_AUTOMATIC))
	{
		if (BurstCounter == 0)
		{
			FireDelay -= deltaTime;
			if (FireDelay <= 0.0f)
			{
				SetBurstCounter();

				SetFireDelay();

				Fire();
			}
		}
	}
}

//==============================================================================================================

void UScalarCore::OnFireHeld()
{
	SetFireDelay();

	BurstDelay = GetShutterBurstDelayValue();
	SetBurstCounter();

	Fire();
}

//==============================================================================================================

void UScalarCore::OnFireReleased()
{

}

//==============================================================================================================

void UScalarCore::FireCore()
{
	SpawnAmmunitionAtFirePosition();
}

//==============================================================================================================

void UScalarCore::AmmunitionFired(AAmmunitionBase* ammunition)
{
	float damage = GetDamageParameterValue();
	float speed = GetSpeedParameterValue();
	float size = GetSizeMultiplierParameterValue();
	float lifetime = GetLifetimeParameterValue();

	ammunition->Initialise(damage, speed, size, lifetime);
}

//==============================================================================================================

void UScalarCore::AmmunitionUpdate(AAmmunitionBase* ammunition, float deltaTime)
{
	ammunition->AddMovement(deltaTime);
}

//==============================================================================================================

void UScalarCore::InitialiseParameters()
{
	// Basic
	InitialiseDamageParameter("Damage", 50.0f, 0.0f, 100.0f, 1.0f);
	InitialiseSpeedParameter("Speed (m/s)", 10.0f, 0.0f, 50.0f, 1.0f);
	InitialiseSizeMultiplierParameter("Size Multiplier", 1.0f, 0.5f, 3.0f, 0.1f);
	InitialiseLifetimeParameter("Lifetime (seconds)", 5.0f, 0.1f, 10.0f, 0.1f);

	// Shutter
	AddParameter(ShutterFireModeParameter, NewObject<UFireModeEnumParameter>());
	if (UFireModeEnumParameter* param = GetParameter<UFireModeEnumParameter>(ShutterFireModeParameter))
	{
		param->Initialise("Fire Mode", EFireMode::FIREMODE_AUTOMATIC);
		param->OnValueChanged.AddDynamic(this, &UScalarCore::OnShutterFireModeChanged);
	}

	AddParameter(ShutterDelayParameter, NewObject<UFloatParameter>()); // Delay between shots when using FIREMODE_AUTOMATIC
	if (UFloatParameter* param = GetParameter<UFloatParameter>(ShutterDelayParameter))
	{
		param->Initialise("Fire rate (shots/second)", 2.0f, 0.1f, 5.0f, 0.1f);
	}

	AddParameter(ShutterBurstParameter, NewObject<UBooleanParameter>());
	if (UBooleanParameter* param = GetParameter<UBooleanParameter>(ShutterBurstParameter))
	{
		param->Initialise("Burst Enabled", false);
		param->OnValueChanged.AddDynamic(this, &UScalarCore::OnShutterBurstValueChanged);
	}

	AddParameter(ShutterBurstCountParameter, NewObject<UIntegerParameter>());
	if (UIntegerParameter* param = GetParameter<UIntegerParameter>(ShutterBurstCountParameter))
	{
		param->Initialise("Burst Count", 3, 1, 10, 1);
	}

	AddParameter(ShutterBurstDelayParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(ShutterBurstDelayParameter))
	{
		param->Initialise("Burst Delay (seconds)", 0.1f, 0.1f, 5.0f, 0.1f);
	}

	// Run delegate functions
	OnShutterFireModeChanged(GetShutterFireModeValue());
	OnShutterBurstValueChanged(GetShutterBurstValue());
}

//==============================================================================================================

void UScalarCore::SetFireDelay()
{
	FireDelay = (1 / GetShutterDelayValue());
}

//==============================================================================================================

void UScalarCore::SetBurstCounter()
{
	BurstCounter = (GetShutterBurstValue() ? (GetShutterBurstCountValue() - 1) : 0);
}

//==============================================================================================================

EFireMode UScalarCore::GetShutterFireModeValue()
{
	UFireModeEnumParameter* param = GetParameter<UFireModeEnumParameter>(ShutterFireModeParameter);
	return (param ? param->GetValue() : EFireMode::FIREMODE_AUTOMATIC);
}

//==============================================================================================================

bool UScalarCore::DoesShutterFireModeEqual(EFireMode value)
{
	UFireModeEnumParameter* param = GetParameter<UFireModeEnumParameter>(ShutterFireModeParameter);
	return (param ? param->DoesValueEqual(value) : false);
}

//==============================================================================================================

void UScalarCore::OnShutterFireModeChanged(EFireMode value)
{
	switch (value)
	{
	case EFireMode::FIREMODE_SEMIAUTOMATIC:
	{
		if (UFloatParameter* param = GetParameter<UFloatParameter>(ShutterDelayParameter))
		{
			param->SetEditable(false);
		}
		break;
	}
	case EFireMode::FIREMODE_AUTOMATIC:
	{
		if (UFloatParameter * param = GetParameter<UFloatParameter>(ShutterDelayParameter))
		{
			param->SetEditable(true);
		}
		break;
	}
	default:
		break;
	}
}

//==============================================================================================================

float UScalarCore::GetShutterDelayValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(ShutterDelayParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================

bool UScalarCore::GetShutterBurstValue()
{
	UBooleanParameter* param = GetParameter<UBooleanParameter>(ShutterBurstParameter);
	return (param ? param->GetValue() : false);
}

//==============================================================================================================

void UScalarCore::OnShutterBurstValueChanged(bool value)
{
	if(UIntegerParameter* param = GetParameter<UIntegerParameter>(ShutterBurstCountParameter))
	{
		param->SetEditable(value);
	}

	if (UFloatParameter * param = GetParameter<UFloatParameter>(ShutterBurstDelayParameter))
	{
		param->SetEditable(value);
	}
}

//==============================================================================================================

int UScalarCore::GetShutterBurstCountValue()
{
	UIntegerParameter* param = GetParameter<UIntegerParameter>(ShutterBurstCountParameter);
	return (param ? param->GetValue() : 0);
}

//==============================================================================================================

float UScalarCore::GetShutterBurstDelayValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(ShutterBurstDelayParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================