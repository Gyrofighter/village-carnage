// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VCHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamage, float, Damage, float, NewHealth, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, DeltaHealth, float, NewHealth, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VILLAGECARNAGE_API UVCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UVCHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, Meta = (ClampMin = 0.0f))
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, Meta = (ClampMin = 0.0f))
	float InitialHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, Meta = (ClampMin = 0.0f))
	float RegenerationRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float HealthPerRegenataion = 1.0f;

		

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Health)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Health)
	bool IsDead() const { return bDead; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Health)
	void AddHealth(const float DeltaHealth);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Health)
	void DealDamage(const float Damage);



	UPROPERTY(BlueprintAssignable, Category = Health)
	FOnTakeDamage OnTakeDamage;

	UPROPERTY(BlueprintAssignable, Category = Health)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = Health)
	FOnDeath OnDeath;
	

		

protected:
	
	virtual void BeginPlay() override;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR

private:

	float Health;

	bool bDead = false;

	FTimerHandle RegenerationTimerHandle;



	void Die();

	void DecreaseHealth(const float DeltaHealth);

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void Regeneration();

	void StartRegenerationTimer();

	void StopRegenerationTimer();
		
};
