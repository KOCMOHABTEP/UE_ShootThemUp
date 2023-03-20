// Shoot Them Up Game. All Rights Reserved.


#include "Player/STUBaseCharacter.h"

#include "STUCharacterMovementComponent.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(GetCharacterMovement());
	check(GetMesh());

	OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Пример нанесения урона самому себе
	// TakeDamage(0.1f, FDamageEvent{}, Controller,this);
}

void ASTUBaseCharacter::TurnOff()
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);
	Super::TurnOff();
}

void ASTUBaseCharacter::Reset()
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);
	Super::Reset();
}

bool ASTUBaseCharacter::IsRunning() const
{
	return false;
}

float ASTUBaseCharacter::GetMovementDirection() const
{

	if (GetVelocity().IsZero())
	{
		return 0.0f;		
	}
	
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z); 
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if (!MaterialInst) return;

	MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
}


void ASTUBaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

	// PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
	
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& HitResult)
{
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
	UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X)
	{
		return;
	}

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(BaseCharacterLog, Display, TEXT("Final Damage: %f"), FinalDamage);
	
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}


