// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VCHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UVCHealthComponent::UVCHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;

}

#if WITH_EDITOR
void UVCHealthComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	InitialHealth = FMath::Clamp(InitialHealth, 0.0f, MaxHealth);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif //WITH_EDITOR



void UVCHealthComponent::AddHealth(const float DeltaHealth)
{
	DecreaseHealth(-DeltaHealth);
}

void UVCHealthComponent::DealDamage(const float Damage)
{
	DecreaseHealth(Damage);
}

void UVCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwnerRole() == ROLE_Authority)
	{
		Health = InitialHealth;
		OnHealthChanged.Broadcast(InitialHealth, Health, MaxHealth != 0.0f ? Health / MaxHealth : 0.0f);
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UVCHealthComponent::TakeDamage);

		StartRegenerationTimer();
	}
}

void UVCHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	DecreaseHealth(Damage);
}


void UVCHealthComponent::DecreaseHealth(const float DeltaHealth)
{
	if (bDead) return;

	float OldHealth = Health;
	Health = FMath::Clamp(Health - DeltaHealth, 0.0f, MaxHealth);	

	if (Health < OldHealth)
	{
		OnTakeDamage.Broadcast(OldHealth - Health, Health, MaxHealth != 0.0f ? Health / MaxHealth : 0.0f);
		OnHealthChanged.Broadcast(Health - OldHealth, Health, MaxHealth != 0.0f ? Health / MaxHealth : 0.0f);
		StartRegenerationTimer();
	}
	else if (Health > OldHealth)
	{
		OnHealthChanged.Broadcast(Health - OldHealth, Health, MaxHealth != 0.0f ? Health / MaxHealth : 0.0f);
	}
	else
	{
		return;
	}

	if (Health <= 0.0f)
	{
		Die();
	}
}

void UVCHealthComponent::Die()
{
	Health = 0.0f;
	bDead = true;
	OnDeath.Broadcast();
	StopRegenerationTimer();
}

void UVCHealthComponent::Regeneration()
{
	UE_LOG(LogHealthComponent, Warning, TEXT("Regeneration works"));

	DecreaseHealth(-HealthPerRegenataion);

	if (HealthPerRegenataion > 0.0f && Health >= MaxHealth)
	{
		StopRegenerationTimer();
	}
}

void UVCHealthComponent::StartRegenerationTimer()
{
	if (!GetWorld() || RegenerationTimerHandle.IsValid() ||RegenerationRate == 0.0f || HealthPerRegenataion == 0.0f || (HealthPerRegenataion > 0.0f && Health >= MaxHealth)) return;

	UE_LOG(LogHealthComponent, Warning, TEXT("START"));

	GetWorld()->GetTimerManager().SetTimer(RegenerationTimerHandle, this, &UVCHealthComponent::Regeneration, RegenerationRate, true, 0.0f);
}

void UVCHealthComponent::StopRegenerationTimer()
{
	if (!GetWorld() || !RegenerationTimerHandle.IsValid()) return;

	UE_LOG(LogHealthComponent, Warning, TEXT("STOP"));

	GetWorld()->GetTimerManager().ClearTimer(RegenerationTimerHandle);
}



