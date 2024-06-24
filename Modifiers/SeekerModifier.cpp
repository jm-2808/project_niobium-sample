//==============================================================================================================

#include "SeekerModifier.h"

//==============================================================================================================

#include "Kismet/KismetMathLibrary.h"

//==============================================================================================================

USeekerModifier::USeekerModifier()
	: UModifierBase()
{ }

//==============================================================================================================

USeekerModifier::~USeekerModifier()
{ }

//==============================================================================================================

void USeekerModifier::InitialiseModifier()
{
	Tracer = nullptr;

	TracerLifetime = 0.0f;

	SeekerCounter = 0;
}

//==============================================================================================================

void USeekerModifier::UpdateModifier(float deltaTime)
{
	if (Tracer != nullptr)
	{
		// If tracer is dead, stop all seekers
		if (Tracer->IsPendingKillPending())
		{
			UE_LOG(NiobiumModifier, Display, TEXT("%s: Tracer target destroyed"), *GetName().ToString());
			ClearTracer();
		}
	}

	if (Tracer != nullptr) // Second check required in case previous if statement clears the tracer
	{
		// Decay lifetime
		TracerLifetime -= deltaTime;
		if (TracerLifetime <= 0.0f)
		{
			UE_LOG(NiobiumModifier, Display, TEXT("%s: Tracer decayed"), *GetName().ToString());
			ClearTracer();
		}
	}
}

//==============================================================================================================

void USeekerModifier::AmmunitionFired(AAmmunitionBase* ammunition)
{
	if (SeekerCounter > 0)
	{
		AddSeeker(ammunition);
		UE_LOG(NiobiumModifier, Display, TEXT("%s: Spawned shot tagged (Seekers left: %i)"), *GetName().ToString(), SeekerCounter);
	}
}

//==============================================================================================================

void USeekerModifier::AmmunitionUpdate(AAmmunitionBase* ammunition, float deltaTime)
{
	if (Tracer != nullptr)
	{
		// Make any in flight shots seekers when tracer is set
		if (SeekerCounter > 0 && !IsSeeker(ammunition) && !ammunition->HasExpired())
		{
			AddSeeker(ammunition);
			UE_LOG(NiobiumModifier, Display, TEXT("%s: Active shot tagged (Seekers left: %i)"), *GetName().ToString(), SeekerCounter);
		}

		// If this shot is a seeker, seek and destroy
		if (IsSeeker(ammunition))
		{
			if (!Seekers[ammunition].Recalibrating)
			{
				ammunition->RotateToFaceLocation(Tracer->GetActorLocation(), deltaTime, GetSeekerArcSpeedValue());
			}
			else
			{
				Seekers[ammunition].Recalibrate(deltaTime);
			}
		}
	}
}

//==============================================================================================================

void USeekerModifier::AmmunitionHit(AAmmunitionBase* ammunition, FHitResult hitResult)
{
	if (Tracer == nullptr && !IsSeeker(ammunition) && IsEnemy(hitResult.GetActor()))
	{
		SetTracer(hitResult.GetActor());
		ammunition->ForceExpire();
	}

	if (IsSeeker(ammunition))
	{
		Seekers[ammunition].Recalibrating = true;
		Seekers[ammunition].RecalibrationTimer = GetSeekerRecalibrationValue();
	}
}

//==============================================================================================================

void USeekerModifier::AmmunitionExpire(AAmmunitionBase* ammunition)
{
	if (IsSeeker(ammunition))
	{
		RemoveSeeker(ammunition);
		if (Seekers.Num() == 0 && SeekerCounter <= 0 && Tracer != nullptr)
		{
			ClearTracer();
		}
	}
}

//==============================================================================================================

void USeekerModifier::InitialiseParameters()
{
	UModifierBase::InitialiseParameters();

	AddParameter(TracerLifetimeParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(TracerLifetimeParameter))
	{
		param->Initialise("Tracer lifetime (seconds)", 10.0f, 1.0f, 25.0f, 0.5f);
	}

	AddParameter(SeekerCountParameter, NewObject<UIntegerParameter>());
	if (UIntegerParameter* param = GetParameter<UIntegerParameter>(SeekerCountParameter))
	{
		param->Initialise("Seekers per Tracer", 10, 1, 15, 1);
	}

	AddParameter(SeekerArcSpeedParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(SeekerArcSpeedParameter))
	{
		param->Initialise("Seeker Arc Speed", 8.0f, 6.0f, 10.0f, 0.25f);
	}

	AddParameter(SeekerRecalibrationParameter, NewObject<UFloatParameter>());
	if (UFloatParameter* param = GetParameter<UFloatParameter>(SeekerRecalibrationParameter))
	{
		param->Initialise("Recalibration duration (seconds)", 0.2f, 0.1f, 1.0f, 0.1f);
	}
}

//==============================================================================================================

void USeekerModifier::SetTracer(AActor* target)
{
	Tracer = target;
	SeekerCounter = GetSeekerCountValue();
	TracerLifetime = GetTracerLifetimeValue();

	UE_LOG(NiobiumModifier, Display, TEXT("%s: Tracer set (Seekers left: %i)"), *GetName().ToString(), SeekerCounter);
}

//==============================================================================================================

void USeekerModifier::ClearTracer()
{
	Tracer = nullptr;
	SeekerCounter = 0;
	TracerLifetime = 0.0f;
	Seekers.Empty();

	UE_LOG(NiobiumModifier, Display, TEXT("%s: Tracer cleared"), *GetName().ToString());
}

//==============================================================================================================

void USeekerModifier::AddSeeker(AAmmunitionBase* ammunition)
{
	if (SeekerCounter > 0)
	{
		Seekers.Add(ammunition, SeekerInfo());
		SeekerCounter--;
	}
}

//==============================================================================================================

void USeekerModifier::RemoveSeeker(AAmmunitionBase* ammunition)
{
	Seekers.Remove(ammunition);
}

//==============================================================================================================

bool USeekerModifier::IsSeeker(AAmmunitionBase* ammunition)
{
	return Seekers.Contains(ammunition);
}

//==============================================================================================================

float USeekerModifier::GetTracerLifetimeValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(TracerLifetimeParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================

int USeekerModifier::GetSeekerCountValue()
{
	UIntegerParameter* param = GetParameter<UIntegerParameter>(SeekerCountParameter);
	return (param ? param->GetValue() : 0);
}

//==============================================================================================================

float USeekerModifier::GetSeekerArcSpeedValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(SeekerArcSpeedParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================

float USeekerModifier::GetSeekerRecalibrationValue()
{
	UFloatParameter* param = GetParameter<UFloatParameter>(SeekerRecalibrationParameter);
	return (param ? param->GetValue() : 0.0f);
}

//==============================================================================================================