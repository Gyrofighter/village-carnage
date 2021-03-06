// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VCBaseCharacter.generated.h"

class UVCHealthComponent;

UCLASS()
class VILLAGECARNAGE_API AVCBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVCBaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UVCHealthComponent* HealthComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	

	

	

};
