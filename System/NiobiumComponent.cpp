//==============================================================================================================

#include "NiobiumComponent.h"

//==============================================================================================================

#include "ConstructorHelpers.h"

#include "JsonObjectConverter.h"

//==============================================================================================================

UNiobiumComponent::UNiobiumComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UNiobiumDataAsset> sysInfo(TEXT("/Game/Niobium/NiobiumSystemInformation")); 
	SystemInformation = sysInfo.Object;

	UE_LOG(NiobiumSystem, Display, TEXT("Compile-time System Information load: %s"), ((SystemInformation != nullptr) ? TEXT("SUCCEEDED") : TEXT("FAILED")));
}

//==============================================================================================================

void UNiobiumComponent::BeginPlay()
{
	Super::BeginPlay();

	FirePosition = nullptr;

	UE_LOG(NiobiumSystem, Display, TEXT("=== Niobium System Activated ========================================="));

	if (SystemInformation != nullptr)
	{
		UE_LOG(NiobiumSystem, Display, TEXT("System Information load: SUCCEEDED"));

		InitialiseCompiler();
		InitialiseComponents();

		UE_LOG(NiobiumSystem, Display, TEXT("=== Niobium System initialised ======================================="));
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("System Information load: FAILED"));

		UE_LOG(NiobiumSystem, Display, TEXT("=== Niobium System error ============================================="));
	}
}

//==============================================================================================================

void UNiobiumComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (InstalledCore != nullptr)
	{
		InstalledCore->Update(DeltaTime);
	}
}

//==============================================================================================================

void UNiobiumComponent::InitialiseCompiler()
{
	CompilerVisible = false;

	if (SystemInformation->Compiler != nullptr)
	{
		CompilerUI = CreateWidget<UNiobiumCompiler>(GetWorld(), SystemInformation->Compiler);
		if (CompilerUI != nullptr)
		{
			CompilerUI->OnCoreInstall.AddDynamic(this, &UNiobiumComponent::InstallCore);
			CompilerUI->OnModifierAddition.AddDynamic(this, &UNiobiumComponent::AddModifier);
			CompilerUI->OnModifierRemoval.AddDynamic(this, &UNiobiumComponent::RemoveModifier);
			CompilerUI->OnGenerateConfig.AddDynamic(this, &UNiobiumComponent::GenerateConfig_UIEvent);
			CompilerUI->OnLoadConfig.AddDynamic(this, &UNiobiumComponent::LoadConfig);

			UE_LOG(NiobiumSystem, Display, TEXT("Compiler User-interface load: SUCCEEDED"));
		}
		else
		{
			UE_LOG(NiobiumSystem, Display, TEXT("Compiler User-interface load: FAILED"));
		}
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("Compiler User-interface is not set in the System Information"));
	}
}

//==============================================================================================================

void UNiobiumComponent::InitialiseComponents()
{
	int coreCount = SystemInformation->Cores.Num();
	for (int i = 0; i < coreCount; ++i)
	{
		UCoreBase* core = InitialiseCore(SystemInformation->Cores[i].Name);
		InitialisedCores.Add(core->GetName(), core);
		if (CompilerUI != nullptr)
		{
			CompilerUI->AddCoreSelector(core);
		}
	}

	int modifierCount = SystemInformation->Modifiers.Num();
	for (int i = 0; i < modifierCount; ++i)
	{
		UModifierBase* modifier = InitialiseModifier(SystemInformation->Modifiers[i].Name);
		InitialisedModifiers.Add(modifier->GetName(), modifier);
		if (CompilerUI != nullptr)
		{
			CompilerUI->AddModifierSelector(modifier);
		}
	}

	if (InitialisedCores.Num() > 0)
	{
		UE_LOG(NiobiumSystem, Display, TEXT("Installing default core:"));
		InstallCore(InitialisedCores[SystemInformation->Cores[0].Name]);
	}
}

//==============================================================================================================

UCoreBase* UNiobiumComponent::InitialiseCore(FName name)
{
	UCoreBase* core = nullptr;
	if (SystemInformation != nullptr)
	{
		int coreIdx = SystemInformation->GetCoreIndexByName(name);
		if (coreIdx >= 0)
		{
			FCoreInformation coreInfo = SystemInformation->Cores[coreIdx];
			core = NewObject<UCoreBase>(this, coreInfo.CoreClass);
			core->Initialise(coreInfo);

			UE_LOG(NiobiumSystem, Display, TEXT("Core [%s] initialised"), *coreInfo.Name.ToString());
		}
		else
		{
			UE_LOG(NiobiumSystem, Display, TEXT("Core [%s] does not exist"), *name.ToString());
		}
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("System Information not loaded, unable to create Core [%s]"), *name.ToString());
	}

	return core;
}

//==============================================================================================================

