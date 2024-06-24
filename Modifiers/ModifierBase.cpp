//==============================================================================================================

#include "ModifierBase.h"

//==============================================================================================================

UModifierBase::UModifierBase()
	: UNiobiumBase()
{ }

//==============================================================================================================

UModifierBase::~UModifierBase()
{ }

//==============================================================================================================

void UModifierBase::Initialise(FModifierInformation information)
{
	ModifierInformation = information;

	UNiobiumBase::InitialiseBase();

	BP_InitialiseModifier();
	InitialiseModifier();
}

//==============================================================================================================

void UModifierBase::Update(float deltaTime)
{
	BP_UpdateModifier(deltaTime);
	UpdateModifier(deltaTime);
}

//==============================================================================================================

void UModifierBase::BindDelegates(AAmmunitionBase* ammunition)
{
	// Events
	ammunition->OnModifierUpdate.AddDynamic(this, &UModifierBase::OnAmmunitionUpdate);
	ammunition->OnModifierBeginOverlap.AddDynamic(this, &UModifierBase::OnAmmunitionBeginOverlap);
	ammunition->OnModifierEndOverlap.AddDynamic(this, &UModifierBase::OnAmmunitionEndOverlap);
	ammunition->OnModifierHit.AddDynamic(this, &UModifierBase::OnAmmunitionHit);
	ammunition->OnModifierDetonation.AddDynamic(this, &UModifierBase::OnAmmunitionDetonation);
	ammunition->OnModifierExpire.AddDynamic(this, &UModifierBase::OnAmmunitionExpire);

	// Conflicts
	ammunition->OnDetonateOnHitConflict.AddDynamic(this, &UModifierBase::OnDetonateOnHitConflict);
	ammunition->OnExpireOnHitConflict.AddDynamic(this, &UModifierBase::OnExpireOnHitConflict);
}

//==============================================================================================================