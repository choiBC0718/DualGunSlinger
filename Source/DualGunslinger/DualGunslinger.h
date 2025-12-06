// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGunType : uint8
{
	Pistol,
	Rifle
};

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	PistolWeak,
	RifleWeak,
	Hybrid
};