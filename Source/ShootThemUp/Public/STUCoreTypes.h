﻿#pragma once
#include "STUCoreTypes.generated.h"

//  WeaponComponent
class ASTUBaseWeapon;
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

// Health Component
DECLARE_MULTICAST_DELEGATE(FonDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon", meta = (EditCondition = "!Infinite"))
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<ASTUBaseWeapon> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	UAnimMontage* ReloadAnimMontage;
};