UModifierBase* UNiobiumComponent::InitialiseModifier(FName name)
{
	UModifierBase* modifier = nullptr;
	if (SystemInformation != nullptr)
	{
		int modIdx = SystemInformation->GetModifierIndexByName(name);
		if (modIdx >= 0)
		{
			FModifierInformation modInfo = SystemInformation->Modifiers[modIdx];
			modifier = NewObject<UModifierBase>(this, modInfo.Class);
			modifier->Initialise(modInfo);

			UE_LOG(NiobiumSystem, Display, TEXT("Modifier [%s] initialised"), *modInfo.Name.ToString());
		}
		else
		{
			UE_LOG(NiobiumSystem, Display, TEXT("Modifier [%s] does not exist"), *name.ToString());
		}
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("System Information not loaded, unable to create Modifier [%s]"), *name.ToString());
	}

	return modifier;
}

//==============================================================================================================

UCoreBase* UNiobiumComponent::GetCore(FName name)
{
	if (InitialisedCores.Contains(name))
	{
		return InitialisedCores[name];
	}
	else
	{
		return nullptr;
	}
}

//==============================================================================================================

TArray<FName> UNiobiumComponent::GetCoreNames()
{
	TArray<FName> names;
	for (TPair<FName, UCoreBase*> core : InitialisedCores)
	{
		names.Add(core.Key);
	}

	return names;
}

//==============================================================================================================

UModifierBase* UNiobiumComponent::GetModifier(FName name)
{
	if(InitialisedModifiers.Contains(name))
	{
		return InitialisedModifiers[name];
	}
	else
	{
		return nullptr;
	}
}

//==============================================================================================================

TArray<FName> UNiobiumComponent::GetModifierNames()
{
	TArray<FName> names;
	for (TPair<FName, UModifierBase*> modifier : InitialisedModifiers)
	{
		names.Add(modifier.Key);
	}

	return names;
}

//==============================================================================================================

void UNiobiumComponent::InstallCore(UCoreBase* core)
{
	if (core != nullptr)
	{
		if (InstalledCore != nullptr)
		{
			InstalledCore->RemoveAllModifiers();
			InstalledCore->SetInstalled(false);
			InstalledCore->ResetAllParameters();

			UE_LOG(NiobiumSystem, Display, TEXT("Core [%s] uninstalled"), *InstalledCore->GetName().ToString());
		}

		InstalledCore = core;
		InstalledCore->SetInstalled(true);
		if (FirePosition != nullptr)
		{
			InstalledCore->SetFirePosition(FirePosition);
		}
		UE_LOG(NiobiumSystem, Display, TEXT("Core [%s] installed"), *InstalledCore->GetName().ToString());
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("Core was nullptr"));
	}
}

//==============================================================================================================

void UNiobiumComponent::AddModifier(UModifierBase* modifier)
{
	if (InstalledCore != nullptr)
	{
		if (modifier != nullptr)
		{
			InstalledCore->AddModifier(modifier->GetName().ToString(), modifier);
		}
		else
		{
			UE_LOG(NiobiumSystem, Display, TEXT("Modifier was nullptr, unable to add modifier"));
		}
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("No Core installed, unable to add modifier"));
	}
}

//==============================================================================================================

void UNiobiumComponent::RemoveModifier(UModifierBase* modifier)
{
	if (InstalledCore != nullptr)
	{
		if (modifier != nullptr)
		{
			InstalledCore->RemoveModifier(modifier->GetName().ToString());
		}
		else
		{
			UE_LOG(NiobiumSystem, Display, TEXT("Modifier was nullptr, unable to remove modifier"));
		}
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("No Core installed, unable to remove modifier"));
	}
}

//==============================================================================================================

FString UNiobiumComponent::GenerateConfig()
{
	FNiobiumConfig config;
	if (InstalledCore != nullptr)
	{
		//=== Core ======================

		TArray<FString> coreParams = InstalledCore->GetParameterNames();
		int coreParamCount = coreParams.Num();

		FNiobiumComponentConfig coreConfig;
		coreConfig.Name = InstalledCore->GetName().ToString();
		for (int i = 0; i < coreParamCount; ++i)
		{
			if (UParameterBase* param = InstalledCore->GetParameter(coreParams[i]))
			{
				coreConfig.Parameters.Add(coreParams[i], param->ToString());
			}
		}

		config.Core = coreConfig;

		//===============================

		TArray<FString> modifiers = InstalledCore->GetModifierNames();
		int modifierCount = modifiers.Num();
		for (int m = 0; m < modifierCount; ++m)
		{
			if (UModifierBase * modifier = InstalledCore->GetModifier(modifiers[m]))
			{
				//=== Modifier ==================

				TArray<FString> modifierParams = modifier->GetParameterNames();
				int modifierParamCount = modifierParams.Num();

				FNiobiumComponentConfig modifierConfig;
				modifierConfig.Name = modifier->GetName().ToString();
				for (int p = 0; p < modifierParamCount; ++p)
				{
					if (UParameterBase * param = modifier->GetParameter(modifierParams[p]))
					{
						modifierConfig.Parameters.Add(modifierParams[p], param->ToString());
					}
				}

				config.Modifiers.Add(modifierConfig);

				//===============================
			}
			else
			{
				UE_LOG(NiobiumSystem, Display, TEXT("Config generation warning, unable to read Modifier [%s]"), *modifiers[m]);
			}
		}
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("Config generation failed, no Core installed"));
	}

	FString json = SerialiseConfig(config);
	return json;
}

