// Copyright Epic Games, Inc. All Rights Reserved.


#include "Weapon/WeaponActor.h"


FPrimaryAssetId AWeaponActor::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(Type, GetFName());
}

