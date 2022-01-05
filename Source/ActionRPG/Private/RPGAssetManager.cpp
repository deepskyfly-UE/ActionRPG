// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGAssetManager.h"
#include "Items/RPGItem.h"
#include "AbilitySystemGlobals.h"
#include "Items/RPGWeaponItem.h"

const FPrimaryAssetType	URPGAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	URPGAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType	URPGAssetManager::TokenItemType = TEXT("Token");
const FPrimaryAssetType	URPGAssetManager::WeaponItemType = TEXT("Weapon");

URPGAssetManager& URPGAssetManager::Get()
{
	URPGAssetManager* This = Cast<URPGAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogActionRPG, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<URPGAssetManager>(); // never calls this
	}
}

void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}


URPGItem* URPGAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{	
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	URPGItem* LoadedItem = Cast<URPGItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogActionRPG, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}

void URPGAssetManager::AssetManagerExample() 
{
	URPGAssetManager& AssetManager = URPGAssetManager::Get();

	// Get a list of all weapons that can be loaded
	TArray<FPrimaryAssetId > WeaponIdList; 	
	AssetManager.GetPrimaryAssetIdList(WeaponItemType, WeaponIdList);
	
	for (const FPrimaryAssetId& WeaponId : WeaponIdList)
	{
		// Get tag / value data for an unloaded weapon
		FAssetData AssetDataToParse;
		AssetManager.GetPrimaryAssetData(WeaponId, AssetDataToParse);

		FName QueryExample;
		AssetDataToParse.GetTagValue(GET_MEMBER_NAME_CHECKED(URPGItem, ExampleRegistryTag), QueryExample);
		UE_LOG(LogTemp, Log, TEXT(" Read Exampleregstry Tag % s from Weapon % s "), *QueryExample.ToString(), *AssetDataToParse.AssetName.ToString());

	}

	// Permanently load a single item
	TArray < FName > CurrentLoadState;
	CurrentLoadState.Add(FName("Game"));
	
	FName WeaponName = FName("Weapon_Hammer_3");
	FPrimaryAssetId WeaponId = FPrimaryAssetId(WeaponItemType, WeaponName);
	AssetManager.LoadPrimaryAsset(WeaponId, CurrentLoadState, FStreamableDelegate::CreateUObject(&AssetManager, &URPGAssetManager::CallbackFunction, WeaponId));
		
	TArray<FPrimaryAssetId > ListOfPrimaryAssetIds;
	AssetManager.GetPrimaryAssetIdList(SkillItemType, ListOfPrimaryAssetIds);

	// Load a list of items as long as Handle is alive
	TSharedPtr<FStreamableHandle> Handle = AssetManager.PreloadPrimaryAssets(ListOfPrimaryAssetIds, CurrentLoadState, false);

	// Store Handle somewhere
}


void URPGAssetManager::CallbackFunction(FPrimaryAssetId WeaponId)
{
	URPGWeaponItem* Weapon = GetPrimaryAssetObject< URPGWeaponItem >(WeaponId);
	if (Weapon) 
	{
		UE_LOG(LogTemp, Log, TEXT(" Loaded Weapon % s "), *Weapon->GetName());		
	}
	// Release previously loaded item
	UnloadPrimaryAsset(WeaponId);
}
