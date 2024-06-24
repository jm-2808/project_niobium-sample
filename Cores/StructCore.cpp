//==============================================================================================================

#include "StructCore.h"

//==============================================================================================================

#include "NiobiumSystem/Ammunition/Projectile/ProjectileAmmunition.h"

#include "NiobiumSystem/Utility/Parameters/FiremodeEnumParameter.h"

#include "Math/UnrealMathUtility.h"

//==============================================================================================================

UStructCore::UStructCore()
	: UCoreBase()
{ }

//==============================================================================================================

UStructCore::~UStructCore()
{ }

//==============================================================================================================

void UStructCore::InitialiseCore()
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

void UStructCore::UpdateCore(float deltaTime)
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

void UStructCore::OnFireHeld()
{
	SetFireDelay();

	BurstDelay = GetShutterBurstDelayValue();
	SetBurstCounter();

	Fire();
}

//==============================================================================================================

void UStructCore::OnFireReleased()
{

}

//==============================================================================================================

void UStructCore::FireCore()
{
	int shards = GetShardsValue();
	for (int i = 0; i < shards; ++i)
	{
		SpawnAmmunitionAtFirePosition();
	}
}

//==============================================================================================================

void UStructCore::AmmunitionFired(AAmmunitionBase* ammunition)
{
	float damage = GetDamageParameterValue();
	float speed = GetSpeedParameterValue();
	float size = GetSizeMultiplierParameterValue();
	float lifetime = GetLifetimeParameterValue();

	ammunition->Initialise(damage, speed, size, lifetime);

	// Random deviation within the defined spread angle
	FRotator current = ammunition->GetRotation();
	float angle = (GetSpreadValue() / 2);

	FVector forward = FVector::ForwardVector;
	FRotator deviatePitch = FRotator(FMath::RandRange(0.0f, angle), 0, 0);
	FRotator deviateRoll = FRotator(0, 0, FMath::RandRange(0, 360));

	forward = deviatePitch.RotateVector(forward);
	forward = deviateRoll.RotateVector(forward);
	FRotator deviation = forward.Rotation();

	ammunition->SetRotation(FRotator(current.Pitch + deviation.Pitch, current.Yaw + deviation.Yaw, current.Roll + deviation.Roll));
}

//==============================================================================================================

void UStructCore::AmmunitionUpdate(AAmmunitionBase* ammunition, float deltaTime)
{
	ammunition->AddMovement(deltaTime);
}

//==============================================================================================================

void UStructCore::InitialiseParameters()
{
	// Basic
	InitialiseDamageParameter("Damage", 8.0f, 0.0f, 15.0f, 0.5f);
	InitialiseSpeedParameter("Speed (m/s)", 6.0f, 0.0f, 50.0f, 1.0f);
	InitialiseSizeMultiplierParameter("Size Multiplier", 1.0f, 0.5f, 3.0f, 0.1f);
	InitialiseLifetimeParameter("Lifetime (seconds)", 2.0f, 0.1f, 10.0f, 0.1f);

	// Struct
	AddParameter(ShardsParameter, NewObject<UIntegerParameter>());
	if (UIntegerParameter* param = GetParameter<UIntegerParameter>(ShardsParameter))
	{
		param->Initialise("Shrapnel Shards", 6, 1, 15, 1);
	}

	AddParameter(SpreadParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(SpreadParameter))
	{
		param->Initialise("Spread (degrees)", 30.0f, 0.0f, 180.0f, 5.0f);
	}

	// Shutter
	AddParameter(ShutterFireModeParameter, NewObject<UFireModeEnumParameter>());
	if (UFireModeEnumParameter * param = GetParameter<UFireModeEnumParameter>(ShutterFireModeParameter))
	{
		param->Initialise("Fire Mode", EFireMode::FIREMODE_AUTOMATIC);
		param->OnValueChanged.AddDynamic(this, &UStructCore::OnShutterFireModeChanged);
	}

	AddParameter(ShutterDelayParameter, NewObject<UFloatParameter>()); // Delay between shots when using FIREMODE_AUTOMATIC
	if (UFloatParameter * param = GetParameter<UFloatParameter>(ShutterDelayParameter))
	{
		param->Initialise("Fire rate (shots/second)", 2.0f, 0.1f, 5.0f, 0.1f);
	}

	AddParameter(ShutterBurstParameter, NewObject<UBooleanParameter>());
	if (UBooleanParameter * param = GetParameter<UBooleanParameter>(ShutterBurstParameter))
	{
		param->Initialise("Burst Enabled", false);
		param->OnValueChanged.AddDynamic(this, &UStructCore::OnShutterBurstValueChanged);
	}

	AddParameter(ShutterBurstCountParameter, NewObject<UIntegerParameter>());
	if (UIntegerParameter * param = GetParameter<UIntegerParameter>(ShutterBurstCountParameter))
	{
		param->Initialise("Burst Count", 3, 1, 10, 1);
	}

	AddParameter(ShutterBurstDelayParameter, NewObject<UFloatParameter>());
	if (UFloatParameter * param = GetParameter<UFloatParameter>(ShutterBurstDelayParameter))
	{
		param->Initialise("Burst Delay (seconds)", 0.1f, 0.1f, 5.0f, 0.1f);
	}

	// Run delegate functions
	OnShutterFireModeChanged(GetShutterFireModeValue());
	OnShutterBurstValueChanged(GetShutterBurstValue());
}