void UNiobiumComponent::GenerateConfig_UIEvent()
{
	FString config = GenerateConfig();
	if (CompilerUI != nullptr)
	{
		CompilerUI->DisplayGeneratedConfig(config);
	}
}

//==============================================================================================================

void UNiobiumComponent::LoadConfig(FString configString)
{
	FNiobiumConfig config = DeserialiseConfig(configString);

	UCoreBase* core = GetCore(*config.Core.Name);
	if (core != nullptr)
	{
		//=== Core ======================

		InstallCore(core);
		for (TPair<FString, FString> param : config.Core.Parameters)
		{
			if (UParameterBase * p = core->GetParameter(*param.Key))
			{
				p->FromString(param.Value);
			}
		}

		//===============================

		int modifierCount = config.Modifiers.Num();
		for (int i = 0; i < modifierCount; ++i)
		{
			UModifierBase* modifier = GetModifier(*config.Modifiers[i].Name);
			if (modifier != nullptr)
			{
				//=== Modifier ==================

				AddModifier(modifier);
				for (TPair<FString, FString> param : config.Modifiers[i].Parameters)
				{
					if (UParameterBase * p = modifier->GetParameter(*param.Key))
					{
						p->FromString(param.Value);
					}
				}

				//===============================
			}
			else
			{
				UE_LOG(NiobiumSystem, Display, TEXT("Config load warning, Modifier [%s] does not exist"), *config.Modifiers[i].Name);
			}
		}
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("Config load failed, Core [%s] does not exist"), *config.Core.Name);
	}
}

//==============================================================================================================
/*
Format:
{
	"Core":{
		"Name":"_NAME_",
		"Parameters":{
			"_NAME_":"_VALUE_",
			"_NAME_":"_VALUE_",
			...
		}
	},
	"Modifiers":[
		{
			"Name":"_NAME_",
			"Parameters":{
				"_NAME_":"_VALUE_",
				"_NAME_":"_VALUE_",
				...
			}
		},
		...
	]
}
*/
//==============================================================================================================

FString UNiobiumComponent::SerialiseConfig(FNiobiumConfig config)
{
	FString jsonString = "";
	if (FJsonObjectConverter::UStructToJsonObjectString(config, jsonString, 0, 0, 0, nullptr, false))
	{
		jsonString.Replace(TEXT("\n"), TEXT(""));
		UE_LOG(NiobiumSystem, Display, TEXT("Config Serialise: SUCCEEDED"));
	}
	else
	{
		jsonString = "";
		UE_LOG(NiobiumSystem, Display, TEXT("Config Serialise: FAILED"));
	}
	return jsonString;
}

//==============================================================================================================

FNiobiumConfig UNiobiumComponent::DeserialiseConfig(FString jsonString)
{
	FNiobiumConfig config;
	if (FJsonObjectConverter::JsonObjectStringToUStruct(jsonString, &config, 0, 0))
	{
		UE_LOG(NiobiumSystem, Display, TEXT("Config Deserialise: SUCCEEDED"));
	}
	else
	{
		UE_LOG(NiobiumSystem, Display, TEXT("Config Deserialise: FAILED"));
	}
	return config;
}

//==============================================================================================================

void UNiobiumComponent::SetFirePosition(USceneComponent* firePosition)
{
	FirePosition = firePosition;
	if (InstalledCore != nullptr)
	{
		InstalledCore->SetFirePosition(FirePosition);
	}
}

//==============================================================================================================

void UNiobiumComponent::FirePressed()
{
	if (InstalledCore != nullptr)
	{
		InstalledCore->SetFireHeld(true);
	}
}

//==============================================================================================================

void UNiobiumComponent::FireReleased()
{
	if (InstalledCore != nullptr)
	{
		InstalledCore->SetFireHeld(false);
	}
}

//==============================================================================================================

void UNiobiumComponent::SetCompilerVisible(bool state)
{
	CompilerVisible = state;
	if (CompilerVisible)
	{
		CompilerUI->AddToViewport();
	}
	else
	{
		CompilerUI->RemoveFromViewport();
	}
}

//==============================================================================================================