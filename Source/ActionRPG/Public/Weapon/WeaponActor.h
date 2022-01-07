// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataAsset.h"
#include "WeaponActor.generated.h"

UCLASS()
class ACTIONRPG_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FPrimaryAssetType Type = TEXT("BPWeapon");

public:	

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

};