//==============================================================================================================

void UStructCore::SetFireDelay()
{
	FireDelay = (1 / GetShutterDelayValue());
}

//==============================================================================================================

void UStructCore::SetBurstCounter()
{
	BurstCounter = (GetShutterBurstValue() ? (GetShutterBurstCountValue() - 1) : 0);

}

//==============================================================================================================

int UStructCore::GetShardsValue()
{
	UIntegerParameter* param = GetParameter<UIntegerParameter>(ShardsParameter);
	return (param ? param->GetValue() : 1);
}

//==============================================================================================================

float UStructCore::GetSpreadValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(SpreadParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================

EFireMode UStructCore::GetShutterFireModeValue()
{
	UFireModeEnumParameter* param = GetParameter<UFireModeEnumParameter>(ShutterFireModeParameter);
	return (param ? param->GetValue() : EFireMode::FIREMODE_AUTOMATIC);
}

//==============================================================================================================

bool UStructCore::DoesShutterFireModeEqual(EFireMode value)
{
	UFireModeEnumParameter* param = GetParameter<UFireModeEnumParameter>(ShutterFireModeParameter);
	return (param ? param->DoesValueEqual(value) : false);
}

//==============================================================================================================

void UStructCore::OnShutterFireModeChanged(EFireMode value)
{
	switch (value)
	{
	case EFireMode::FIREMODE_SEMIAUTOMATIC:
	{
		if (UFloatParameter * param = GetParameter<UFloatParameter>(ShutterDelayParameter))
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

float UStructCore::GetShutterDelayValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(ShutterDelayParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================

bool UStructCore::GetShutterBurstValue()
{
	UBooleanParameter* param = GetParameter<UBooleanParameter>(ShutterBurstParameter);
	return (param ? param->GetValue() : false);
}

//==============================================================================================================

void UStructCore::OnShutterBurstValueChanged(bool value)
{
	if (UIntegerParameter * param = GetParameter<UIntegerParameter>(ShutterBurstCountParameter))
	{
		param->SetEditable(value);
	}

	if (UFloatParameter * param = GetParameter<UFloatParameter>(ShutterBurstDelayParameter))
	{
		param->SetEditable(value);
	}
}

//==============================================================================================================

int UStructCore::GetShutterBurstCountValue()
{
	UIntegerParameter* param = GetParameter<UIntegerParameter>(ShutterBurstCountParameter);
	return (param ? param->GetValue() : 0);
}

//==============================================================================================================

float UStructCore::GetShutterBurstDelayValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(ShutterBurstDelayParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================