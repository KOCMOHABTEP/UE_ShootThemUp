// Shoot Them Up Game. All Rights Reserved.

#include "STURifleWeapon.h"
#include "DrawDebugHelpers.h"

#include "Engine/DamageEvents.h"

void ASTURifleWeapon::StartFire()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
	} else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}

	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation; // GetMuzzleWorldLocation();
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad) ; // SocketTransform.GetRotation().GetForwardVector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	UE_LOG(LogTemp, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
	
}
