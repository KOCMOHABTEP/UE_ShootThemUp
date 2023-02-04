// Shoot Them Up Game. All Rights Reserved.


#include "Components/STUHealthComponent.h"

#include "STUIceDamageType.h"
#include "STYFireDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	AActor* ComponentOwner = GetOwner();

	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;

	UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

	if (DamageType)
	{
		if (DamageType->IsA<USTYFireDamageType>())
		{
			UE_LOG(LogHealthComponent, Display, TEXT("So hooooooot!!!"));
		} else if (DamageType->IsA<USTUIceDamageType>())
		{
			UE_LOG(LogHealthComponent, Display, TEXT("So cooooold!!!"));
		}
	}
}

