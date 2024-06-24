//==============================================================================================================

#include "NiobiumDataAsset.h"

//==============================================================================================================

int UNiobiumDataAsset::GetCoreIndexByName(FName name)
{
	int cores = Cores.Num();
	for (int i = 0; i < cores; ++i)
	{
		if (Cores[i].Name == name)
		{
			return i;
		}
	}
	return -1;
}

//==============================================================================================================

int UNiobiumDataAsset::GetModifierIndexByName(FName name)
{
	int mods = Modifiers.Num();
	for (int i = 0; i < mods; ++i)
	{
		if (Modifiers[i].Name == name)
		{
			return i;
		}
	}
	return -1;
}

//==============================================================================================================

TArray<FName> UNiobiumDataAsset::GetCoreNames()
{
	TArray<FName> names;
	for (int i = 0; i < Cores.Num(); ++i)
	{
		names.Add(Cores[i].Name);
	}

	return names;
}

//==============================================================================================================

TArray<FName> UNiobiumDataAsset::GetModifierNames()
{
	TArray<FName> names;
	for (int i = 0; i < Modifiers.Num(); ++i)
	{
		names.Add(Modifiers[i].Name);
	}

	return names;
}

//==============================================================================================================